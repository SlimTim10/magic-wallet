#include "lcd.h"
#include "hal.h"
#include <Arduino.h>

void lcd_nybble(void) {
	digitalWrite(LCD_E, LOW);	// Clock enable: falling edge
	delay(2);	// Enable pulse width >=300 ns
	digitalWrite(LCD_E, HIGH);
}

/*
  Send command
*/
void lcd_cmd(uint8_t i) {
	digitalWrite(LCD_RS, LOW);	// RS=LOW : Send instruction
	digitalWrite(LCD_RW, LOW);	// R/W=LOW : Write
	// Put upper 4 bits on output port
	if (i & 0x10) digitalWrite(LCD_DB4, HIGH); else digitalWrite(LCD_DB4, LOW);
	if (i & 0x20) digitalWrite(LCD_DB5, HIGH); else digitalWrite(LCD_DB5, LOW);
	if (i & 0x40) digitalWrite(LCD_DB6, HIGH); else digitalWrite(LCD_DB6, LOW);
	if (i & 0x80) digitalWrite(LCD_DB7, HIGH); else digitalWrite(LCD_DB7, LOW);
	lcd_nybble();	// Send upper 4 bits
	// Put lower 4 bits on output port
	if (i & 0x01) digitalWrite(LCD_DB4, HIGH); else digitalWrite(LCD_DB4, LOW);
	if (i & 0x02) digitalWrite(LCD_DB5, HIGH); else digitalWrite(LCD_DB5, LOW);
	if (i & 0x04) digitalWrite(LCD_DB6, HIGH); else digitalWrite(LCD_DB6, LOW);
	if (i & 0x08) digitalWrite(LCD_DB7, HIGH); else digitalWrite(LCD_DB7, LOW);
	lcd_nybble();	// Send lower 4 bits
}

/*
  Send data
*/
void lcd_data(uint8_t i) {
	digitalWrite(LCD_RS, HIGH);	// RS=LOW : Send data
	digitalWrite(LCD_RW, LOW);	// R/W=LOW : Write
	// Put upper 4 bits on output port
	if (i & 0x10) digitalWrite(LCD_DB4, HIGH); else digitalWrite(LCD_DB4, LOW);
	if (i & 0x20) digitalWrite(LCD_DB5, HIGH); else digitalWrite(LCD_DB5, LOW);
	if (i & 0x40) digitalWrite(LCD_DB6, HIGH); else digitalWrite(LCD_DB6, LOW);
	if (i & 0x80) digitalWrite(LCD_DB7, HIGH); else digitalWrite(LCD_DB7, LOW);
	lcd_nybble();	// Send upper 4 bits
	// Put lower 4 bits on output port
	if (i & 0x01) digitalWrite(LCD_DB4, HIGH); else digitalWrite(LCD_DB4, LOW);
	if (i & 0x02) digitalWrite(LCD_DB5, HIGH); else digitalWrite(LCD_DB5, LOW);
	if (i & 0x04) digitalWrite(LCD_DB6, HIGH); else digitalWrite(LCD_DB6, LOW);
	if (i & 0x08) digitalWrite(LCD_DB7, HIGH); else digitalWrite(LCD_DB7, LOW);
	lcd_nybble();	// Send lower 4 bits
}

/*
  Initialization
*/
void lcd_init(void) {
	// Setup pins
	pinMode(LCD_RS, OUTPUT);
	pinMode(LCD_RW, OUTPUT);
	pinMode(LCD_E, OUTPUT);
	pinMode(LCD_DB4, OUTPUT);
	pinMode(LCD_DB5, OUTPUT);
	pinMode(LCD_DB6, OUTPUT);
	pinMode(LCD_DB7, OUTPUT);
	// Clear output port
	digitalWrite(LCD_DB4, LOW);
	digitalWrite(LCD_DB5, LOW);
	digitalWrite(LCD_DB6, LOW);
	digitalWrite(LCD_DB7, LOW);
	delay(15);	// Wait >15 ms after power is applied
	// Put 0x30 on the output port
	digitalWrite(LCD_DB4, HIGH);
	digitalWrite(LCD_DB5, HIGH);
	digitalWrite(LCD_DB6, LOW);
	digitalWrite(LCD_DB7, LOW);
	delay(5);	// Must wait 5 ms
	lcd_nybble();	// Command 0x30 = Wake up
	delay(1);	// Must wait 160 us
	lcd_nybble();	// Wake up #2
	delay(1);	// Must wait 160 us
	lcd_nybble();	// Wake up #3
	delay(1);	// Must wait 160 us
	// Put 0x20 on the output port
	digitalWrite(LCD_DB4, LOW);
	digitalWrite(LCD_DB5, HIGH);
	digitalWrite(LCD_DB6, LOW);
	digitalWrite(LCD_DB7, LOW);
	lcd_nybble();	// Function set: 4 bit interface
	lcd_cmd(0x28);	// Function set: 4-bit/2-line
	lcd_cmd(0x10);	// Set cursor
	lcd_cmd(0x0F);	// Display ON; Blinking cursor
	lcd_cmd(0x06);	// Entry mode set
	lcd_cmd(0x01);	// Clear display
}

/* 
  Set the cursor to the beginning of line 1 or 2
*/
void lcd_set_line(uint8_t line) {
	if (line == 1) {
		lcd_cmd(0x80);	// Set DDRAM address to 00
	} else {
		lcd_cmd(0xC0);	// Set DDRAM address to 40
	}
}

/*
  Clear display and return home
*/
void lcd_clear(void) {
	lcd_cmd(0x01);	// Clear display
}

/*
  Clear line 1
*/
void lcd_clear1(void) {
	lcd_set_line(1);
	uint8_t i;
	// Clear line 1
	for (i = 0; i < 16; i++) {
		lcd_data(0x20);
	}
}

/*
  Clear line 2
*/
void lcd_clear2(void) {
	lcd_set_line(2);
	uint8_t i;
	// Clear line 2
	for (i = 0; i < 16; i++) {
		lcd_data(0x20);
	}
}

/*
  Write to the display
 */
void lcd_print(const char *str) {
	uint8_t i;
	for (i = 0; i < LCD_WIDTH && *str; i++) {
		lcd_data(*str++);
	}
}

/*
  Clear display and write to it, wrapping to line 2 if overflow
*/
void lcd_write(const char *str) {
	lcd_clear();
	uint8_t i;
	// Write string to line 1
	for (i = 0; i < 16 && *str; i++) {
		lcd_data(*str++);
	}
	if (*str) {	// Overflow
		lcd_set_line(2);
		// Write string to line 2
		for (i = 0; i < 16 && *str; i++) {
			lcd_data(*str++);
		}
	}
}

/*
  Clear line 1 and write to it
*/
void lcd_write1(const char *str) {
	lcd_set_line(1);
	uint8_t i;
	// Clear line 1
	for (i = 0; i < 16; i++) {
		lcd_data(0x20);
	}
	lcd_set_line(1);
	// Write string to line 1
	for (i = 0; i < 16 && *str; i++) {
		lcd_data(*str++);
	}
}

/*
  Clear line 2 and write to it
*/
void lcd_write2(const char *str) {
	lcd_set_line(2);
	uint8_t i;
	// Clear line 2
	for (i = 0; i < 16; i++) {
		lcd_data(0x20);
	}
	lcd_set_line(2);
	// Write string to line 2
	for (i = 0; i < 16 && *str; i++) {
		lcd_data(*str++);
	}
}

/*
  Clear line 1 and write an int to it
*/
void lcd_write1_int(int n) {
	lcd_set_line(1);
	uint8_t i;
	// Clear line 1
	for (i = 0; i < 16; i++) {
		lcd_data(0x20);
	}
	lcd_set_line(1);
	// Write int as string to line 1
	char str[16];
	snprintf(str, 16, "%d", n);
	for (i = 0; i < 16 && str[i]; i++) {
		lcd_data(str[i]);
	}
}

/*
  Clear line 2 and write an int to it
*/
void lcd_write2_int(int n) {
	lcd_set_line(2);
	uint8_t i;
	// Clear line 1
	for (i = 0; i < 16; i++) {
		lcd_data(0x20);
	}
	lcd_set_line(2);
	// Write int as string to line 1
	char str[16];
	snprintf(str, 16, "%d", n);
	for (i = 0; i < 16 && str[i]; i++) {
		lcd_data(str[i]);
	}
}

/*
  Turn off cursor
*/
void lcd_cursor_off(void) {
	lcd_cmd(0x0C);
}

/*
  Turn on cursor blinking
*/
void lcd_cursor_on(void) {
	lcd_cmd(0x0D);
}
