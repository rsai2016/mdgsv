#ifndef LCD_H_
#define LCD_H_

void lcd_clear(void);
void lcd_home(void);
void lcd_print_hexbyte(uint8_t i);
void lcd_string(const char* data);
void lcd_string_P(const char* data);
void lcd_generatechar(uint8_t code, const uint8_t* data);
void lcd_progressbar(uint16_t progress, uint16_t max_progress, uint8_t length);

#endif /* LCD_H_ */
