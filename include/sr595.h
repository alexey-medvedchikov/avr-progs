#ifndef __SR595_H__
#define __SR595_H__

#include <stdint.h>

void sr595_outb(volatile uint8_t *const port, const uint8_t ser_pin, const uint8_t srclk_pin, const uint8_t data) {
  uint8_t data_stream = data;
  for (uint8_t i = 0; i < sizeof(data) * 8; i++) {
    if (data_stream & 0x1) {
      *port |= ser_pin;
    } else {
      *port &= (uint8_t)~ser_pin;
    }
    *port |= srclk_pin;
    *port &= (uint8_t)~srclk_pin;
    data_stream >>= 1;
  }
}

void sr595_outw(volatile uint8_t *const port, const uint8_t ser_pin, const uint8_t srclk_pin, const uint16_t data) {
  uint16_t data_stream = data;
  for (uint8_t i = 0; i < sizeof(data) * 8; i++) {
    if (data_stream & 0x1) {
      *port |= ser_pin;
    } else {
      *port &= (uint8_t)~ser_pin;
    }
    *port |= srclk_pin;
    *port &= (uint8_t)~srclk_pin;
    data_stream >>= 1;
  }
}

void sr595_clear(volatile uint8_t *const port, const uint8_t srclr_pin) {
  *port &= (uint8_t)~srclr_pin;
  *port |= srclr_pin;
}

void sr595_store(volatile uint8_t *const port, const uint8_t rclk_pin) {
  *port |= rclk_pin;
  *port &= (uint8_t)~rclk_pin;
}

#endif
