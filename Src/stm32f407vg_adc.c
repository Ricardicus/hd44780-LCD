#include "stm32f407vg_adc.h"

static volatile int32_t isr_adc_channel_values[ADC_LAST+1]; // Used by the ISR
static volatile int32_t isr_adc_regular_channel_count[3] = {0,0,0}; // Used by the ISR
static volatile int32_t isr_adc_channels[3][16]; // Used by the ISR
static volatile int32_t isr_adc_regular_channel_idx[3] = {0,0,0}; // Used by the ISR

static const int32_t adcs[3] = {
	ADC_BASE_ADDRESS + ADC1_OFFSET,
	ADC_BASE_ADDRESS + ADC2_OFFSET,
	ADC_BASE_ADDRESS + ADC3_OFFSET
};

void ADC_IRQHandler()
{
	volatile adc_conf_t* conf;
	int i = 0;
	int32_t channel;

	while ( i < 3 ) {
		conf = (volatile adc_conf_t*) adcs[i];

		if ( conf->sr & BIT(1) ) {
			channel = isr_adc_channels[i][isr_adc_regular_channel_idx[i]];

			isr_adc_regular_channel_idx[i] = ((isr_adc_regular_channel_idx[i] + 1) & 15 ) % isr_adc_regular_channel_count[i];

			isr_adc_channel_values[channel] = conf->dr;
		}
		++i;
	}

}

int adc_init() 
{
	adc_conf_t *conf;
	adc_common_conf_t *adc_common_conf;

	int32_t *reg;
	int status = 0;
	int i = 0;

	memset(isr_adc_channel_values, 0, sizeof(isr_adc_channel_values));
	memset(isr_adc_regular_channel_idx, 0, sizeof(isr_adc_regular_channel_idx));
	memset(isr_adc_regular_channel_count, 0, sizeof(isr_adc_regular_channel_count));

	ENABLE_IRQ()

  	NVIC_SET_PRIO(18, 10);
  	NVIC_ENABLE(18);

	adc_common_conf = (adc_common_conf_t*) (ADC_BASE_ADDRESS + ADC_COMMON_OFFSET);

	// Turning on the ADC clock (APB2)
	reg = (int32_t*)ADC_CLOCK_RCC;
	*reg |= BIT(8) + BIT(9) + BIT(10);

	// Give it some time
	busy_wait(5000);

	// Turn on the temperature sensor
	adc_common_conf->ccr |= BIT(23);
	// Set ADC prescaler to 8
	adc_common_conf->ccr |= BIT(16) + BIT(17);
	// Set delay between sampling phases to 20 * T_ADCCLK
	adc_common_conf->ccr |= BIT(8) + BIT(9) + BIT(10) + BIT(11);
	// All the ADCs work independently
	adc_common_conf->ccr &= ( ~BIT(0) & ~BIT(1) & ~BIT(2) & ~BIT(3) & ~BIT(4) );

	while ( i < 3 ) {
		conf = (adc_conf_t*) adcs[i];

		// Power on the ADC
		conf->cr2 |= BIT(0); // ADON
		// Give it some time
		busy_wait(50000);
		// Set alignment
		conf->cr2 &= ~BIT(11); // ALIGN (right alignment)
		// Set SCAN mode
		conf->cr1 |= BIT(8); // SCAN
		// Interrupt enable for EOC
		conf->cr1 |= BIT(5); // EOCIE
		// The EOC bit is set at the end of each regular conversion
		conf->cr2 |= BIT(10); // EOCS
		// Overrun interrupts disabled
		conf->cr1 &= ~BIT(26); // OVR

		++i;
	}

	return status;
}

void adc_start_conversion()
{
	volatile adc_conf_t* conf;
	int32_t i = 0;
	int32_t n = 0;

	while ( i < 3 ) {
		int len = isr_adc_regular_channel_count[i];
		n = 0;

		while ( n < len ) {

			conf = (volatile adc_conf_t*)adcs[i];

			// Start conversion
			conf->cr2 |= BIT(30);

			while ( conf->cr2 & BIT(30) ) {
				// wait until conversion starts
			}

			++n;
		}

		++i;
	}

}

float adc_read_value(int ch)
{
	float value = 0.0;

	int32_t val;

	if ( ch > ADC_LAST )
		return 0.0;
	val = isr_adc_channel_values[ch];

	value = (float)val;

	return value;
}

int adc_configure_channels(adc_channels_t *channels)
{
	adc_conf_t *conf;

	int sequence_lengths[3] = {0,0,0};

	int status = 0;
	int i = 0;

	while ( i < channels->len ) {

		int channel = channels->chan[i];
		int seq_n;
		int channel_select = 0;
		int32_t *seq_reg;
		int adcn_chx_x = channel % 19;

		if ( channel < 19 ) {
			// ADC1
			conf = (adc_conf_t*) ADC_BASE_ADDRESS + ADC1_OFFSET;
			channel_select = 0;
		} else if ( channel < 38 ) {
			// ADC2
			conf = (adc_conf_t*) ADC_BASE_ADDRESS + ADC2_OFFSET;
			channel_select = 1;
		} else {
			// ADC3
			conf = (adc_conf_t*) ADC_BASE_ADDRESS + ADC3_OFFSET;
			channel_select = 2;
		}

		isr_adc_channels[channel_select][sequence_lengths[channel_select]] = adcn_chx_x;

		if ( sequence_lengths[channel_select] < 6 ) {
			seq_reg = &conf->sqr[2];
		} else if ( sequence_lengths[channel_select] < 12 ) {
			seq_reg = &conf->sqr[1];
		} else {
			seq_reg = &conf->sqr[0];
		}

		// Write the channel number in the sequence
		seq_n = sequence_lengths[channel_select] % 6;
		*seq_reg |= (adcn_chx_x << (5*seq_n));

		// set the sampling rate of this channel
		if ( adcn_chx_x < 10 ) {
			conf->smpr1 |= (channels->sampling_rate << (3*(adcn_chx_x%10)));
		} else {
			conf->smpr2 |= (channels->sampling_rate << (3*(adcn_chx_x%10)));
		}

		sequence_lengths[channel_select]++;

		isr_adc_regular_channel_count[channel_select] = sequence_lengths[channel_select];

		// Update the channel length
		conf->sqr[0] |= ( (sequence_lengths[channel_select]-1) << 20 );

		++i;
	}

	return status;
}
