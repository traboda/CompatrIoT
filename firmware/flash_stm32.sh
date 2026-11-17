#!/usr/bin/env bash
# Flash the "Target Acquired" STM32F103 firmware over SWD. Fully automatic:
# finds a flashing tool (OpenOCD or st-flash) and an attached ST-Link/J-Link,
# flashes the target image, verifies. No arguments needed.
#
# Wire the probe to header H3 on the badge:  SWDIO->pin 1, SWCLK->pin 2,
# NRST->pin 3, GND->pin 4, and give the STM32 3V3 (the badge's own power is fine).
#
#   ./flash_stm32.sh
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ELF="$HERE/stm32_target/compatriot_stm32_target.elf"
BIN="$HERE/stm32_target/compatriot_stm32_target.bin"

say()  { printf '\n\033[1;32m==>\033[0m %s\n' "$1"; }
die()  { printf '\033[1;31m!!\033[0m %s\n' "$1" >&2; exit 1; }

[ -f "$ELF" ] || [ -f "$BIN" ] || die "STM32 target image not found under $HERE/stm32_target/"

# Prefer OpenOCD (programs + verifies from the ELF); fall back to st-flash with the .bin.
if command -v openocd >/dev/null 2>&1 && [ -f "$ELF" ]; then
  say "Flashing the STM32 target over SWD with OpenOCD"
  # connect_assert_srst holds the core in reset while attaching; without it the target\'s
  # own wfi sleep loop makes a plain connect fail with "unable to reset target".
  openocd -f interface/stlink.cfg \
    -c "reset_config srst_only srst_nogate connect_assert_srst" \
    -f target/stm32f1x.cfg \
    -c "program \"$ELF\" verify reset exit" \
    || die "OpenOCD could not program the STM32. Check the ST-Link and the H3 wiring (SWDIO/SWCLK/NRST/GND)."
  say "Done - the STM32 is flashed and verified."
  exit 0
fi

if command -v st-flash >/dev/null 2>&1 && [ -f "$BIN" ]; then
  say "Flashing the STM32 target over SWD with st-flash"
  st-flash --reset write "$BIN" 0x08000000 \
    || die "st-flash could not program the STM32. Check the ST-Link and the H3 wiring."
  say "Done - the STM32 is flashed."
  exit 0
fi

# nothing available: install one automatically where we can
say "No SWD flashing tool found - trying to install one"
if command -v apt >/dev/null 2>&1; then
  sudo apt-get install -y openocd && exec "$0" "$@"
elif command -v brew >/dev/null 2>&1; then
  brew install open-ocd && exec "$0" "$@"
fi
die "Install OpenOCD (recommended) or stlink-tools, then re-run:
  Linux:  sudo apt install openocd
  macOS:  brew install open-ocd
  Then connect an ST-Link to H3 and run ./flash_stm32.sh again."
