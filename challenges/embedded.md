# Compatriot: Embedded Challenges

## Tools

Four pieces of hardware cover most of the eight challenges below. None of it needs to be expensive, the cheap clones of each work fine. Two-Wire Tap additionally needs a second microcontroller with I2C slave support (any spare Arduino, ESP32, or STM32 board), see that challenge for why.

Serial adapter

USB-TTL / any USB-serial cable. Drives Shell Breakout, Bus Stop, and flag submission.

Logic analyzer

Saleae-compatible, run with the Logic 2 app. Used for Baud Hunter, Two-Wire Tap, Bus Stop.

CH341A programmer

With an SOIC-8 test clip. Dumps the external EEPROM and flash chips. Set its voltage jumper to 3.3V first.

SWD probe

ST-Link or J-Link. Debugs the STM32 target over H3 for Target Acquired.

**Already on the Arsenal Labs kit?** All of this is preinstalled there. Skip straight to Header reference below.



## Install the tools

4 installs + serial setup

Install all of this once on a Linux machine, in a terminal.

**Serial terminal:** `screen`, `minicom` or `picocom` all work, pick one.
    
    
    sudo apt install minicom

**Logic 2** (Saleae's logic analyzer app) isn't a package, download the installer directly:

[saleae.com/downloads](https://www.saleae.com/downloads/), grab the Linux `.AppImage`, mark it executable (`chmod +x Logic*.AppImage`), then run it. Free, no account needed for the app itself.

Don't have real Saleae hardware? The cheap 8-channel clones work fine with the free, open-source **PulseView** instead (`sudo apt install pulseview`, or [sigrok.org/wiki/Downloads](https://sigrok.org/wiki/Downloads)). The steps below are written for Logic 2; PulseView's menu names differ but the same settings (sample rate, channel assignment, decoder/analyzer type) are all there under different labels.

**flashrom** and **i2c-tools** , for the CH341A:
    
    
    sudo apt install flashrom i2c-tools

**OpenOCD** and the **ARM GDB** toolchain, for the STM32 SWD probe:
    
    
    sudo apt install openocd gdb-multiarch

If `arm-none-eabi-gdb` specifically isn't on your system after that, install the full cross toolchain instead:
    
    
    sudo apt install gcc-arm-none-eabi

**Connecting to the badge's serial port.** This is the same every time you see "connect at 115200 8N1" below, for Shell Breakout, Bus Stop, and for submitting a flag. Everything here assumes Linux (the Arsenal Labs kit).

  1. Plug the badge into your computer over USB.
  2. Find its port name: 
         
         ls /dev/ttyACM*

It's usually `/dev/ttyACM0`.
  3. Open it with your serial terminal at 115200 baud, 8 data bits, no parity, 1 stop bit (written as "115200 8N1"). Any of these work: 
         
         minicom -D /dev/ttyACM0 -b 115200
         
         screen /dev/ttyACM0 115200
         
         putty -serial /dev/ttyACM0 -sercfg 115200,8,n,1,N

(PuTTY isn't just a Windows tool, `sudo apt install putty` gets you the same program and the same command-line flags on Linux.)
  4. You should land at a prompt or see the badge printing text. If it's silent, make sure the right lab or screen is open on the badge.



## Header reference (H1, H2, H3)

Three 4-pin headers sit next to the ESP32 module: **H3** is the STM32's SWD debug port (Target Acquired), **H2** is the ESP32's own JTAG/SWD port, and **H1** is an internal UART bridge between the two chips. None of the bus-sniffing labs below use these three, but it's easy to clip onto the wrong one.

Header| Pin 1| Pin 2| Pin 3| Pin 4  
---|---|---|---|---  
H3 (STM32 SWD)| SWDIO| SWCLK| NRST| GND  
H2 (ESP32 JTAG)| 3V3| —| TDI| TDO  
H1 (UART bridge)| STM32_TX| RX_ESP32| STM32_RX| TX_ESP32  
  
## Chip-dump targets: EEPROM & flash

EEPROM Raider and Flash Raider each target a separate memory chip, each on its own bus.

Chip| Bus| Signals  
---|---|---  
U2 (M24256E)| I2C, addr 0x50| I2C_SCL / I2C_SDA (IO22 / IO21)  
U5 (W25Q64)| SPI| SPI_CLK / SPI_MOSI / SPI_MISO / FLASH_CS  
  
01

## Shell Breakout

Easy

The badge exposes a minimal UNIX-like shell over USB serial with a small fake filesystem. A flag file sits in a hidden directory. A plain directory listing won't show it, but that doesn't mean it can't be reached.

#### Tool

A serial terminal at 115200 8N1 (PuTTY, `screen`, `minicom`).

Connection** 

No extra wiring. Just the badge's own USB cable, plugged into your computer. See Connecting to the badge's serial port above for the exact commands.

02

## Baud Hunter (UART)

Easy

A flag streams continuously on a UART TX line, but not at the obvious default rate, so capturing garbage at the standard assumption is expected. The whole puzzle is finding the real baud.

#### Tool

Logic analyzer + Logic 2. Wire a channel to `IO16` (header U13, pin 4) plus GND.

#### Approach

Capture at ≥1 MS/s and add a UART analyzer. The default-rate decode will be garbage. Either measure one bit's duration directly off the waveform and compute the rate from it, or just step through the standard rates until one decodes cleanly.

**Connection** 

**Where:** header **U13** (one of the 1x7 headers near the ESP32 module), pin 4. On the schematic this pin is labeled `TX_ESP32` rather than "IO16" directly, since it's wired straight out to a UART bridge, but it's the same signal:

Badge pin| U13 pin| Logic analyzer  
---|---|---  
IO16| 4 (TX_ESP32)| Any free channel  
GND| any GND pin| GND, do not skip this wire  
  
That's it, two wires. There's nothing to receive, so no TX channel on the analyzer side is needed.

03

## Two-Wire Tap (I2C)

Medium

Not a pure sniffing challenge: the badge tries to write the flag to I2C address `0x42`, but nothing on the bus answers, so the write gets NAK'd on the address byte and aborts before any data goes out. That's real I2C behavior, not a bug. To see anything at all, you have to supply the missing device.

#### Tool

A second microcontroller with I2C slave support (any 3.3 V board: Arduino, another ESP32, STM32, whatever you have), plus a logic analyzer + Logic 2 to confirm the capture. Wire to `SDA (IO21)` / `SCL (IO22)` (header U14, pins 5 and 6) plus GND.

#### Approach

Wire your own board onto the same bus and program it as an I2C slave that ACKs address `0x42`. Once something answers, the badge's write completes for real and the flag goes out on the wire where you can capture it.

**Connection** 

**Where:** header **U14** (one of the 1x7 headers near the ESP32 module), pin 5 = SDA, pin 6 = SCL. Straight from the board's own schematic:

Badge pin| U14 pin| Wire to  
---|---|---  
IO21| 5 (I2C_SDA)| your board's SDA + logic analyzer  
IO22| 6 (I2C_SCL)| your board's SCL + logic analyzer  
GND| any GND pin| shared GND, both devices  
  
Same bus the EEPROM Raider chip sits on. Don't drive 5 V logic onto these pins, only 3.3 V.

04

## Bus Stop (SPI)

Medium

A two-step one: the SPI bus carries a _password_ , not the flag directly. Sniff the password, then type it back into the lab's own serial console to unlock the flag. It's a small model of a bus leaking a credential rather than the secret itself.

#### Tool

Logic analyzer + Logic 2, plus a serial terminal for the console step. Wire to `SCK (IO2)` / `MOSI (IO3)` / `CS (IO15)` plus GND.

#### Approach

Capture at ≥1 MS/s, add an SPI analyzer set to mode 0 / MSB-first / CS active-low, and read the MOSI lane's bytes as ASCII. Open a serial connection to the badge with this lab selected and send the recovered text back.

CS lives on IO15, not IO4. IO4 is also wired to the onboard SWD line used to program the STM32 target (see Target Acquired), so this lab was moved off it to keep the two from fighting over the same pin.

**Connection**  Badge pin| Header| Logic analyzer  
---|---|---  
IO2| U13 pin 6| SCK  
IO3| U13 pin 5| MOSI  
IO15| U13 pin 2| CS  
GND| U13 GND| GND  
  
**Where:** all three land on header **U13** , same connector as IO16 for Baud Hunter. SCK is pin 6 (net `MOD1_CS`), MOSI is pin 5 (net `MOD2_CS`), CS is pin 2 (a spare GPIO15, labeled on the schematic as just `GPIO15`).

No MISO wire, there isn't one. This is a transmit-only bus, leave that channel unassigned in your decoder.

05

## EEPROM Raider

Medium

The flag lives on the external I2C EEPROM (U2), not inside the ESP32's own firmware. A plain single-byte-addressed dump reads the wrong location on this chip, it needs two address bytes.

#### Tool

CH341A programmer in I2C mode + `i2c-tools`.

#### Approach

Power the badge off before clipping on. Confirm the chip answers at its address with `i2cdetect`, then use a two-byte-address read starting at offset 0\. This chip is large enough that it needs the full 16-bit address, not the single-byte form most dump commands default to.

**Connection** 

CH341A| U2 (EEPROM)  
---|---  
SDA| SDA  
SCL| SCL  
VCC| 3.3V  
GND| GND  
  
**Power the badge off first.** Switch the CH341A to I2C mode and clip the SOIC-8 test clip onto U2, matching pin 1 carefully, the same precision this takes on Flash Raider.

06

## Flash Raider

Medium

Same idea as EEPROM Raider, different chip and bus: the flag lives on the external SPI NOR flash (U5), not in the ESP32's own program memory. The most hands-on challenge on the badge, and the most common real hardware-hacking workflow there is.

#### Tool

CH341A programmer in SPI mode + `flashrom`.

#### Approach

Power the badge off and unplug USB entirely before clipping on. Check the CH341A's voltage jumper is set to 3.3V, not 5V. Confirm the programmer identifies the chip before dumping, then search the dumped image for a readable flag banner.

**Connection** 

CH341A| U5 (flash)  
---|---  
CS| CS  
CLK| SCK  
DI| MOSI  
DO| MISO  
VCC| 3.3V  
GND| GND  
  
**Power the badge off and unplug USB entirely first.** Check the CH341A's voltage jumper is set to 3.3V, not 5V, that's the single most common way to damage this chip. Clip the SOIC-8 test clip onto U5, matching pin 1 carefully.

07

## Blinkenlights (LED)

Medium

An optical side-channel. The flag is blinked out in Morse on the badge's on-board LED. No wires needed, but decoding it takes patience, which is why it sits here rather than earlier.

#### Tool

Nothing but a phone camera.

#### Approach

Film at least one full loop, including the longer gap that marks where the message repeats, then scrub frame-by-frame. The shortest flash is one unit. Everything else (dashes, gaps between symbols and characters) is a whole-number multiple of it.

**Connection** 

Nothing to wire up. The NeoPixel is on-board (IO10), there's no external hardware for this one at all, just point a phone camera at the badge.

08

## Target Acquired (STM32)

Hard

A real firmware-reversing challenge against the badge's second microcontroller: an STM32F103C8T6 wired up purely as a training target, unconnected to the ESP32's own challenge engine. Its firmware holds the flag encrypted, not in the clear, so a plain dump won't hand it over directly.

#### Tool

ST-Link or J-Link SWD probe + `openocd` \+ `arm-none-eabi-gdb` (or a disassembler like Ghidra). See the H3 header table above for wiring.

#### Approach, two valid paths

**Static:** dump the flash over SWD, disassemble it, find the small decryption loop and recover its key, then apply it to the ciphertext sitting nearby in the dump.  
**Live:** attach GDB to the running chip instead, it already decrypted the flag into RAM once at boot, so you can read the plaintext straight out of memory without redoing the math.

A probe reporting "cannot identify target" almost always means SWDIO/SWCLK are swapped.

**Connection** 

Full H3 pinout and a real photo of the header are in the header reference near the top of this page.

H3 pin| Probe wire  
---|---  
SWDIO| SWDIO  
SWCLK| SWCLK  
NRST| NRST (if your probe breaks it out)  
GND| GND  
  
H3 has no 3V3 pin, the STM32 target is already powered from the main board. Don't wire 3V3 to this header.
