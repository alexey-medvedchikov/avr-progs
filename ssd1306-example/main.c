#define __DELAY_ROUND_CLOSEST__
#define F_CPU 8000000

#ifndef __AVR_ATtiny85__
#define __AVR_ATtiny85__
#endif

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
#include "ssd1306.h"

volatile uint8_t *const I2C_PORT = &PORTB;
volatile uint8_t *const I2C_PORT_DDR = &DDRB;
const volatile uint8_t *const I2C_PIN = &PINB;

const uint8_t SDA = 1 << PORTB0;
const uint8_t SCL = 1 << PORTB1;

int main() {
  *I2C_PORT |= SDA | SCL;
  *I2C_PORT_DDR |= SDA | SCL;

  _delay_ms(5);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xae);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xd5, 0x80);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xa8, 0x3f);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xd3, 0x00);
  _delay_ms(1);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0x40);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0x8d, 0x10);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xd8, 0x05);
  _delay_ms(1);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xa1);
  _delay_ms(1);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xc8);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xda, 0x12);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0x81, 0x8f);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xd9, 0x22);
  _delay_ms(1);
  i2c_send_byte(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xdb, 0x40);
  _delay_ms(1);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xa4);
  _delay_ms(1);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xa6);
  _delay_ms(1);
  i2c_send_cmd(I2C_PORT, I2C_PIN, I2C_PORT_DDR, SDA, SCL, ssd1306_dev_addr, 0xaf);

  while (1) {
  }
}
