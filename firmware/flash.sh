#!/usr/bin/env bash
# Flash the Compatriot badge firmware (ESP32-C6) over USB.
# Usage: ./flash.sh [/dev/ttyACM0]
set -euo pipefail

PORT="${1:-/dev/ttyACM0}"
HERE="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

if ! command -v esptool.py >/dev/null 2>&1 && ! command -v esptool >/dev/null 2>&1; then
  echo "esptool not found. Install it with: pip install esptool" >&2
  exit 1
fi
ESPTOOL=$(command -v esptool.py || command -v esptool)

echo "Flashing $HERE/compatriot_merged.bin to $PORT ..."
"$ESPTOOL" --chip esp32c6 --port "$PORT" --baud 921600 \
  write_flash 0x0 "$HERE/compatriot_merged.bin"

echo "Done. Reset the badge if it doesn't reboot on its own."
