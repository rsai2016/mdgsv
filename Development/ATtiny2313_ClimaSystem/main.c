/*
 ============================================================================
 Name        : main.c
 Author      : Stefan Petkov
 Version     :
 Copyright   :
 Description : main loop
 ============================================================================
 */

#ifndef TEST
#include <avr/io.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "pins.h"

#include "lcd/lcd_config.h"
#include "lcd/splc780d1.h"
#include "lcd/tc1604a.h"
#include "lcd/lcd.h"
#else
#include <stdio.h>
#include <windows.h>
#endif

#include "sensor/crc8.h"
#include "sensor/ds18x20.h"

#define DELAY 750

#ifdef TEST
void _delay_ms(int duration) {
    Sleep(duration);
}

#define HEATER_ON() puts("HEATER is ON")
#define HEATER_OFF() puts("HEATER is OFF")

#define FAN_ON() puts("FAN is ON")
#define FAN_OFF() puts("FAN is OFF")
#else
#define HEATER_ON() PORTD |= _BV(PD0)
#define HEATER_OFF() PORTD &= ~_BV(PD0)

#define FAN_ON() PORTD |= _BV(PD1)
#define FAN_OFF() PORTD &= ~_BV(PD1)

void display_print(const char* temp, int error)
{
	lcd_clear();
	lcd_home();
	if (! error) {
		lcd_string("T:");
		lcd_string(temp);
		lcd_string("C ");
	} else {
		lcd_string(temp);
	}
	lcd_setcursor(0,1);
	lcd_string("Tmin: +25.0C");
    lcd_setcursor(0,2);
    lcd_string("Tmax: +29.0C");
}

#endif

uint8_t buffer[9];

int main(void) {
char temp[7];
int16_t d;
#ifndef TEST
    DDRD |= _BV(PD0);
    DDRD |= _BV(PD1);

    lcd_init();

#endif

    while (1) {

#ifndef TEST
        DS18X20_start_meas(DS18X20_POWER_PARASITE, 0);

        _delay_ms(DELAY);

        if (DS18X20_read_decicelsius_single(0x28, &d) != DS18X20_OK) {
        	display_print("ERROR", 1);
        	continue;
        }
#else
        _delay_ms(DELAY);
        // 21.7*C
        //uint8_t sp[DS18X20_SP_SIZE] = {0x5B, 0x01, 0x4B, 0x46, 0x7F, 0xFF, 0x05, 0x10, 0xB5};
        // 25.7*C
        uint8_t sp[DS18X20_SP_SIZE] = {0x9B, 0x01, 0x4B, 0x46, 0x7F, 0xFF, 0x05, 0x10, 0x6e};

        if (crc8(sp, DS18X20_SP_SIZE)) {
            printf("CRC ERROR\n");
            continue;
        }

        d = DS18X20_raw_to_decicelsius(0x28, sp);
#endif

        if (d <= 250) {
            HEATER_ON();
        }
        else {
            HEATER_OFF();
        }

        if (d >= 290) {
            FAN_ON();
        }
        else {
            FAN_OFF();
        }
#ifndef TEST

        if (DS18X20_format_from_decicelsius(d, temp, 7) != DS18X20_OK) {
        	display_print("ERROR", 1);
            continue;
        }


        display_print(temp, 0);
#else
        if (DS18X20_format_from_decicelsius(d, temp, 7) != DS18X20_OK) {
			printf("ERROR");
			continue;
		}

        printf("%sC\n", temp);
#endif
    }

    return 0;
}
