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

**Hint** 

`ls` is not the whole truth on a UNIX box. A directory can hold something a listing doesn't show you, and `cd` doesn't care what `ls` hides.

**Full guide** 

  1. Connect at 115200 8N1. You land at a shell prompt in the home directory.
  2. `ls` shows a notes file and a couple of directories. Nothing obviously useful.
  3. `cat` the notes file. It's a red herring about a password, a nudge to keep digging rather than an answer.
  4. `cd` into the documents directory and `ls` again. Only one file shows up.
  5. The shell's `ls` hides dot-entries, but `cd` doesn't block them. Try `cd .secret` even though you never saw it listed. It works.
  6. `ls` again inside it. The flag file is right there.
  7. `cat` it. The flag prints, and the badge marks the lab solved the instant it does.



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

**Hint** 

9600 was wrong. Measure a bit's width directly off the waveform instead of guessing, or just try the other standard rates in order.

**Full guide** 

  1. Wire a logic analyzer channel to `IO16` on header U13 pin 4, and its GND to any GND pin on the badge. Don't skip the ground wire.
  2. In Logic 2, open the channel's sample rate dropdown (top toolbar) and set it to at least 1 MS/s, then click **Start** to capture 2-3 seconds.
  3. Click the **+** next to "Analyzers" in the right-hand panel, pick **Async Serial** , set its Input Channel to the one you wired, and leave the baud rate on the default 9600 for now.
  4. Look at the decode: at 9600 it's garbage, which just confirms something is transmitting and that 9600 is wrong.
  5. Zoom into one data bit on the waveform and measure its width directly (click-drag a measurement across the narrowest pulse, Logic 2 shows the time delta). Baud is 1 divided by that time. Expect something in the tens of microseconds.
  6. Re-open the analyzer's settings (gear icon) and set the baud rate to `57600`. If you'd rather not measure, that's the answer: 9600 was the wrong guess, the badge actually transmits at **57600 baud**.
  7. The decoded bytes show in the Data Table at the bottom and annotated on the trace itself, spelling out the flag, repeating once per second.



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

**Hint** 

Nothing answers at 0x42 yet. Build the thing that should.

**Full guide** 

  1. Wire a second microcontroller onto the bus: its SDA to `IO21` (U14 pin 5), its SCL to `IO22` (U14 pin 6), and a shared GND.
  2. Program it as an I2C slave on address `0x42` (e.g. Arduino `Wire.begin(0x42)` with an `onReceive()` handler). It doesn't need to do anything with the bytes, it just has to exist and ACK.
  3. Also clip a logic analyzer onto IO21/IO22 + GND. Set the sample rate to ≥1 MS/s and click **Start**.
  4. Click the **+** next to Analyzers, pick **I2C** , and set its SDA/SCL inputs to the matching channels.
  5. With your slave running, the Data Table now shows a Start, an address byte with a Write flag that's **ACKed** this time, a run of data bytes, then a Stop, repeating about once a second.
  6. Read the data bytes as ASCII directly from the Data Table (it has an ASCII column), or by hand from the hex. Together they spell the flag.



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

**Hint** 

The bus isn't leaking the flag, it's leaking the way in. Mode 0, MSB first, and the password is plain ASCII once decoded.

**Full guide** 

  1. Wire SCK to `IO2` (U13 pin 6), MOSI to `IO3` (U13 pin 5), CS to `IO15` (U13 pin 2), plus GND. Leave MISO unassigned, this is a transmit-only bus.
  2. Set the sample rate to ≥1 MS/s (10x the ~100 kHz bus clock) and click **Start** , let it run a couple of seconds to catch a few bursts.
  3. Click the **+** next to Analyzers, pick **SPI** , assign CLK/MOSI/ Enable to the matching channels, and set Clock State (CPOL) = 0, Clock Phase (CPHA) = 0, Bits per Transfer = 8, Significant Bit = MSB first, Enable Line = Active Low.
  4. The MOSI lane's decode in the Data Table reads as ASCII text: that's the password. If you see garbage instead, double-check the Enable polarity and CPOL/CPHA, a mode mismatch is the usual culprit.
  5. Open a serial connection to the badge at 115200 8N1 with this lab selected, and type the recovered password in.
  6. The badge replies with the flag directly in the terminal and marks the lab solved.



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

**Hint** 

This isn't a 24C02. One address byte reads you garbage from the wrong place, this chip needs two.

**Full guide** 

  1. Power the badge off. Switch the CH341A to I2C mode and clip it onto U2: SDA, SCL, VCC (3.3V), GND.
  2. Find which `/dev/i2c-N` bus it registered as with `i2cdetect -l`.
  3. Confirm the chip answers: `i2cdetect -y <bus>`. You should see `50` in the grid.
  4. Don't reach for plain `i2cdump`, it assumes single-byte addressing, which is wrong for a 32 KB part. Use `i2ctransfer -y <bus> w2@0x50 0x00 0x00 r128@0x50` instead. This writes the two-byte address 0x0000, then reads 128 bytes back from there.
  5. The output is the banner in ASCII, flag included.



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

**Hint** 

The flag was never in the microcontroller. Dump the external chip and search the raw bytes for readable text.

**Full guide** 

  1. Power the badge off and unplug USB entirely. Never clip onto a chip another master is actively driving.
  2. Check the CH341A's voltage jumper is set to 3.3V, not 5V. This is the single most common way to damage the chip.
  3. Clip the SOIC-8 test clip onto U5, matching pin 1 carefully. A clip rotated or offset by one pin is the usual cause of a failed read.
  4. Confirm the programmer sees the chip: `flashrom -p ch341a_spi`. It should identify a W25Q64. Don't proceed until this succeeds cleanly.
  5. Dump it: `flashrom -p ch341a_spi -r dump.bin`.
  6. Search the image: `strings dump.bin | grep wired{`. The banner sits at offset 0.



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

**Hint** 

Shortest flash equals one unit. Every other flash and gap is a whole-number multiple of that same length.

**Full guide** 

  1. Film the LED in video mode for 10-15 seconds, long enough to catch one full loop including the long repeat gap.
  2. Scrub the video frame by frame to tell short flashes from long ones precisely.
  3. Identify the unit length from the shortest flash. Dashes, inter-symbol gaps and inter-character gaps are all whole-number multiples of it.
  4. Transcribe what you see into dots and dashes, using gap length to tell where one symbol, one character, and the whole message end.
  5. Decode the Morse with any reference. Watch for the less common sequences that spell the flag's punctuation.



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

**Hint** 

The flag isn't in flash as plaintext, but the chip already decrypted it once into RAM at boot. You can watch it happen, or redo the single instruction that undoes it.

**Full guide** 

**Static path.**

  1. Wire the probe to H3 (see Connection above): SWDIO, SWCLK, GND, and NRST if your probe has it.
  2. Dump flash over SWD: `openocd -f interface/stlink.cfg -f target/stm32f1x.cfg -c "init" -c "reset halt" -c "dump_image dump.bin 0x08000000 0x10000" -c "shutdown"`
  3. Confirm it's encrypted: `strings dump.bin | grep wired{` finds nothing.
  4. Disassemble: `arm-none-eabi-objdump -D -b binary -m arm --no-show-raw-insn dump.bin | less` (or import into Ghidra as raw ARM Cortex at base 0x08000000).
  5. Find the small loop that reads a byte, XORs it with an immediate, and writes it elsewhere. That immediate is the key.
  6. XOR the ciphertext sitting next to it in `.rodata` with that key to get the flag.



**Live path, faster.**

  1. Start OpenOCD and leave it running: `openocd -f interface/stlink.cfg -f target/stm32f1x.cfg`
  2. In a second terminal, attach GDB: `arm-none-eabi-gdb`, then `target remote localhost:3333`.
  3. Disassemble the start of flash to find the key and the RAM address the decrypted bytes land in: `x/40i 0x08000000`.
  4. Read that RAM address as a string, e.g. `x/29bs 0x20000000` (adjust to whatever address your disassembly actually points at). The flag prints already decrypted.


