#ifndef __MCP7940M_H__
#define __MCP7940M_H__ "mcp7940m.h"

#include <stdint.h>

typedef void (*mcp7940m_write_cb)(const uint8_t addr, const uint8_t data);
typedef uint8_t (*mcp7940m_read_cb)(const uint8_t addr);

const uint8_t mcp7940m_dev_addr = 0x6f;

const uint8_t mcp7940m_sec_reg_addr = 0x00;
const uint8_t mcp7940m_min_reg_addr = 0x01;
const uint8_t mcp7940m_hour_reg_addr = 0x02;
const uint8_t mcp7940m_weekday_reg_addr = 0x03;
const uint8_t mcp7940m_day_reg_addr = 0x04;
const uint8_t mcp7940m_month_reg_addr = 0x05;
const uint8_t mcp7940m_year_reg_addr = 0x06;
const uint8_t mcp7940m_control_reg_addr = 0x07;
const uint8_t mcp7940m_osctrim_reg_addr = 0x08;
// 0x09 is reserved, do not use
const uint8_t mcp7940m_alm0sec_reg_addr = 0x0a;
const uint8_t mcp7940m_alm0min_reg_addr = 0x0b;
const uint8_t mcp7940m_alm0hour_reg_addr = 0x0c;
const uint8_t mcp7940m_alm0weekday_reg_addr = 0x0d;
const uint8_t mcp7940m_alm0day_reg_addr = 0x0e;
const uint8_t mcp7940m_alm0month_reg_addr = 0x0f;
// 0x10 is reserved, do not use
const uint8_t mcp7940m_alm1sec_reg_addr = 0x11;
const uint8_t mcp7940m_alm1min_reg_addr = 0x12;
const uint8_t mcp7940m_alm1hour_reg_addr = 0x13;
const uint8_t mcp7940m_alm1weekday_reg_addr = 0x14;
const uint8_t mcp7940m_alm1day_reg_addr = 0x15;
const uint8_t mcp7940m_alm1month_reg_addr = 0x16;
// 0x17 - 0x1f are reserved, do not use

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 3;
    uint8_t start_oscillator : 1;
  } fields;
  uint8_t bits;
} mcp7940m_sec_reg;

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 3;
    uint8_t _reserved : 1;
  } fields;
  uint8_t bits;
} mcp7940m_min_reg;

typedef struct {
  uint8_t ones : 4;
  uint8_t tens : 2;
  uint8_t _reserved : 2;
} mcp7940m_hour_24_reg;

typedef struct {
  uint8_t ones : 4;
  uint8_t tens : 1;
  uint8_t pm : 1;
  uint8_t _reserved : 2;
} mcp7940m_hour_12_reg;

typedef union {
  struct {
    uint8_t _reserved1 : 6;
    uint8_t hour12 : 1;
    uint8_t _reserved2 : 1;
  } hour_mode;
  mcp7940m_hour_12_reg hour12;
  mcp7940m_hour_24_reg hour24;
  uint8_t bits;
} mcp7940m_hour_reg;

typedef union {
  struct {
    uint8_t weekday : 3;
    uint8_t _reserved1 : 2;
    uint8_t oscillator_running : 1;
    uint8_t _reserved2 : 2;
  } fields;
  uint8_t bits;
} mcp7940m_weekday_reg;

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 2;
    uint8_t _reserved : 2;
  } fields;
  uint8_t bits;
} mcp7940m_day_reg;

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 1;
    uint8_t leap_year : 1;
    uint8_t _reserved : 2;
  } fields;
  uint8_t bits;
} mcp7940m_month_reg;

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 4;
  } fields;
  uint8_t bits;
} mcp7940m_year_reg;

typedef union {
  struct {
    uint8_t sqw_output_freq : 2;
    uint8_t coarse_trim_mode_enable : 1;
    uint8_t ext_oscillator_enable : 1;
    uint8_t alm0_enable : 1;
    uint8_t alm1_enable : 1;
    uint8_t sqw_output_enable : 1;
    uint8_t gpo_level : 1;
  } fields;
  uint8_t bits;
} mcp7940m_control_reg;

typedef union {
  struct {
    uint8_t value : 7;
    uint8_t sign : 1;
  } fields;
  uint8_t bits;
} mcp7940m_osctrim_reg;

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 3;
    uint8_t _reserved : 1;
  } fields;
  uint8_t bits;
} mcp7940m_almsec_reg;

typedef union {
  struct {
    uint8_t ones : 4;
    uint8_t tens : 3;
    uint8_t _reserved : 1;
  } fields;
  uint8_t bits;
} mcp7940m_almmin_reg;

typedef mcp7940m_hour_reg mcp7940m_almhour_reg;

typedef union {
  struct {
    uint8_t weekday : 3;
    uint8_t alm_int_flag : 1;
    uint8_t alm_mask : 3;
    uint8_t alm_polarity : 1;
  } fields;
  uint8_t bits;
} mcp7940m_almweekday_reg;

typedef mcp7940m_day_reg mcp7940m_almday_reg;
typedef mcp7940m_month_reg mcp7940m_almmonth_reg;

#define MCP7940M_REG_READ_DECL(name, type, address)                                                                    \
  mcp7940m_##type mcp7940m_##name(const mcp7940m_read_cb read_cb) {                                                    \
    return (const mcp7940m_##type){                                                                                    \
        .bits = read_cb(mcp7940m_##address),                                                                           \
    };                                                                                                                 \
  }

MCP7940M_REG_READ_DECL(sec_reg_read, sec_reg, sec_reg_addr);
MCP7940M_REG_READ_DECL(min_reg_read, min_reg, min_reg_addr);
MCP7940M_REG_READ_DECL(hour_reg_read, hour_reg, hour_reg_addr);
MCP7940M_REG_READ_DECL(weekday_reg_read, weekday_reg, weekday_reg_addr);
MCP7940M_REG_READ_DECL(day_reg_read, day_reg, day_reg_addr);
MCP7940M_REG_READ_DECL(month_reg_read, month_reg, month_reg_addr);
MCP7940M_REG_READ_DECL(year_reg_read, year_reg, year_reg_addr);
MCP7940M_REG_READ_DECL(control_reg_read, control_reg, control_reg_addr);
MCP7940M_REG_READ_DECL(osctrim_reg_read, osctrim_reg, osctrim_reg_addr);

MCP7940M_REG_READ_DECL(alm0sec_reg_read, almsec_reg, alm0sec_reg_addr);
MCP7940M_REG_READ_DECL(alm0min_reg_read, almmin_reg, alm0min_reg_addr);
MCP7940M_REG_READ_DECL(alm0hour_reg_read, almhour_reg, alm0hour_reg_addr);
MCP7940M_REG_READ_DECL(alm0weekday_reg_read, almweekday_reg, alm0weekday_reg_addr);
MCP7940M_REG_READ_DECL(alm0day_reg_read, almday_reg, alm0day_reg_addr);
MCP7940M_REG_READ_DECL(alm0month_reg_read, almmonth_reg, alm0month_reg_addr);

MCP7940M_REG_READ_DECL(alm1sec_reg_read, almsec_reg, alm1sec_reg_addr);
MCP7940M_REG_READ_DECL(alm1min_reg_read, almmin_reg, alm1min_reg_addr);
MCP7940M_REG_READ_DECL(alm1hour_reg_read, almhour_reg, alm1hour_reg_addr);
MCP7940M_REG_READ_DECL(alm1weekday_reg_read, almweekday_reg, alm1weekday_reg_addr);
MCP7940M_REG_READ_DECL(alm1day_reg_read, almday_reg, alm1day_reg_addr);
MCP7940M_REG_READ_DECL(alm1month_reg_read, almmonth_reg, alm1month_reg_addr);

#define MCP7940M_REG_WRITE_DECL(name, type, address)                                                                   \
  void mcp7940m_##name(const mcp7940m_write_cb write_cb, const mcp7940m_##type reg) {                                  \
    write_cb(mcp7940m_##address, reg.bits);                                                                            \
  }

MCP7940M_REG_WRITE_DECL(sec_reg_write, sec_reg, sec_reg_addr);
MCP7940M_REG_WRITE_DECL(min_reg_write, min_reg, min_reg_addr);
MCP7940M_REG_WRITE_DECL(hour_reg_write, hour_reg, hour_reg_addr);
MCP7940M_REG_WRITE_DECL(weekday_reg_write, weekday_reg, weekday_reg_addr);
MCP7940M_REG_WRITE_DECL(day_reg_write, day_reg, day_reg_addr);
MCP7940M_REG_WRITE_DECL(month_reg_write, month_reg, month_reg_addr);
MCP7940M_REG_WRITE_DECL(year_reg_write, year_reg, year_reg_addr);
MCP7940M_REG_WRITE_DECL(control_reg_write, control_reg, control_reg_addr);
MCP7940M_REG_WRITE_DECL(osctrim_reg_write, osctrim_reg, osctrim_reg_addr);

MCP7940M_REG_WRITE_DECL(alm0sec_reg_write, almsec_reg, alm0sec_reg_addr);
MCP7940M_REG_WRITE_DECL(alm0min_reg_write, almmin_reg, alm0min_reg_addr);
MCP7940M_REG_WRITE_DECL(alm0hour_reg_write, almhour_reg, alm0hour_reg_addr);
MCP7940M_REG_WRITE_DECL(alm0weekday_reg_write, almweekday_reg, alm0weekday_reg_addr);
MCP7940M_REG_WRITE_DECL(alm0day_reg_write, almday_reg, alm0day_reg_addr);
MCP7940M_REG_WRITE_DECL(alm0month_reg_write, almmonth_reg, alm0month_reg_addr);

MCP7940M_REG_WRITE_DECL(alm1sec_reg_write, almsec_reg, alm1sec_reg_addr);
MCP7940M_REG_WRITE_DECL(alm1min_reg_write, almmin_reg, alm1min_reg_addr);
MCP7940M_REG_WRITE_DECL(alm1hour_reg_write, almhour_reg, alm1hour_reg_addr);
MCP7940M_REG_WRITE_DECL(alm1weekday_reg_write, almweekday_reg, alm1weekday_reg_addr);
MCP7940M_REG_WRITE_DECL(alm1day_reg_write, almday_reg, alm1day_reg_addr);
MCP7940M_REG_WRITE_DECL(alm1month_reg_write, almmonth_reg, alm1month_reg_addr);

#endif
