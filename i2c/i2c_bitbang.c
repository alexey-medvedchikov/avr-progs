#include <stdint.h>

const uint8_t I2C_NACK = 0x0;
const uint8_t I2C_ACK = 0x1;

uint8_t i2c_set_read(const uint8_t dev_addr) { return (uint8_t)((dev_addr << 1) | 0x01); }

uint8_t i2c_set_write(const uint8_t dev_addr) { return (uint8_t)((dev_addr << 1) & ~0x01); }

void i2c_start(volatile uint8_t *const port, const uint8_t sda, const uint8_t scl) {
  *port |= sda;
  *port |= scl;
  *port &= (uint8_t)~sda;
  *port &= (uint8_t)~scl;
}

void i2c_stop(volatile uint8_t *const port, const uint8_t sda, const uint8_t scl) {
  *port &= (uint8_t)~scl;
  *port &= (uint8_t)~sda;
  *port |= scl;
  *port |= sda;
}

void i2c_send(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
              const uint8_t sda, const uint8_t scl, const uint8_t data) {
  uint8_t data_stream = data;
  for (uint8_t i = 0; i < 8; i++) {
    if (data_stream & 0b10000000) {
      *port |= sda;
    } else {
      *port &= (uint8_t)~sda;
    }
    data_stream = (uint8_t)(data_stream << 1);
    *port |= scl;
    *port &= (uint8_t)~scl;
  }
  *ddr &= (uint8_t)~sda;
  *port |= scl;
  while (*pin & sda)
    ;
  *port &= (uint8_t)~scl;
  *ddr |= sda;
}

uint8_t i2c_recv(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                 const uint8_t sda, const uint8_t scl, const uint8_t flags) {
  uint8_t data = 0;
  *ddr &= (uint8_t)~sda;
  uint8_t i = 0;
  while (1) {
    *port |= scl;
    if (*pin & sda) {
      data |= 1;
    }
    *port &= (uint8_t)~scl;
    i++;
    if (i == 8) {
      break;
    }
    data = (uint8_t)(data << 1);
  }
  *ddr |= sda;
  if (flags & I2C_ACK) {
    *port &= (uint8_t)~sda;
    *port |= scl;
    *port &= (uint8_t)~scl;
    *port |= sda;
  }
  return data;
}

void i2c_send_cmd(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                  const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t cmd) {
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_write(dev_addr));
  i2c_send(port, pin, ddr, sda, scl, cmd);
  i2c_stop(port, sda, scl);
}

void i2c_send_byte(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                   const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr,
                   const uint8_t data) {
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_write(dev_addr));
  i2c_send(port, pin, ddr, sda, scl, addr);
  i2c_send(port, pin, ddr, sda, scl, data);
  i2c_stop(port, sda, scl);
}

void i2c_send_page(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                   const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr,
                   const uint8_t data[], const uint8_t data_len) {
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_write(dev_addr));
  i2c_send(port, pin, ddr, sda, scl, addr);
  for (uint8_t i = 0; i < data_len; i++) {
    i2c_send(port, pin, ddr, sda, scl, data[i]);
  }
  i2c_stop(port, sda, scl);
}

uint8_t i2c_recv_byte(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                      const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr) {
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_write(dev_addr));
  i2c_send(port, pin, ddr, sda, scl, addr);
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_read(dev_addr));
  uint8_t result = i2c_recv(port, pin, ddr, sda, scl, I2C_NACK);
  i2c_stop(port, sda, scl);
  return result;
}

void i2c_recv_page(volatile uint8_t *const port, volatile const uint8_t *const pin, volatile uint8_t *const ddr,
                   const uint8_t sda, const uint8_t scl, const uint8_t dev_addr, const uint8_t addr, uint8_t data[],
                   const uint8_t data_len) {
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_write(dev_addr));
  i2c_send(port, pin, ddr, sda, scl, addr);
  i2c_start(port, sda, scl);
  i2c_send(port, pin, ddr, sda, scl, i2c_set_read(dev_addr));
  for (uint8_t i = 0; i < data_len - 1; i++) {
    data[i] = i2c_recv(port, pin, ddr, sda, scl, I2C_ACK);
  }
  data[data_len - 1] = i2c_recv(port, pin, ddr, sda, scl, I2C_NACK);
  i2c_stop(port, sda, scl);
}
