#!/usr/bin/env bash
# Flash the Compatriot badge ESP32-C6 firmware over USB. Fully automatic:
# finds esptool, finds the badge's port, flashes, verifies. No arguments needed.
#
#   ./flash_esp32.sh                 # auto-detect everything
#   ./flash_esp32.sh /dev/ttyACM1    # or name the port explicitly
set -euo pipefail

HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
IMG="$HERE/compatriot_merged.bin"

say()  { printf '\n\033[1;32m==>\033[0m %s\n' "$1"; }
die()  { printf '\033[1;31m!!\033[0m %s\n' "$1" >&2; exit 1; }

[ -f "$IMG" ] || die "Firmware image not found: $IMG"

# esptool, by either name, installing it automatically if it is missing
ESPTOOL="$(command -v esptool.py || command -v esptool || true)"
if [ -z "$ESPTOOL" ]; then
  say "esptool not found - installing it"
  python3 -m pip install --user --quiet esptool \
    || pip3 install --user --quiet esptool \
    || die "could not install esptool. Install it with: pip install esptool"
  ESPTOOL="$(command -v esptool.py || command -v esptool || echo "$HOME/.local/bin/esptool.py")"
fi

# find the badge's serial port if one was not given
PORT="${1:-}"
if [ -z "$PORT" ]; then
  say "Looking for the badge"
  for p in /dev/ttyACM* /dev/tty.usbmodem* /dev/cu.usbmodem*; do
    [ -e "$p" ] && PORT="$p" && break
  done
  [ -n "$PORT" ] || die "No badge found on USB (no /dev/ttyACM*). Plug it in and try again."
  echo "   using $PORT"
fi

say "Flashing $(basename "$IMG") to $PORT"
"$ESPTOOL" --chip esp32c6 --port "$PORT" --baud 921600 \
  write_flash 0x0 "$IMG"

say "Done - the badge reboots into the new firmware."
