/*
 ============================================================================
 Name        : main.c
 Author      : Stefan Petkov
 Version     :
 Copyright   :
 Description : main loop
 ============================================================================
 */

#include <stdint.h>

#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "pins.h"

#include "lcd/lcd_config.h"
#include "lcd/splc780d1.h"
#include "lcd/tc1604a.h"
#include "lcd/lcd.h"

#include "sensor/crc8.h"
#include "sensor/ds18x20.h"

#define DELAY 1000

#define MIN_TEMP 260
#define MAX_TEMP 280

uint8_t buffer[9];

int main(void) {
    DDRD |= _BV(PD0);
    DDRD |= _BV(PD1);

    lcd_init();

while (1) {
            DS18X20_start_meas(DS18X20_POWER_PARASITE, 0);
            _delay_ms(750);
            lcd_clear();
            lcd_home();
            int16_t d;
            if (DS18X20_read_decicelsius_single(0x28, &d) != DS18X20_OK) {
                break;
            }
            if (d <= MIN_TEMP) {
                PORTD |= _BV(PD0);
            }
            else {
                PORTD &= ~_BV(PD0);
            }
            if (d >= MAX_TEMP) {
                PORTD |= _BV(PD1);
            }
            else {
                PORTD &= ~_BV(PD1);
            }
            char temp[7];
            if (DS18X20_format_from_decicelsius(d, temp, 7) != DS18X20_OK) {
                break;
            }
            lcd_string(temp);
            lcd_data('C');
}
    return 0;
}
