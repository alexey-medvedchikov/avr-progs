#ifndef __I2C_BITBANG__
#define __I2C_BITBANG__ "i2c_bitbang.h"

#include <stdint.h>

uint8_t i2c_set_read(const uint8_t dev_addr);
uint8_t i2c_set_write(const uint8_t dev_addr);
void i2c_start(volatile uint8_t *const port, const uint8_t sda, const uint8_t scl);
void i2c_stop(volatile uint8_t *const port, const uint8_t sda, const uint8_t scl);
void i2c_send(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
              const uint8_t sda, const uint8_t scl, const uint8_t data);
uint8_t i2c_recv(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                 const uint8_t sda, const uint8_t scl, const uint8_t flags);
void i2c_send_cmd(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                  const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t cmd);
void i2c_send_byte(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                   const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr,
                   const uint8_t data);
void i2c_send_page(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                   const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr,
                   const uint8_t data[], const uint8_t data_len);
uint8_t i2c_recv_byte(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                      const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr);
void i2c_recv_page(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                   const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr, uint8_t data[],
                   const uint8_t data_len);

#endif
