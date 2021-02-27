#ifndef __PCF8574LCD_H__
#define __PCF8574LCD_H__ "pcf8574lcd.h"

#include <stdint.h>
#include <util/delay.h>

const uint8_t pcf8574lcd_dev_addr = 0x27;

#define PCF8574LCD_REG_DATA 0x01
#define PCF8574LCD_READ 0x02
#define PCF8574LCD_ENABLE 0x04
#define PCF8574LCD_BACKLIGHT 0x08
#define PCF8574LCD_NOBACKLIGHT 0x00

#define PCF8574LCD_ENTRY_MODE_SET_LEFT 0x00
#define PCF8574LCD_ENTRY_MODE_SET_RIGHT 0x02
#define PCF8574LCD_ENTRY_MODE_SET_NOSHIFT 0x00
#define PCF8574LCD_ENTRY_MODE_SET_SHIFT 0x01

#define PCF8574LCD_DISPLAY_CONTROL_OFF 0x00
#define PCF8574LCD_DISPLAY_CONTROL_ON 0x04
#define PCF8574LCD_DISPLAY_CONTROL_NOCURSOR 0x00
#define PCF8574LCD_DISPLAY_CONTROL_CURSOR 0x02
#define PCF8574LCD_DISPLAY_CONTROL_NOBLINK 0x00
#define PCF8574LCD_DISPLAY_CONTROL_BLINK 0x02

#define PCF8574LCD_SHIFT_CURSOR 0x00
#define PCF8574LCD_SHIFT_DISPLAY 0x08
#define PCF8574LCD_SHIFT_LEFT 0x00
#define PCF8574LCD_SHIFT_RIGHT 0x04

#define PCF8574LCD_FUNC_SET_1LINE 0x00
#define PCF8574LCD_FUNC_SET_2LINE 0x08
#define PCF8574LCD_FUNC_SET_5X8FONT 0x00
#define PCF8574LCD_FUNC_SET_5X10FONT 0x04

typedef void (*pcf8574lcd_write_cb)(const uint8_t data);

void pcf8574lcd_i2c_write_nibble(const pcf8574lcd_write_cb i2c_write, const uint8_t nibble, const uint8_t flags) {
  i2c_write((uint8_t)(nibble << 4) | flags | PCF8574LCD_ENABLE);
  i2c_write((uint8_t)(nibble << 4) | flags);
}

void pcf8574lcd_i2c_write(const pcf8574lcd_write_cb i2c_write, const uint8_t data, const uint8_t flags) {
  pcf8574lcd_i2c_write_nibble(i2c_write, data >> 4, flags);
  pcf8574lcd_i2c_write_nibble(i2c_write, data, flags);
}

void pcf8574lcd_clear_display(const pcf8574lcd_write_cb i2c_write, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, 0x01, flags);
  _delay_ms(3);
}

void pcf8574lcd_return_home(const pcf8574lcd_write_cb i2c_write, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, 0x02, flags);
  _delay_ms(3);
}

void pcf8574lcd_entry_mode_set(const pcf8574lcd_write_cb i2c_write, const uint8_t mode, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, 0x04 | mode, flags);
}

void pcf8574lcd_display_control(const pcf8574lcd_write_cb i2c_write, const uint8_t mode, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, 0x08 | mode, flags);
}

void pcf8574lcd_shift(const pcf8574lcd_write_cb i2c_write, const uint8_t mode, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, 0x10 | mode, flags);
}

void pcf8574lcd_func_set(const pcf8574lcd_write_cb i2c_write, const uint8_t mode, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, 0x20 | mode, flags);
}

void pcf8574lcd_write_char(const pcf8574lcd_write_cb i2c_write, const char ch, const uint8_t flags) {
  pcf8574lcd_i2c_write(i2c_write, (uint8_t)ch, PCF8574LCD_REG_DATA | flags);
}

void pcf8574lcd_write_str(const pcf8574lcd_write_cb i2c_write, const char *s, const uint8_t flags) {
  for (uint8_t i = 0; s[i] != '\0'; i++) {
    pcf8574lcd_write_char(i2c_write, s[i], flags);
  }
}

void pcf8574lcd_set_cursor(const pcf8574lcd_write_cb i2c_write, const uint8_t display_cols, const uint8_t col,
                           const uint8_t row, const uint8_t flags) {
  uint8_t base_addr = 0x00;
  switch (row) {
  case 1:
    base_addr = 0x40;
    break;
  case 2:
    base_addr = (uint8_t)(0x00 + display_cols);
    break;
  }
  uint8_t nibble = (uint8_t)(0x80 | (base_addr + col));
  pcf8574lcd_i2c_write(i2c_write, nibble, flags);
}

void pcf8574lcd_init(const pcf8574lcd_write_cb i2c_write) {
  _delay_ms(20);
  pcf8574lcd_i2c_write_nibble(i2c_write, 0x03, 0x00);
  _delay_ms(5);
  pcf8574lcd_i2c_write_nibble(i2c_write, 0x03, 0x00);
  _delay_us(150);
  pcf8574lcd_i2c_write_nibble(i2c_write, 0x03, 0x00);
  pcf8574lcd_i2c_write_nibble(i2c_write, 0x02, 0x00);
}

#endif
