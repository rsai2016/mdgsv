#include <util/delay.h>
#include <avr/pgmspace.h>

#include "splc780d1.h"
#include "tc1604a.h"

extern const unsigned char lcd_custom_char[];

void lcd_clear(void)
{
    lcd_instruction(LCD_CLEAR_DISPLAY);
    _delay_ms(2);
}

void lcd_home(void)
{
    lcd_instruction(LCD_CURSOR_HOME);
    _delay_ms(2);
}

void lcd_print_hexbyte(uint8_t i)
{
    uint8_t h, l;

    h = '0' + ((i >> 4) & 0x0F); // High nibble

    if (h > '9')
        h += 7;

    l = '0' + (i & 0x0F); // Low nibble

    if (l > '9')
        l += 7;

    lcd_data(h);
    lcd_data(l);
}

void lcd_string(const char* data)
{
    while (*data)
    {
        lcd_check_eol();
        lcd_data(*data++);
    }
}

void lcd_string_P(const char* data)
{
    while (pgm_read_byte(data))
    {
        lcd_check_eol();
        lcd_data(pgm_read_byte(data++));
    }
}

void lcd_generatechar(uint8_t code, const uint8_t* data)
{
    uint8_t i;

    lcd_instruction(LCD_SET_CGADR | (code * 8));

    for (i = 0; i < 8; ++i)
    {
        lcd_data(pgm_read_byte(data + i));
    }
}

void lcd_progress_bar(uint16_t progress, uint16_t max_progress, uint8_t length)
{
    uint8_t i, c;
    uint32_t pixel_progress;

    for (i = 0; i < 6; ++i)
    {
        lcd_generatechar(i, lcd_custom_char + i * 8);
    }

    pixel_progress = ((progress * (length * LCD_PROGRESS_PIXELS_PER_CHAR))
            / max_progress);

    for (i = 0; i < length; i++)
    {
        if (((i * (uint16_t) LCD_PROGRESS_PIXELS_PER_CHAR) + 5)
                > pixel_progress)
        {
            if (((i * (uint16_t) LCD_PROGRESS_PIXELS_PER_CHAR))
                    > pixel_progress)
            {
                c = 0;
            }
            else
            {
                c = pixel_progress % LCD_PROGRESS_PIXELS_PER_CHAR;
            }
        }
        else
        {
            c = 5;
        }

        lcd_data(c);
    }

}
