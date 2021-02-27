#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "easy_defs.h"
#include "i2c_bitbang.h"
#include "mcp7940m.h"
#include "pcf8574lcd.h"

volatile uint8_t *const I2C_PORT = &PORTB;
volatile uint8_t *const I2C_PORT_DDR = &DDRB;
const volatile uint8_t *const I2C_PIN = &PINB;

const uint8_t SDA = 1 << PORTB0;
const uint8_t SCL = 1 << PORTB1;

volatile uint8_t *const SW_PORT = &PORTB;
volatile uint8_t *const SW_PORT_DDR = &DDRB;
const volatile uint8_t *const SW_PIN = &PINB;

const uint8_t SW_SET = 1 << PORTB2;
const uint8_t SW_MOVE = 1 << PORTB3;
const uint8_t SW_UP = 1 << PORTB4;
const uint8_t SW_BACKLIGHT = 1 << PORTB4;

const uint8_t DISPLAY_COLS = 20;

uint8_t _mcp7940m_read_cb(const uint8_t addr) {
  return i2c_recv_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, mcp7940m_dev_addr, addr);
}

void _mcp7940m_write_cb(const uint8_t addr, const uint8_t data) {
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, mcp7940m_dev_addr, addr, data);
}

void _pcf8574lcd_write_cb(const uint8_t data) {
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, pcf8574lcd_dev_addr, data);
}

typedef struct {
  uint8_t sec;
  uint8_t min;
  uint8_t hour;
  uint8_t wday;
  uint8_t mday;
  uint8_t mon;
  uint8_t year;
} datetime;

volatile struct {
  datetime ts;
  uint8_t set_mode;
  uint8_t cursor_pos;
  uint8_t backlight;
} state = {
    .set_mode = 0,
    .cursor_pos = 0,
    .backlight = PCF8574LCD_BACKLIGHT,
};

static uint8_t cursor_pos_map[] = {1, 4, 7, 10, 13, 16};

void append_2digits(const uint8_t x, char *dst) {
  dst[0] = '0' + x / 10;
  dst[1] = '0' + x % 10;
}

ISR(WDT_vect) {
  mcp7940m_sec_reg sec = mcp7940m_sec_reg_read(_mcp7940m_read_cb);
  state.ts.sec = sec.fields.tens * 10 + sec.fields.ones;
  mcp7940m_min_reg min = mcp7940m_min_reg_read(_mcp7940m_read_cb);
  state.ts.min = min.fields.tens * 10 + min.fields.ones;
  mcp7940m_hour_reg hour = mcp7940m_hour_reg_read(_mcp7940m_read_cb);
  state.ts.hour = hour.hour24.tens * 10 + hour.hour24.ones;
  mcp7940m_day_reg day = mcp7940m_day_reg_read(_mcp7940m_read_cb);
  state.ts.mday = day.fields.tens * 10 + day.fields.ones;
  mcp7940m_month_reg mon = mcp7940m_month_reg_read(_mcp7940m_read_cb);
  state.ts.mon = mon.fields.tens * 10 + mon.fields.ones;
  mcp7940m_year_reg year = mcp7940m_year_reg_read(_mcp7940m_read_cb);
  state.ts.year = year.fields.tens * 10 + year.fields.ones;

  //                          01234567890123456
  static char time_str[18] = "00:00:00 00.00.00";

  append_2digits(state.ts.hour, time_str);
  append_2digits(state.ts.min, time_str + 3);
  append_2digits(state.ts.sec, time_str + 6);
  append_2digits(state.ts.year, time_str + 9);
  append_2digits(state.ts.mon, time_str + 12);
  append_2digits(state.ts.mday, time_str + 15);

  pcf8574lcd_clear_display(_pcf8574lcd_write_cb, state.backlight);
  pcf8574lcd_set_cursor(_pcf8574lcd_write_cb, DISPLAY_COLS, 0, 0, state.backlight);
  pcf8574lcd_write_str(_pcf8574lcd_write_cb, time_str, state.backlight);
  const uint8_t display_control =
      (state.set_mode ? PCF8574LCD_DISPLAY_CONTROL_BLINK | PCF8574LCD_DISPLAY_CONTROL_CURSOR : 0);
  pcf8574lcd_display_control(_pcf8574lcd_write_cb, display_control | PCF8574LCD_DISPLAY_CONTROL_ON, state.backlight);

  pcf8574lcd_set_cursor(_pcf8574lcd_write_cb, DISPLAY_COLS, cursor_pos_map[state.cursor_pos], 0, state.backlight);

  // Enable interrupt from button
  GIMSK |= bitval(PCIE);
}

uint8_t move_cursor(const uint8_t pos) {
  uint8_t new_pos = pos + 1;
  if (new_pos >= sizeof(cursor_pos_map) / sizeof(cursor_pos_map[0])) {
    new_pos -= sizeof(cursor_pos_map);
  }

  return new_pos;
}

void change_hour() {
  state.ts.hour++;
  if (state.ts.hour >= 24) {
    state.ts.hour -= 24;
  }
  const mcp7940m_hour_reg hour = {
      .hour_mode.hour12 = 0,
      .hour24.tens = state.ts.hour / 10,
      .hour24.ones = state.ts.hour % 10,
  };
  mcp7940m_hour_reg_write(_mcp7940m_write_cb, hour);
}

void change_min() {
  state.ts.min++;
  if (state.ts.min >= 60) {
    state.ts.min -= 60;
  }
  const mcp7940m_min_reg min = {
      .fields.tens = state.ts.min / 10,
      .fields.ones = state.ts.min % 10,
  };
  mcp7940m_min_reg_write(_mcp7940m_write_cb, min);
}

void change_sec() {
  state.ts.sec++;
  if (state.ts.sec >= 60) {
    state.ts.sec -= 60;
  }
  const mcp7940m_sec_reg sec = {
      .fields.tens = state.ts.sec / 10,
      .fields.ones = state.ts.sec % 10,
  };
  mcp7940m_sec_reg_write(_mcp7940m_write_cb, sec);
}

void change_mday() {
  const uint8_t day_in_mon_map[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
  uint8_t day_in_mon = day_in_mon_map[state.ts.mon];
  if ((state.ts.mon == 1) && (state.ts.year % 4 == 0) && (state.ts.year != 0)) {
    day_in_mon = 29;
  }
  state.ts.mday++;
  if (state.ts.mday >= day_in_mon) {
    state.ts.mday -= day_in_mon;
  }
  const mcp7940m_day_reg mday = {
      .fields.tens = state.ts.mday / 10,
      .fields.ones = state.ts.mday % 10,
  };
  mcp7940m_day_reg_write(_mcp7940m_write_cb, mday);
}

void change_mon() {
  state.ts.mon++;
  if (state.ts.mon >= 12) {
    state.ts.mon -= 12;
  }
  const mcp7940m_month_reg mon = {
      .fields.tens = state.ts.mon / 10,
      .fields.ones = state.ts.mon % 10,
  };
  mcp7940m_month_reg_write(_mcp7940m_write_cb, mon);
}

void change_year() {
  state.ts.year++;
  if (state.ts.year >= 100) {
    state.ts.year -= 100;
  }
  const mcp7940m_year_reg year = {
      .fields.tens = state.ts.year / 10,
      .fields.ones = state.ts.year % 10,
  };
  mcp7940m_year_reg_write(_mcp7940m_write_cb, year);
}

void change_time() {
  static void (*func_map[])() = {change_hour, change_min, change_sec, change_mday, change_mon, change_year};
  func_map[state.cursor_pos]();
}

ISR(PCINT0_vect) {
  // Disable interrupt from buttons to avoid jitter
  GIMSK &= (uint8_t)~bitval(PCIE);

  if (*SW_PIN & SW_SET) {
    state.set_mode = !state.set_mode;
    state.cursor_pos = 0;
  } else if (*SW_PIN & SW_MOVE) {
    state.cursor_pos = move_cursor(state.cursor_pos);
  } else if ((*SW_PIN & SW_UP) && state.set_mode) {
    change_time();
  } else if ((*SW_PIN & SW_BACKLIGHT) && !state.set_mode) {
    if (state.backlight == PCF8574LCD_BACKLIGHT) {
      state.backlight = PCF8574LCD_NOBACKLIGHT;
    } else {
      state.backlight = PCF8574LCD_BACKLIGHT;
    }
  }
}

int main() {
  *I2C_PORT |= SDA | SCL;
  *I2C_PORT_DDR |= SDA | SCL;

  *SW_PORT_DDR &= (uint8_t) ~(SW_SET | SW_MOVE | SW_UP | SW_BACKLIGHT);
  *SW_PORT &= (uint8_t) ~(SW_SET | SW_MOVE | SW_UP | SW_BACKLIGHT);

  pcf8574lcd_init(_pcf8574lcd_write_cb);
  pcf8574lcd_func_set(_pcf8574lcd_write_cb, PCF8574LCD_FUNC_SET_2LINE | PCF8574LCD_FUNC_SET_5X8FONT, state.backlight);
  pcf8574lcd_entry_mode_set(_pcf8574lcd_write_cb, PCF8574LCD_ENTRY_MODE_SET_RIGHT | PCF8574LCD_ENTRY_MODE_SET_NOSHIFT,
                            state.backlight);
  pcf8574lcd_display_control(_pcf8574lcd_write_cb, PCF8574LCD_DISPLAY_CONTROL_ON, state.backlight);

  mcp7940m_sec_reg_write(_mcp7940m_write_cb, (const mcp7940m_sec_reg){
                                                 .fields.start_oscillator = 1,
                                             });

  WDTCR = bitval(WDIE) | WDT_PRESCALE_250MS;
  GIMSK |= bitval(PCIE);
  PCMSK = bitval(PCINT2) | bitval(PCINT3) | bitval(PCINT4);

  power_adc_disable();
  power_timer1_disable();
  power_usi_disable();
  set_sleep_mode(SLEEP_MODE_IDLE);
  cli();
  while (1) {
    sleep_enable();
    sleep_bod_disable();
    sei();
    sleep_cpu();
    sleep_disable();
  }
  sei();
}
