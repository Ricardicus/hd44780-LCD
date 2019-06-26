#include "gpio.h"

// GPIOD = 0x4002 0C00 - 0x4002 0FFF
/*
* Led 3,5,6,4 (clock wise round)
*/

void gpio_rcc_init(uint32_t base) 
{
	static int rcc_gpio_enabled[11];
	static int first_call = 1;
	uint32_t *rcc_enable = (uint32_t*) (RCC_ADDR + RCC_HAL_AHB1ENR_OFFSET);
	int entry;

	if ( first_call ) {
		memset(rcc_gpio_enabled, 0, sizeof(rcc_gpio_enabled));
		first_call = 0;
	}

	entry = (base - GPIOA_ADDR) >> 10;

	if ( entry < 11 && ! rcc_gpio_enabled [entry] ) {
		*rcc_enable |= BIT(entry);
		(void)*rcc_enable;
	}

}

/* Terminating entry has pin set to a negative number */
void init_leds(gpio_setting_t *settings)
{
	gpio_conf_t *gpio_conf;
	int i = 0;
	gpio_setting_t setting = settings[0];

	while ( setting.pin >= 0 ) {

		int pin = setting.pin;

		gpio_rcc_init(setting.base);

		gpio_conf = (gpio_conf_t*) setting.base;

		gpio_conf->mode  |= ((setting.mode)<<(pin*2));
		gpio_conf->type  |= ((setting.type)<<(pin));
		gpio_conf->speed |= ((setting.speed)<<(pin*2));
		gpio_conf->pupdr |= ((setting.pupdr)<<(pin*2));

		++i;
		setting = settings[i];
	}
}

int32_t gpio_gp_pin_get(uint32_t base, uint32_t pin)
{
	gpio_conf_t *gpio_conf = (gpio_conf_t*) base;

	if (pin > 15)
		return -1;

	return (int32_t) (gpio_conf->idr & BIT(pin)) > 0 ? 1 : 0;
}

void gpio_gp_pin_set(uint32_t base, uint32_t pin, int32_t val)
{
	gpio_conf_t *gpio_conf = (gpio_conf_t*) base;

	if ( pin > 15 )
		return;

	if ( val > 0 ) {
		gpio_conf->odr |= BIT(pin);
	} else {
		gpio_conf->odr &= (~BIT(pin));
	}
}