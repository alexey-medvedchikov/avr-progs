#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stdint.h>

const uint8_t ssd1306_dev_addr = 0x3c;

typedef void (*ssd1306_write_cb)(const uint8_t data);

#endif
