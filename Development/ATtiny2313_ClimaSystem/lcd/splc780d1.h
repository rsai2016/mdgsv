#ifndef SPLC780D1_H_
#define SPLC780D1_H_

#include <avr/io.h>

#include "../pins.h"
#include "lcd_config.h"

#define LCD_DELAY do { _delay_us(1); } while (0)

// I/O modes

#define LCD_RS_INST 0
#define LCD_RS_DATA 1

#define LCD_RW_WRITE 0
#define LCD_RW_READ  1

// Command set

// Clear Display -------------- 0b00000001
#define LCD_CLEAR_DISPLAY       0x01

// Cursor Home ---------------- 0b0000001x
#define LCD_CURSOR_HOME         0x02

// Set Entry Mode ------------- 0b000001xx
#define LCD_SET_ENTRY           0x04

#define LCD_ENTRY_DECREASE      0x00
#define LCD_ENTRY_INCREASE      0x02
#define LCD_ENTRY_NOSHIFT       0x00
#define LCD_ENTRY_SHIFT         0x01

// Set Display ---------------- 0b00001xxx
#define LCD_SET_DISPLAY         0x08

#define LCD_DISPLAY_OFF         0x00
#define LCD_DISPLAY_ON          0x04
#define LCD_CURSOR_OFF          0x00
#define LCD_CURSOR_ON           0x02
#define LCD_BLINKING_OFF        0x00
#define LCD_BLINKING_ON         0x01

// Set Shift ------------------ 0b0001xxxx
#define LCD_SET_SHIFT           0x10

#define LCD_CURSOR_MOVE         0x00
#define LCD_DISPLAY_SHIFT       0x08
#define LCD_SHIFT_LEFT          0x00
#define LCD_SHIFT_RIGHT         0x04

// Set Function --------------- 0b001xxxxx
#define LCD_SET_FUNCTION        0x20

#define LCD_FUNCTION_4BIT       0x00
#define LCD_FUNCTION_8BIT       0x10
#define LCD_FUNCTION_1LINE      0x00
#define LCD_FUNCTION_2LINE      0x08
#define LCD_FUNCTION_5X8        0x00
#define LCD_FUNCTION_5X10       0x04

// Set CG RAM Address --------- 0b01xxxxxx  (Character Generator RAM)
#define LCD_SET_CGADR           0x40

#define LCD_GC_CHAR0            0
#define LCD_GC_CHAR1            1
#define LCD_GC_CHAR2            2
#define LCD_GC_CHAR3            3
#define LCD_GC_CHAR4            4
#define LCD_GC_CHAR5            5
#define LCD_GC_CHAR6            6
#define LCD_GC_CHAR7            7

// Set DD RAM Address --------- 0b1xxxxxxx  (Display Data RAM)
#define LCD_SET_DDADR           0x80

// Get Busy Flag and Address -- 0bxyyyyyyy  (x = Busy Flag, y = Address Counter)
#define LCD_GET_BF				0x80
#define LCD_GET_ADDR			0x7F

// cmd port setup
inline static void lcd_set_instruction(void) {
	PORT(LCD_CMD_PORT) &= ~_BV(LCD_RS);
}

inline static void lcd_set_data(void) {
	PORT(LCD_CMD_PORT) |= _BV(LCD_RS);
}

inline static void lcd_set_write(void) {
	PORT(LCD_CMD_PORT) &= ~_BV(LCD_RW);
}

inline static void lcd_write(uint8_t data);

inline static void lcd_set_read(void) {
	lcd_write(0xFF);
	PORT(LCD_CMD_PORT) |= _BV(LCD_RW);
}

inline static void lcd_set_enable_on(void) {
	PORT(LCD_CMD_PORT) |= _BV(LCD_E);
}

inline static void lcd_set_enable_off(void) {
	PORT(LCD_CMD_PORT) &= ~_BV(LCD_E);
}

// data port setup
inline static void lcd_set_port_write(void) {
	DDR(LCD_DATA_PORT) = 0xFF;
}

inline static void lcd_set_port_read(void) {
	DDR(LCD_DATA_PORT) = 0x00;
}

// data port i/o
inline static void lcd_write(uint8_t data) {
	PORT(LCD_DATA_PORT) = data;
}

inline static uint8_t lcd_read(void) {
	return PIN(LCD_DATA_PORT);
}

void lcd_send(uint8_t data, uint8_t type);
uint8_t lcd_get(uint8_t type);

void lcd_instruction(uint8_t data);
void lcd_data(uint8_t data);

uint8_t lcd_bf_addr(void);
uint8_t lcd_char(void);

void lcd_busy_wait(void);

#endif /* SPLC780D1_H_ */
