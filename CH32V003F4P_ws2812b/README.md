# WS2812B on CH32V003F4P

WS2812B (NeoPixel) driver for the CH32V003F4P RISC-V microcontroller, using inline assembly GPIO bitbanging to meet the strict timing requirements of the WS2812 protocol.

## Hardware

| Item | Detail |
|------|--------|
| MCU | CH32V003F4P (RV32EC, 48 MHz) |
| Flash / RAM | 16 KB / 2 KB |
| LED data pin | PC1 |
| Max LEDs | 8 (compile-time constant `NEO_MAX_LEDS`) |

## How it works

The WS2812 protocol requires precise pulse widths that are difficult to achieve reliably in C at 48 MHz. This driver uses **inline RISC-V assembly** with NOP padding to hit the required timings:

| Symbol | High time | Low time |
|--------|-----------|----------|
| Bit 0  | ~350 ns   | ~850 ns  |
| Bit 1  | ~700 ns   | ~600 ns  |
| Reset  | 60 µs LOW after all data |

Global interrupts are disabled for the duration of `neo_show()` to prevent jitter.

Data is sent MSB-first in **GRB order** (green, red, blue) as required by the WS2812 spec.

## API

```c
void neo_init(void);                          // configure PC1 as push-pull output
void neo_set(uint8_t idx, uint8_t r, uint8_t g, uint8_t b); // set LED colour in buffer
void neo_show(void);                          // transmit buffer to LEDs
void neo_clear(void);                         // set all LEDs off and show
```

## Demo animations

`main.c` includes four patterns that loop indefinitely:

- **Rainbow chase** — full HSV spectrum scrolling across all LEDs
- **Colour wipe** — progressive fill in red, green, then blue
- **Theatre chase** — every third LED marching in orange and cyan
- **Twinkle** — random LEDs sparkling with a fade decay

Brightness is capped at `64/255` by default to avoid eye strain.

## Building

The project uses Eclipse CDT with the GNU RISC-V toolchain.

**Toolchain:** `riscv-none-embed-gcc`  
**Architecture flags:** `-march=rv32ecxw -mabi=ilp32e`  
**Optimisation:** `-Os` with `-ffunction-sections -fdata-sections`

Open the project in MounRiver Studio or a compatible Eclipse CDT environment, select the Release configuration, and build. The output artefacts (`*.elf`, `*.hex`) appear in the `obj/` directory.

Flash the `.hex` to the CH32V003 using WCH-Link and the WCH Flash Tool (or OpenOCD with WCH support).

## Project structure

```
├── User/
│   ├── main.c          # animation patterns
│   ├── ws2812.c        # driver — timing-critical assembly
│   └── ws2812.h        # public API and pin definitions
├── Core/               # RISC-V startup and vector table
├── Peripheral/         # CH32V003 HAL (GPIO, RCC, timers, …)
├── Debug/              # UART printf and microsecond delay
└── Ld/Link.ld          # linker script (16 KB flash, 2 KB RAM)
```

## License

This project is released into the public domain. The CH32V003 peripheral library files retain their original WCH licensing.
