#include "stm32f407vg_adc.h"

#ifdef USE_ADC_ISR
static volatile int32_t isr_adc_channel_values[ADC_LAST+1]; // Used by the ISR
static volatile int32_t isr_adc_regular_channel_count[3] = {0,0,0}; // Used by the ISR
static volatile int32_t isr_adc_channels[3][16]; // Used by the ISR
#else 
static volatile int32_t adc_regular_channel_count[3] = {0,0,0};
static volatile int32_t adc_channel_values[ADC_LAST+1];
static volatile int32_t adc_channels[3][16];
#endif

extern volatile int debug_int;

int adc_init() 
{
	adc_conf_t *conf;
	adc_common_conf_t *adc_common_conf;

	int32_t *reg;
	int status = 0;
	int i = 0;

	int32_t adcs[3] = {
		ADC_BASE_ADDRESS + ADC1_OFFSET,
		ADC_BASE_ADDRESS + ADC2_OFFSET,
		ADC_BASE_ADDRESS + ADC3_OFFSET
	};

#ifdef USE_ADC_ISR
	memset(isr_adc_channel_values, 0, sizeof(isr_adc_channel_values));
#else
	memset(adc_channel_values, 0, sizeof(adc_channel_values));
#endif

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
		busy_wait(5000);
		// Set alignment
		conf->cr2 &= ~BIT(11); // ALIGN (right alignment)
		// Set SCAN mode
		conf->cr1 |= BIT(8); // SCAN
		// Interrupt disable for EOC
		conf->cr1 &= ~BIT(5); // EOCIE
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
	int32_t channel = 0;
	int32_t i = 0;
	int32_t n = 0;

	int32_t adcs[3] = {
		ADC_BASE_ADDRESS + ADC1_OFFSET,
		ADC_BASE_ADDRESS + ADC2_OFFSET,
		ADC_BASE_ADDRESS + ADC3_OFFSET
	};

	debug_int = 6;

	while ( i < 3 ) {
#ifdef USE_ADC_ISR
		int len = isr_adc_regular_channel_count[i];
#else
		int len = adc_regular_channel_count[i];
#endif
		n = 0;

		while ( n < len ) {
			int32_t val; 

			conf = (volatile adc_conf_t*)adcs[i];

#ifdef USE_ADC_ISR
			channel = isr_adc_channels[i][n] + 19*i;
#else
			channel = adc_channels[i][n] + 19*i;
#endif

			// Start conversion
			conf->cr2 |= BIT(30);

			while ( conf->cr2 & BIT(30) ) {
				// wait until conversion starts
			}

#ifndef USE_ADC_ISR
			while ( !(conf->sr & BIT(1))  ) {
				// wait until conversion finished
				debug_int++;
			}
			val = conf->dr;

			debug_int = val;

			adc_channel_values[channel] = val;
#endif

			++n;
		}

		++i;
	}

	NVIC_TRIGGER_INT(18);

//	debug_int = 10;
}

float adc_read_value(int ch)
{
	float value = 0.0;

	int32_t val;

	if ( ch > ADC_LAST )
		return 0.0;
#ifdef USE_ADC_ISR
	val = isr_adc_channel_values[ch];
#else
	val = adc_channel_values[ch];
#endif

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
#ifdef USE_ADC_ISR
		isr_adc_channels[channel_select][sequence_lengths[channel_select]] = adcn_chx_x;
#else
		adc_channels[channel_select][sequence_lengths[channel_select]] = adcn_chx_x;
#endif
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

#ifdef USE_ADC_ISR
		isr_adc_regular_channel_count[channel_select] = sequence_lengths[channel_select];
#else 
		adc_regular_channel_count[channel_select] = sequence_lengths[channel_select];
#endif

		// Update the channel length
		conf->sqr[0] |= ( (sequence_lengths[channel_select]-1) << 20 );

		++i;
	}

	return status;
}
