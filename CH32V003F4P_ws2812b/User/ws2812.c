#include "ws2812.h"
#include "debug.h"
#include <string.h>

static uint8_t neo_buf[NEO_MAX_LEDS][3];

void neo_init(void) {
    GPIO_InitTypeDef cfg = {0};
    RCC_APB2PeriphClockCmd(NEO_CLK, ENABLE);
    cfg.GPIO_Pin   = NEO_PIN;
    cfg.GPIO_Mode  = GPIO_Mode_Out_PP;
    cfg.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(NEO_PORT, &cfg);
    GPIO_ResetBits(NEO_PORT, NEO_PIN);
}

void neo_set(uint8_t idx, uint8_t r, uint8_t g, uint8_t b) {
    if (idx >= NEO_MAX_LEDS) return;
    neo_buf[idx][0] = g;
    neo_buf[idx][1] = r;
    neo_buf[idx][2] = b;
}

static void neo_send_byte(uint8_t data) {
    asm volatile (
        "   li   a5, 8               \n"
        "   li   a4, %[pin]          \n"
        "   li   a3, %[base]         \n"
        "1:                          \n"
        "   andi a2, %[byte], 0x80   \n"
        "   sw   a4, %[bshr](a3)     \n"
        "   bnez a2, 2f              \n"
        "   sw   a4, %[bcr](a3)      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   j    3f                  \n"
        "2:                          \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   nop                      \n"
        "   sw   a4, %[bcr](a3)      \n"
        "3:                          \n"
        "   slli %[byte], %[byte], 1 \n"
        "   addi a5, a5, -1          \n"
        "   bnez a5, 1b              \n"
        :
        [byte] "+r" (data)
        :
        [pin]  "i"  (NEO_PIN_BM),
        [base] "i"  (NEO_GPIO_BASE),
        [bshr] "i"  (NEO_GPIO_BSHR),
        [bcr]  "i"  (NEO_GPIO_BCR)
        :
        "a2", "a3", "a4", "a5", "memory"
    );
}

void neo_show(uint8_t num_leds) {
    if (num_leds > NEO_MAX_LEDS) num_leds = NEO_MAX_LEDS;
    __disable_irq();
    for (uint8_t i = 0; i < num_leds; i++) {
        neo_send_byte(neo_buf[i][0]);
        neo_send_byte(neo_buf[i][1]);
        neo_send_byte(neo_buf[i][2]);
    }
    __enable_irq();
    GPIO_ResetBits(NEO_PORT, NEO_PIN);
    Delay_Us(60);
}

void neo_clear(uint8_t num_leds) {
    memset(neo_buf, 0, sizeof(neo_buf));
    neo_show(num_leds);
}