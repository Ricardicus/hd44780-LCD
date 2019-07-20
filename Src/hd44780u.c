#include "hd44780u.h"

/*
* This interface only works for one HD44780u display
*/

static void hd44780u_4bit_instruct(int rs, int db7, int db6, int db5, int db4);

static hd44780u_config_t configuration[6];
static TickType_t xDelay;
static volatile int activated = 0;
static volatile int message_written = 0;

static SemaphoreHandle_t hd44780u_mux;

int hd44780u_init_4bit_op(hd44780u_config_t *configs)
{
	hd44780u_config_t config = configs[0];
	int i = 0;
	unsigned int delay = 100000;

	hd44780u_mux = xSemaphoreCreateBinary();

	if ( hd44780u_mux == NULL )
		return 1;

	for (; i < 6; i++ ) {
		configuration[i].type = -1;
	}

	i = 0;
	while ( config.type >= 0 ) {

		switch ( config.type ) {
		case HD44780U_DB4:
		configuration[HD44780U_DB4] = configs[i];
		break;
		case HD44780U_DB5:
		configuration[HD44780U_DB5] = configs[i];
		break;
		case HD44780U_DB6:
		configuration[HD44780U_DB6] = configs[i];
		break;
		case HD44780U_DB7:
		configuration[HD44780U_DB7] = configs[i];
		break;
		case HD44780U_E:
		configuration[HD44780U_E] = configs[i];
		break;
		case HD44780U_RS:
		configuration[HD44780U_RS] = configs[i];
		break;
		default:
		break;
		}

		++i;
		config = configs[i];
	}

	i = 0;
	for (; i < 6; i++ ) {
		if ( -1 == configuration[i].type ) {
			return 1;
		}
	}

	// Program the HD44780u
	xDelay = HD44780U_BUSY_DELAY; //; / portTICK_PERIOD_MS; // Interval between instructions

	// Set to 4 bit operation
	hd44780u_4bit_instruct(0, 0, 0, 1, 0);
	busy_wait(delay);

	// Set 4 bit operation and select 1-line display and 5x8 char font
	hd44780u_4bit_instruct(0, 0, 0, 1, 0);
	busy_wait(delay);

	hd44780u_4bit_instruct(0, 0, 0, 0, 0);
	busy_wait(delay);

	// Turn on display and cursor
	hd44780u_4bit_instruct(0, 0, 0, 0, 0);
	busy_wait(delay);
	hd44780u_4bit_instruct(0, 1, 1, 1, 0);
	busy_wait(delay);

	// Set mode to increment the address by one and to shift the cursor to the right after write
	hd44780u_4bit_instruct(0, 0, 0, 0, 0);
	busy_wait(delay);
	hd44780u_4bit_instruct(0, 0, 1, 1, 0);
	busy_wait(delay);

	// Return home
	hd44780u_4bit_instruct(0, 0, 0, 0, 0);
	busy_wait(delay);
	hd44780u_4bit_instruct(0, 0, 0, 1, 0);
	busy_wait(delay);

	xSemaphoreGive(hd44780u_mux);

	activated = 1;

	return 0;

}

void hd44780u_4bit_return_home()
{
	if (! activated ) 
		return;
	// Return home
	hd44780u_4bit_instruct(0, 0, 0, 0, 0);
	vTaskDelay(xDelay);
	hd44780u_4bit_instruct(0, 0, 0, 1, 0);
	vTaskDelay(xDelay);

}

void hd44780u_4bit_write(const char * message) 
{
	const char *c = message;
	static int prev = 0;
	static unsigned int chars_limit = 0;
	unsigned int chars_written = 0;
	unsigned int q = 0;
	unsigned int n = 0;

	if (! activated ) 
		return;

	if ( xSemaphoreTake( hd44780u_mux, portMAX_DELAY ) == pdTRUE ) {

		hd44780u_4bit_return_home();

		while ( *c ) {
			switch(*c) {
				case 'a':
				case 'A':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 0, 0, 1);
				chars_written++;
				break;
				case 'b':
				case 'B':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				chars_written++;
				break;
				case 'c':
				case 'C':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				chars_written++;
				break;
				case 'd':
				case 'D':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				chars_written++;
				break;
				case 'e':
				case 'E':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				chars_written++;
				break;
				case 'f':
				case 'F':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 1, 1, 0);
				chars_written++;
				break;
				case 'g':
				case 'G':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 0, 1, 1, 1);
				chars_written++;
				break;
				case 'h':
				case 'H':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 0, 0, 0);
				chars_written++;
				break;
				case 'i':
				case 'I':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 0, 0, 1);
				chars_written++;
				break;
				case 'j':
				case 'J':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 0, 1, 0);
				chars_written++;
				break;
				case 'k':
				case 'K':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 0, 1, 1);
				chars_written++;
				break;
				case 'l':
				case 'L':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 1, 0, 0);
				chars_written++;
				break;
				case 'm':
				case 'M':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 1, 0, 1);
				chars_written++;
				break;
				case 'n':
				case 'N':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 1, 1, 0);
				chars_written++;
				break;
				case 'o':
				case 'O':
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				hd44780u_4bit_instruct(1, 1, 1, 1, 1);
				chars_written++;
				break;
				case 'p':
				case 'P':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 0, 0, 0);
				chars_written++;
				break;
				case 'q':
				case 'Q':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 0, 0, 1);
				chars_written++;
				break;
				case 'r':
				case 'R':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				chars_written++;
				break;
				case 's':
				case 'S':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				chars_written++;
				break;	
				case 't':
				case 'T':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				chars_written++;
				break;
				case 'u':
				case 'U':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				chars_written++;
				break;
				case 'v':
				case 'V':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 1, 1, 0);
				chars_written++;
				break;
				case 'w':
				case 'W':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 0, 1, 1, 1);
				chars_written++;
				break;
				case 'x':
				case 'X':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 1, 0, 0, 0);
				chars_written++;
				break;
				case 'y':
				case 'Y':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 1, 0, 0, 1);
				chars_written++;
				break;
				case 'z':
				case 'Z':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 1, 0, 1, 0);
				chars_written++;
				break;
				case '<':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 1, 1, 0, 0);
				chars_written++;
				break;
				case '.':
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				hd44780u_4bit_instruct(1, 1, 1, 1, 0);
				chars_written++;
				break;
				case '(':
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				hd44780u_4bit_instruct(1, 1, 0, 0, 0);
				chars_written++;
				break;
				case ')':
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				hd44780u_4bit_instruct(1, 1, 0, 0, 1);
				chars_written++;
				break;
				case '0':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 0, 0, 0);
				chars_written++;
				break;
				case '1':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 0, 0, 1);
				chars_written++;
				break;
				case '2':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				chars_written++;
				break;
				case '3':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				chars_written++;
				break;
				case '4':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 1, 0, 0);
				chars_written++;
				break;
				case '5':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				chars_written++;
				break;
				case '6':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 1, 1, 0);
				chars_written++;
				break;
				case '7':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 1, 1, 1);
				chars_written++;
				break;
				case '8':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 1, 0, 0, 0);
				chars_written++;
				break;
				case '9':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 1, 0, 0, 1);
				chars_written++;
				break;
				case ' ':
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				hd44780u_4bit_instruct(1, 0, 0, 0, 0);
				chars_written++;
				break;
				case '!':
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				hd44780u_4bit_instruct(1, 0, 0, 0, 1);
				chars_written++;
				break;
				case '?':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 1, 1, 1, 1);
				chars_written++;
				break;
				case '=':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 1, 1, 0, 1);
				chars_written++;
				break;
				case '-':
				hd44780u_4bit_instruct(1, 1, 0, 1, 1);
				hd44780u_4bit_instruct(1, 0, 0, 0, 0);
				chars_written++;
				break;
				case '_':
				hd44780u_4bit_instruct(1, 0, 1, 0, 1);
				hd44780u_4bit_instruct(1, 1, 1, 1, 1);
				chars_written++;
				break;
				case ',':
				hd44780u_4bit_instruct(1, 0, 0, 1, 0);
				hd44780u_4bit_instruct(1, 1, 1, 0, 0);
				chars_written++;
				break;
				case ':':
				hd44780u_4bit_instruct(1, 0, 0, 1, 1);
				hd44780u_4bit_instruct(1, 1, 0, 1, 0);
				chars_written++;
				break;
				case 165:
				case 133:
				if ( prev == 195 ) {
					hd44780u_4bit_instruct(1, 0, 1, 1, 0);
					hd44780u_4bit_instruct(1, 0, 0, 0, 1);
					chars_written++;
				}
				break;
				case 164:
				case 132:
				if ( prev == 195 ) {
					hd44780u_4bit_instruct(1, 1, 1, 1, 0);
					hd44780u_4bit_instruct(1, 0, 0, 0, 1);
					chars_written++;
				}
				break;
				case 182:
				case 150:
				if ( prev == 195 ) {
					hd44780u_4bit_instruct(1, 1, 1, 1, 0);
					hd44780u_4bit_instruct(1, 1, 1, 1, 1);
					chars_written++;
				}
				break;
				default:
				break;
			}

			prev = *c;

			vTaskDelay(xDelay);
			++c;
		}

		message_written = 1;

		q = chars_written;
		n = 0;
		while ( q < chars_limit ) {
			hd44780u_4bit_instruct(1, 0, 0, 1, 0);
			hd44780u_4bit_instruct(1, 0, 0, 0, 0);
			vTaskDelay(xDelay);
			++q;
			++n;
		}

		while ( n != 0 ) {
			hd44780u_4bit_instruct(0, 0, 0, 0, 1);
			vTaskDelay(xDelay);
			hd44780u_4bit_instruct(0, 0, 0, 0, 0);
			vTaskDelay(xDelay);
			--n;
		}

		chars_limit = chars_written;

		xSemaphoreGive(hd44780u_mux);
	}

}

void hd44780u_4bit_shift_display_left()
{
	if (! activated || !message_written )  
		return;
	if ( xSemaphoreTake( hd44780u_mux, portMAX_DELAY ) == pdTRUE ) {

		hd44780u_4bit_instruct(0, 0, 0, 0, 1);
		vTaskDelay(xDelay);
		hd44780u_4bit_instruct(0, 1, 0, 0, 0);
		vTaskDelay(xDelay);

		xSemaphoreGive(hd44780u_mux);
	}
}

void hd44780u_4bit_shift_display_right()
{
	if (! activated || !message_written)
		return;
	if ( xSemaphoreTake( hd44780u_mux, portMAX_DELAY ) == pdTRUE ) {
		hd44780u_4bit_instruct(0, 0, 0, 0, 1);
		vTaskDelay(xDelay);
		hd44780u_4bit_instruct(0, 1, 1, 0, 0);
		vTaskDelay(xDelay);

		xSemaphoreGive(hd44780u_mux);
	}
}

void hd44780u_4bit_shift_cursor_left()
{
	if (! activated || !message_written)
		return;
	if ( xSemaphoreTake( hd44780u_mux, portMAX_DELAY ) == pdTRUE ) {
		hd44780u_4bit_instruct(0, 0, 0, 0, 1);
		vTaskDelay(xDelay);
		hd44780u_4bit_instruct(0, 0, 0, 0, 0);
		vTaskDelay(xDelay);

		xSemaphoreGive(hd44780u_mux);
	}
}

void hd44780u_4bit_shift_cursor_right()
{
	if (! activated || !message_written)
		return;
	if ( xSemaphoreTake( hd44780u_mux, portMAX_DELAY ) == pdTRUE ) {
		hd44780u_4bit_instruct(0, 0, 0, 0, 1);
		vTaskDelay(xDelay);
		hd44780u_4bit_instruct(0, 0, 1, 0, 0);
		vTaskDelay(xDelay);

		xSemaphoreGive(hd44780u_mux);
	}
}

static void hd44780u_4bit_instruct(int rs, int db7, int db6, int db5, int db4)
{
	if (! activated )
		return;
	// Set E high
	gpio_gp_pin_set(
		configuration[HD44780U_E].pin.base,
		configuration[HD44780U_E].pin.pin,
		1);
	// Write 4 bit instruction
	gpio_gp_pin_set(
		configuration[HD44780U_RS].pin.base,
		configuration[HD44780U_RS].pin.pin,
		rs);
	gpio_gp_pin_set(
		configuration[HD44780U_DB4].pin.base,
		configuration[HD44780U_DB4].pin.pin,
		db4);
	gpio_gp_pin_set(
		configuration[HD44780U_DB5].pin.base,
		configuration[HD44780U_DB5].pin.pin,
		db5);
	gpio_gp_pin_set(
		configuration[HD44780U_DB6].pin.base,
		configuration[HD44780U_DB6].pin.pin,
		db6);
	gpio_gp_pin_set(
		configuration[HD44780U_DB7].pin.base,
		configuration[HD44780U_DB7].pin.pin,
		db7);
	// Set E low (internal operation start)
	gpio_gp_pin_set(
		configuration[HD44780U_E].pin.base,
		configuration[HD44780U_E].pin.pin,
		0);

}

