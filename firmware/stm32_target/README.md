# STM32 target firmware

The firmware for the badge's secondary microcontroller (STM32F103C8T6, U7), used by
the **Target Acquired** challenge. It is electrically separate from the ESP32: no
UART, no GPIO activity, no link to the rest of the badge. The only way to interact
with it is SWD, over header H3.

This folder has the compiled image only, no source, since reading the source would
skip the point of the challenge.

| File | Use |
|------|-----|
| `compatriot_stm32_target.bin` | Raw flash image, for flashing a fresh STM32 or for a static dump-and-disassemble approach |
| `compatriot_stm32_target.elf` | Same firmware with its symbol table (function/variable names, no debug info), for setting GDB breakpoints by name over SWD |

## Flash it to a fresh board

```bash
openocd -f interface/stlink.cfg -f target/stm32f1x.cfg \
  -c "program compatriot_stm32_target.bin 0x08000000 verify reset exit"
```

Connect an SWD probe (ST-Link or J-Link) to header H3: `SWDIO, SWCLK, NRST, GND`.
H3 has no 3V3 pin, the STM32 target is already powered from the main board.

## Solving the challenge

See [`challenges/embedded.md`](../../challenges/embedded.md) for the full
player-facing guide (both the static-dump and live-GDB approaches).
