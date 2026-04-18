#ifndef WS2812_H
#define WS2812_H

#include "ch32v00x.h"
#include <stdint.h>

#define NEO_PIN         GPIO_Pin_1
#define NEO_PORT        GPIOC
#define NEO_CLK         RCC_APB2Periph_GPIOC

#define NEO_PIN_BM      0x02
#define NEO_GPIO_BASE   0x40011000
#define NEO_GPIO_BSHR   0x10
#define NEO_GPIO_BCR    0x14

#define NEO_MAX_LEDS    8

void neo_init(void);
void neo_set(uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void neo_show(uint8_t num_leds);
void neo_clear(uint8_t num_leds);

#endif