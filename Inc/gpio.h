#ifndef RICKARD_GPIO_H
#define RICKARD_GPIO_H 

#include <inttypes.h>
#include <string.h>
#include "cmsis_os.h"

#define BIT(x) ( (1<<(x)) )

typedef struct gpio_conf_s {
  uint32_t mode;
  uint32_t type;
  uint32_t speed;
  uint32_t pupdr;
  uint32_t idr;
  uint32_t odr;
  uint32_t bsrr;
  uint32_t lckr;
  uint32_t afrl;
  uint32_t afrh;
} gpio_conf_t;

typedef struct gpio_setting_t {
  int32_t pin;
  int32_t base;
  int32_t mode;
  int32_t type;
  int32_t speed;
  int32_t pupdr;
} gpio_setting_t;

#define GPIOA_ADDR               ((uint32_t) 0x40020000)
#define GPIOB_ADDR               ((uint32_t) 0x40020400)
#define GPIOC_ADDR               ((uint32_t) 0x40020800)
#define GPIOD_ADDR               ((uint32_t) 0x40020C00)
#define GPIOE_ADDR               ((uint32_t) 0x40021000)
#define GPIOF_ADDR               ((uint32_t) 0x40021400)
#define GPIOG_ADDR               ((uint32_t) 0x40021800)
#define GPIOH_ADDR               ((uint32_t) 0x40021C00)
#define GPIOI_ADDR               ((uint32_t) 0x40022000)
#define GPIOJ_ADDR               ((uint32_t) 0x40022400)
#define GPIOK_ADDR               ((uint32_t) 0x40022800)

#define GPIO_SETTING_MODE_IN             ((uint32_t) 0)
#define GPIO_SETTING_MODE_GP_OUT         ((uint32_t) 1)
#define GPIO_SETTING_MODE_AF             ((uint32_t) 2)
#define GPIO_SETTING_MODE_ANALOG         ((uint32_t) 3)
#define GPIO_SETTING_TYPE_PUSH_PULL      ((uint32_t) 0)
#define GPIO_SETTING_TYPE_OPEN_DRAIN     ((uint32_t) 1)
#define GPIO_SETTING_SPEED_LOW           ((uint32_t) 0)
#define GPIO_SETTING_SPEED_MEDIUM        ((uint32_t) 1)
#define GPIO_SETTING_SPEED_HIGH          ((uint32_t) 2)
#define GPIO_SETTING_SPEED_VERY_HIGH     ((uint32_t) 3)
#define GPIO_SETTING_PUPDR_NO_PULL_UP    ((uint32_t) 0)
#define GPIO_SETTING_PUPDR_PULL_UP       ((uint32_t) 1)
#define GPIO_SETTING_PUPDR_PULL_DOWN     ((uint32_t) 2)     

#define RCC_ADDR                 ((uint32_t) 0x40023800)
#define RCC_HAL_AHB1ENR_OFFSET   ((uint32_t) 0x30)
#define RCC_HAL_AHB1ENR_GPIOAEN  BIT(0)
#define RCC_HAL_AHB1ENR_GPIOBEN  BIT(1)
#define RCC_HAL_AHB1ENR_GPIOCEN  BIT(2)
#define RCC_HAL_AHB1ENR_GPIODEN  BIT(3)
#define RCC_HAL_AHB1ENR_GPIOEEN  BIT(4)
#define RCC_HAL_AHB1ENR_GPIOFEN  BIT(5)
#define RCC_HAL_AHB1ENR_GPIOGEN  BIT(6)
#define RCC_HAL_AHB1ENR_GPIOHEN  BIT(7)
#define RCC_HAL_AHB1ENR_GPIOIEN  BIT(8)
#define RCC_HAL_AHB1ENR_GPIOJEN  BIT(9)
#define RCC_HAL_AHB1ENR_GPIOKEN  BIT(10)

#define LED_PATTERN_0            BIT(0)
#define LED_PATTERN_1            BIT(1)

void init_leds(gpio_setting_t *settings);
void gpio_gp_pin_set(uint32_t base, uint32_t pin, int32_t val);
int32_t gpio_gp_pin_get(uint32_t base, uint32_t pin);

#endif