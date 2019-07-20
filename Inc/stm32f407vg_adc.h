/*
* Functionality for handling the ADC 
* specifically for the stm32f07VG MCU
* (as it is the one I mess around with)
*
* TODO: This interface does not use all features
*       of the ADC in the STM32F04x platform
*       for example, it uses only regular sequence
*       conversions.
*/

#include "gpio.h" // Heavily reliant on the the GPIO interface
#include "utils.h"
#include "nvic.h"
#include "cmsis_os.h"
#include "stm32f4xx.h"

#include <string.h>

#define ADC_BASE_ADDRESS   0x40012000

#define ADC_CLOCK_RCC      (0x40023800+0x44)

#define ADC1_OFFSET        0x000
#define ADC2_OFFSET        0x100
#define ADC3_OFFSET        0x200
#define ADC_COMMON_OFFSET  0x300

#define ADC1_IN0  0
#define ADC1_IN1  1
#define ADC1_IN2  2
#define ADC1_IN3  3
#define ADC1_IN4  4
#define ADC1_IN5  5
#define ADC1_IN6  6
#define ADC1_IN7  7
#define ADC1_IN8  8
#define ADC1_IN9  9
#define ADC1_IN10 10
#define ADC1_IN11 11
#define ADC1_IN12 12
#define ADC1_IN13 13
#define ADC1_IN14 14
#define ADC1_IN15 15
#define ADC1_IN16 16
#define ADC1_IN17 17
#define ADC1_IN18 18
#define ADC2_IN0  19
#define ADC2_IN1  20
#define ADC2_IN2  21
#define ADC2_IN3  22
#define ADC2_IN4  23
#define ADC2_IN5  24
#define ADC2_IN6  25
#define ADC2_IN7  26
#define ADC2_IN8  27
#define ADC2_IN9  28
#define ADC2_IN10 29
#define ADC2_IN11 30
#define ADC2_IN12 31
#define ADC2_IN13 32
#define ADC2_IN14 33
#define ADC2_IN15 34
#define ADC2_IN16 35
#define ADC2_IN17 36
#define ADC2_IN18 37
#define ADC3_IN0  38
#define ADC3_IN1  39
#define ADC3_IN2  40
#define ADC3_IN3  41
#define ADC3_IN4  42
#define ADC3_IN5  43
#define ADC3_IN6  44
#define ADC3_IN7  45
#define ADC3_IN8  46
#define ADC3_IN9  47
#define ADC3_IN10 48
#define ADC3_IN11 49
#define ADC3_IN12 50
#define ADC3_IN13 51
#define ADC3_IN14 52
#define ADC3_IN15 53
#define ADC3_IN16 54
#define ADC3_IN17 55
#define ADC3_IN18 56
#define ADC_LAST  56

#define ADC_SAMPLE_TIME_3_CYCLES   0
#define ADC_SAMPLE_TIME_15_CYCLES  BIT(0)
#define ADC_SAMPLE_TIME_28_CYCLES  BIT(1)
#define ADC_SAMPLE_TIME_56_CYCLES  (BIT(1) + BIT(0))
#define ADC_SAMPLE_TIME_84_CYCLES  BIT(2)
#define ADC_SAMPLE_TIME_112_CYCLES (BIT(2) + BIT(0))
#define ADC_SAMPLE_TIME_144_CYCLES (BIT(2) + BIT(1))
#define ADC_SAMPLE_TIME_480_CYCLES (BIT(2) + BIT(1) + BIT(0))

// VSENSE is input to ADC1_IN16 for the STM23F40x and STM32F41x devices and 
// to ADC1_IN18 for the STM32F42x and STM32F43x devices.

// ADC registers
typedef struct adc_conf_s {
	int32_t sr;
	int32_t cr1;
	int32_t cr2;
	int32_t smpr1;
	int32_t smpr2;
	int32_t jofr[4];
	int32_t htr;
	int32_t ltr;
	int32_t sqr[3];
	int32_t jsqr;
	int32_t jdr[4];
	int32_t dr;
} adc_conf_t;

// Common ADC registers
typedef struct adc_common_conf_t {
	int32_t csr;
	int32_t ccr;
	int32_t cdr;
} adc_common_conf_t;

typedef struct adc_channels_t {
	int len;
	int sampling_rate;
	int chan[16];
} adc_channels_t;

int adc_init();
int adc_configure_channels(adc_channels_t *channels);
void adc_start_conversion();
float adc_read_value(int ch);



