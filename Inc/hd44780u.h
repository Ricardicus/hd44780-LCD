#ifndef HD44780U_H
#define HD44780U_H

#include "gpio.h"
#include "cmsis_os.h"
#include <inttypes.h>

#define HD44780U_DB4             0
#define HD44780U_DB5             1
#define HD44780U_DB6             2
#define HD44780U_DB7             3
#define HD44780U_E               4
#define HD44780U_RS              5

#define HD44780U_BUSY_DELAY      200

typedef struct hd44780u_pin {
	int pin;
	uint32_t base;
} hd44780u_pin_t;

typedef struct hd44780u_config {
	int type;
	hd44780u_pin_t pin;
} hd44780u_config_t;

int  hd44780u_init_4bit_op(hd44780u_config_t *configs);
void hd44780u_4bit_write(const char * message);
void hd44780u_4bit_shift_display_left();
void hd44780u_4bit_shift_display_right();
void hd44780u_4bit_shift_cursor_left();
void hd44780u_4bit_shift_cursor_right();
static void hd44780u_4bit_instruct(int rs, int db7, int db6, int db5, int db4);

#endif 