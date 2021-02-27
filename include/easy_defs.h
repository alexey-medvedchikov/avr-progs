#ifndef __EASY_DEFS_H__
#define __EASY_DEFS_H__ "easy_defs.h"

#include <stdint.h>

#define TIM_PRESCALE_1 ((1 << CS00))
#define TIM_PRESCALE_8 ((1 << CS01))
#define TIM_PRESCALE_64 ((1 << CS01) | (1 << CS00))
#define TIM_PRESCALE_256 ((1 << CS02))
#define TIM_PRESCALE_1024 ((1 << CS02) | (1 << CS00))
#define TIM_PRESCALE_EXTFALL ((1 << CS02) | (1 << CS01))
#define TIM_PRESCALE_EXTRISE ((1 << CS02) | (1 << CS01) | (1 << CS00))

#define WDT_PRESCALE_16MS (0)
#define WDT_PRESCALE_32MS ((1 << WDP0))
#define WDT_PRESCALE_64MS ((1 << WDP1))
#define WDT_PRESCALE_125MS ((1 << WDP1) | (1 << WDP0))
#define WDT_PRESCALE_250MS ((1 << WDP2))
#define WDT_PRESCALE_500MS ((1 << WDP2) | (1 << WDP0))
#define WDT_PRESCALE_1S ((1 << WDP2) | (1 << WDP1))
#define WDT_PRESCALE_2S ((1 << WDP2) | (1 << WDP1) | (1 << WDP0))
#define WDT_PRESCALE_4S ((1 << WDP3))
#define WDT_PRESCALE_8S ((1 << WDP3) | (1 << WDP0))

uint8_t bitval(const uint8_t pin) { return (uint8_t)(1 << pin); }

#endif
