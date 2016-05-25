#include <util/delay.h>

#include "splc780d1.h"

// generic send
void lcd_send(uint8_t data, uint8_t type) {
	lcd_busy_wait();

	if (type) {
		lcd_set_data();
	} else {
		lcd_set_instruction();
	}

	lcd_set_write();
	lcd_set_enable_on();
	lcd_set_port_write();
	lcd_write(data);
	LCD_DELAY;
	LCD_DELAY;
	lcd_set_enable_off();
}

// generic get
uint8_t lcd_get(uint8_t type) {
	uint8_t data;

	lcd_busy_wait();
	lcd_set_port_read();
	lcd_write(0xFF);

	if (type) {
		lcd_set_data();
	} else {
		lcd_set_instruction();
	}

	lcd_set_read();
	lcd_set_enable_on();
	LCD_DELAY;
	LCD_DELAY;
	data = lcd_read();
	lcd_set_enable_off();

	return data;
}

// the protocol

// send instruction
void lcd_instruction(uint8_t data) {
	lcd_send(data, LCD_RS_INST);
}

// send data
void lcd_data(uint8_t data) {
	lcd_send(data, LCD_RS_DATA);
}

// get busy flag/address
uint8_t lcd_bf_addr(void) {
	return lcd_get(LCD_RS_INST);
}

// get curr. char
uint8_t lcd_char(void) {
	return lcd_get(LCD_RS_DATA);
}

// wait for busy flag to go away
void lcd_busy_wait(void) {
	lcd_set_instruction();
	lcd_set_port_read();
	lcd_set_read();
	lcd_set_enable_on();
	LCD_DELAY;

	while (lcd_read() & LCD_GET_BF) {
		lcd_set_enable_off();
		LCD_DELAY;
		LCD_DELAY;
		lcd_set_enable_on();
		LCD_DELAY;
		LCD_DELAY;
	}

	lcd_set_enable_off();
}
