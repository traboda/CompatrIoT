# Firmware

Pre-built binaries for both chips on the Compatriot badge. No build toolchain needed,
just flash and go.

- The ESP32-C6 firmware below runs the badge itself: UI, challenge logic, BLE/WiFi, CAN bridge.
- [`stm32_target/`](stm32_target) has the separate firmware for the STM32 target chip used by the Target Acquired challenge.

## Quick flash (recommended)

```bash
pip install esptool      # if you don't have it
./flash.sh /dev/ttyACM0  # adjust the port if needed
```

This writes `compatriot_merged.bin`, a single image with the bootloader, partition
table and application already combined, to flash offset `0x0`. One command, no
partition math.

## Manual flash (advanced)

If you'd rather flash each piece at its normal offset instead of the merged image:

| File | Offset |
|------|--------|
| `bootloader.bin` | `0x0` |
| `partitions.bin` | `0x8000` |
| `boot_app0.bin` | `0xe000` |
| `compatriot_app.bin` | `0x10000` |

```bash
esptool.py --chip esp32c6 --port /dev/ttyACM0 --baud 921600 write_flash \
  0x0      bootloader.bin \
  0x8000   partitions.bin \
  0xe000   boot_app0.bin \
  0x10000  compatriot_app.bin
```

## Notes

- Target: ESP32-C6-WROOM-1 (RISC-V), 4 MB flash, `huge_app` partition scheme.
- USB shows up as a CDC serial port (`/dev/ttyACM0` on Linux). Connect a serial
  terminal at **115200 8N1** after flashing to see the badge's own console.
- Flashing preserves nothing by itself; challenge progress lives in NVS and survives
  a normal reboot, but a full chip erase (`esptool.py erase_flash`) wipes it.
