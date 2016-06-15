#ifndef LCD_H
#define LCD_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {	/* Use C library with Arduino */
#endif

	enum lcd_info {
		LCD_WIDTH = 8,
		LCD_HEIGHT = 1,
		LCD_MAX_CHARS = LCD_WIDTH * LCD_HEIGHT,
	};
	
	void lcd_nybble(void);
	void lcd_cmd(uint8_t);
	void lcd_data(uint8_t);
	void lcd_init(void);
	void lcd_set_line(uint8_t line);
	void lcd_clear(void);
	void lcd_clear1(void);
	void lcd_clear2(void);
	void lcd_print(const char *);
	void lcd_write(const char *);
	void lcd_write1(const char *);
	void lcd_write2(const char *);
	void lcd_write1_int(int);
	void lcd_write2_int(int);
	void lcd_cursor_off(void);
	void lcd_cursor_on(void);

#define lcd_printf(...)	({						\
			char tmpstr[LCD_MAX_CHARS];			\
			sprintf(tmpstr, __VA_ARGS__);		\
			lcd_print(tmpstr);					\
		})

#ifdef __cplusplus
}
#endif

#endif
