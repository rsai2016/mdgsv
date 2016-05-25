#include <avr/pgmspace.h>
#include <util/delay.h>

#include "lcd_config.h"
#include "splc780d1.h"
#include "lcd.h"

// custom LCD characters

const unsigned char lcd_custom_char[] PROGMEM = {
        0, 0x3, 0x7, 0xf, 0x1f, 0x1f, 0x1e, 0x1c, // stage 1 char 0
        0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // stage 1 char 1
        0x0, 0x0, 0x0, 0x0, 0x1, 0x1, 0x1, 0x3, // stage 1 char 2
        0x7, 0xf, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, // stage 1 char 3
        0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, // stage 1 char 4
        0x7, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, // stage 1 char 5
        0x1e, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x18, // stage 1 char 6
        0x0, 0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, // stage 1 char 7

        0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 0x1, 0x1, // stage 2 char 0
        0x1c, 0x1c, 0x1c, 0x1e, 0x1e, 0x1f, 0x1f, 0x1f, // stage 2 char 1
        0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, // stage 2 char 2
        0x1f, 0x1f, 0x1f, 0xf, 0x7, 0x7, 0x3, 0x0, // stage 2 char 3
        0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x18, // stage 2 char 4
        0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1, // stage 2 char 5
        0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x1e, 0x1c, 0x18, // stage 2 char 6
        0x10, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 // stage 2 char 7
};

const unsigned char lcd_ddaddr_line[] PROGMEM = {0x00, 0x40, 0x10, 0x50};

void lcd_init(void) {
	// zero the cmd port
	lcd_set_instruction();
	lcd_set_write();
	lcd_set_enable_off();

	// set it to write only
	DDR(LCD_CMD_PORT) |= _BV(LCD_RS_DIR);
	DDR(LCD_CMD_PORT) |= _BV(LCD_RW_DIR);
	DDR(LCD_CMD_PORT) |= _BV(LCD_E_DIR);

	lcd_write(0);
	lcd_set_port_write();

	_delay_ms(15);

	// reset 1
	lcd_write(LCD_SET_FUNCTION | LCD_FUNCTION_8BIT);
	lcd_set_enable_on();
	LCD_DELAY;
	LCD_DELAY;
	lcd_set_enable_off();
	_delay_ms(5);

	// reset 2
	lcd_set_enable_on();
	LCD_DELAY;
	LCD_DELAY;
	lcd_set_enable_off();
	_delay_ms(1);

	// reset 3
	lcd_set_enable_on();
	LCD_DELAY;
	LCD_DELAY;
	lcd_set_enable_off();
	_delay_ms(1);

	// set mode
	lcd_instruction(
			LCD_SET_FUNCTION | LCD_FUNCTION_8BIT | LCD_FUNCTION_2LINE
					| LCD_FUNCTION_5X8);

	// Display off, cursor off, blinking off
	lcd_instruction(
			LCD_SET_DISPLAY | LCD_DISPLAY_OFF | LCD_CURSOR_OFF
					| LCD_BLINKING_OFF);

	lcd_clear();

	// turn on the lcd
	lcd_instruction(
			LCD_SET_DISPLAY | LCD_DISPLAY_ON | LCD_CURSOR_OFF
					| LCD_BLINKING_OFF);

	// Increasing mode, no shift
	lcd_instruction(LCD_SET_ENTRY | LCD_ENTRY_INCREASE | LCD_ENTRY_NOSHIFT);
}

void lcd_setcursor(uint8_t x, uint8_t y) {
	if (y > 4 || x > 16) {
		return;
	}

	lcd_instruction(LCD_SET_DDADR | (pgm_read_byte(lcd_ddaddr_line + y) + x));
}

void lcd_check_eol(void) {
	return;
	uint8_t addr = lcd_bf_addr() & LCD_GET_ADDR;

	if ((addr & 0xF) != 0xF) {
		return;
	}

	uint8_t y = 0;

	switch (addr) {
	case 0x10:
		y = 1;
		break;
	case 0x50:
		y = 2;
		break;
	case 0x20:
		y = 3;
		break;
	default:
		y = 0;
		break;
	}

	lcd_setcursor(0, y);
}
