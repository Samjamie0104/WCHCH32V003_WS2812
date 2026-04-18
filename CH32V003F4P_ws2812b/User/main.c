#include "debug.h"
#include "ws2812.h"

#define NUM_LEDS    5
#define BRIGHTNESS  64      // 0-255, keep low to avoid burning your eyes out

// ©¤©¤ Helpers ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤

// Scale a 0-255 brightness value by BRIGHTNESS
static uint8_t scale(uint8_t val) {
    return (uint8_t)((uint16_t)val * BRIGHTNESS / 255);
}

// HSV to RGB ¡ª hue 0-1535 (6 * 256), full sat/val
static void hsv(uint16_t h, uint8_t *r, uint8_t *g, uint8_t *b) {
    uint8_t reg  = h / 256;
    uint8_t frac = h % 256;
    switch (reg % 6) {
        case 0: *r=255;      *g=frac;     *b=0;        break;
        case 1: *r=255-frac; *g=255;      *b=0;        break;
        case 2: *r=0;        *g=255;      *b=frac;     break;
        case 3: *r=0;        *g=255-frac; *b=255;      break;
        case 4: *r=frac;     *g=0;        *b=255;      break;
        case 5: *r=255;      *g=0;        *b=255-frac; break;
    }
}

// Simple LCG random number generator (no stdlib needed)
static uint32_t rng_state = 0xDEADBEEF;
static uint8_t rng(void) {
    rng_state = rng_state * 1664525 + 1013904223;
    return (uint8_t)(rng_state >> 24);
}

// ©¤©¤ Patterns ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤

// 1. Rainbow chase ¡ª full spectrum scrolling across all LEDs
void pattern_rainbow(uint32_t duration_ms) {
    uint16_t offset = 0;
    uint32_t steps  = duration_ms / 20;
    for (uint32_t s = 0; s < steps; s++) {
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            uint8_t r, g, b;
            uint16_t hue = (offset + i * (1536 / NUM_LEDS)) % 1536;
            hsv(hue, &r, &g, &b);
            neo_set(i, scale(r), scale(g), scale(b));
        }
        neo_show(NUM_LEDS);
        offset = (offset + 8) % 1536;
        Delay_Ms(20);
    }
}

// 2. Colour wipe ¡ª fills LEDs one by one with a solid colour, then clears
void pattern_wipe(uint8_t r, uint8_t g, uint8_t b, uint32_t step_ms) {
    // wipe on
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        neo_set(i, scale(r), scale(g), scale(b));
        neo_show(NUM_LEDS);
        Delay_Ms(step_ms);
    }
    Delay_Ms(300);
    // wipe off
    for (uint8_t i = 0; i < NUM_LEDS; i++) {
        neo_set(i, 0, 0, 0);
        neo_show(NUM_LEDS);
        Delay_Ms(step_ms);
    }
    Delay_Ms(200);
}

// 3. Theatre chase ¡ª 1 in 3 LEDs lit, marching
void pattern_theatre(uint8_t r, uint8_t g, uint8_t b, uint32_t duration_ms) {
    uint32_t steps = duration_ms / 80;
    for (uint32_t s = 0; s < steps; s++) {
        uint8_t phase = s % 3;
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            if (i % 3 == phase) {
                neo_set(i, scale(r), scale(g), scale(b));
            } else {
                neo_set(i, 0, 0, 0);
            }
        }
        neo_show(NUM_LEDS);
        Delay_Ms(80);
    }
}

// 4. Twinkle ¡ª random LEDs pop on and fade
void pattern_twinkle(uint32_t duration_ms) {
    // fade buffer
    uint8_t fade_r[NUM_LEDS] = {0};
    uint8_t fade_g[NUM_LEDS] = {0};
    uint8_t fade_b[NUM_LEDS] = {0};

    uint32_t steps = duration_ms / 30;
    for (uint32_t s = 0; s < steps; s++) {
        // randomly spark a new LED
        if (rng() < 80) {
            uint8_t idx = rng() % NUM_LEDS;
            uint8_t r, g, b;
            hsv((rng() * 6), &r, &g, &b);
            fade_r[idx] = scale(r);
            fade_g[idx] = scale(g);
            fade_b[idx] = scale(b);
        }

        // apply and fade all LEDs
        for (uint8_t i = 0; i < NUM_LEDS; i++) {
            neo_set(i, fade_r[i], fade_g[i], fade_b[i]);
            // decay
            fade_r[i] = fade_r[i] > 8 ? fade_r[i] - 8 : 0;
            fade_g[i] = fade_g[i] > 8 ? fade_g[i] - 8 : 0;
            fade_b[i] = fade_b[i] > 8 ? fade_b[i] - 8 : 0;
        }
        neo_show(NUM_LEDS);
        Delay_Ms(30);
    }
}

// ©¤©¤ Main ©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤©¤

int main(void) {
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    SystemCoreClockUpdate();
    Delay_Init();

    neo_init();
    neo_clear(NUM_LEDS);

    while (1) {
        pattern_rainbow(4000);

        pattern_wipe(255, 0,   0,   120);   // Red
        pattern_wipe(0,   255, 0,   120);   // Green
        pattern_wipe(0,   0,   255, 120);   // Blue

        pattern_theatre(255, 100, 0, 3000); // Orange theatre chase
        pattern_theatre(0, 100, 255, 3000); // Blue theatre chase

        pattern_twinkle(5000);
    }
}