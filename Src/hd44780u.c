#include "hd44780u.h"

hd44780u_config_t configuration[6];

void hd44780u_init_4bit_op(hd44780u_config_t *configs)
{
	hd44780u_config_t config = configs[0];
	int i = 0;

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

}

void hd44780u_4bit_write(const char * message) 
{
	const char *c = message;
	TickType_t xDelay = 100 / portTICK_PERIOD_MS;

	while ( *c ) {
		switch(*c) {
			case 'a':
			case 'A':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 0, 0, 1);
			break;
			case 'b':
			case 'B':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 0, 1, 0);
			break;
			case 'c':
			case 'C':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			break;
			case 'd':
			case 'D':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			break;
			case 'e':
			case 'E':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			break;
			case 'f':
			case 'F':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 1, 1, 0);
			break;
			case 'g':
			case 'G':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 0, 1, 1, 1);
			break;
			case 'h':
			case 'H':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 0, 0, 0);
			break;
			case 'i':
			case 'I':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 0, 0, 1);
			break;
			case 'j':
			case 'J':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 0, 1, 0);
			break;
			case 'k':
			case 'K':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 0, 1, 1);
			break;
			case 'l':
			case 'L':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 1, 0, 0);
			break;
			case 'm':
			case 'M':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 1, 1, 0, 1);
			break;
			case 'n':
			case 'N':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 1, 1, 0);
			break;
			case 'o':
			case 'O':
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			hd44780u_4bit_instruct(1, 1, 1, 1, 1);
			break;
			case 'p':
			case 'P':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 0, 0, 0);
			break;
			case 'q':
			case 'Q':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 0, 0, 1);
			break;
			case 'r':
			case 'R':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 0, 1, 0);
			break;
			case 's':
			case 'S':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			break;	
			case 't':
			case 'T':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			break;
			case 'u':
			case 'U':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			break;
			case 'v':
			case 'V':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 1, 1, 0);
			break;
			case 'w':
			case 'W':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 0, 1, 1, 1);
			break;
			case 'x':
			case 'X':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 1, 0, 0, 0);
			break;
			case 'y':
			case 'Y':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 1, 0, 0, 1);
			break;
			case 'z':
			case 'Z':
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			hd44780u_4bit_instruct(1, 1, 0, 1, 0);
			break;
			case '<':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 1, 1, 0, 0);
			break;
			case '1':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 0, 0, 1);
			break;
			case '2':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 0, 1, 0);
			break;
			case '3':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			break;
			case '4':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 1, 0, 0);
			break;
			case '5':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 1, 0, 1);
			break;
			case '6':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 1, 1, 0);
			break;
			case '7':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 0, 1, 1, 1);
			break;
			case '8':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 1, 0, 0, 0);
			break;
			case '9':
			hd44780u_4bit_instruct(1, 0, 0, 1, 1);
			hd44780u_4bit_instruct(1, 1, 0, 0, 1);
			break;
			case ' ':
			hd44780u_4bit_instruct(1, 0, 0, 1, 0);
			hd44780u_4bit_instruct(1, 0, 0, 0, 0);
			break;
			default:
			break;
		}

		vTaskDelay(xDelay);
		++c;
	}

}

void hd44780u_4bit_instruct(int rs, int db7, int db6, int db5, int db4)
{
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

