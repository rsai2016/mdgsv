#ifndef TC1604A_H_
#define TC1604A_H_

#define LCD_PROGRESS_PIXELS_PER_CHAR 6

void lcd_init(void);
void lcd_setcursor(uint8_t x, uint8_t y);
void lcd_check_eol(void);

#endif /* TC1604A_H_ */
