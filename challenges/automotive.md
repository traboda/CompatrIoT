# Compatriot: Automotive Challenges

**Already on the Arsenal Labs kit?** All of this is preinstalled there. Skip straight to Bus setup below.



## Install the tools

3 installs

You'll need a Linux machine (a VM is fine) with a terminal open. A terminal is just a window where you type a command and press Enter to run it. Every command below is meant to be typed or pasted exactly as shown, one line at a time.

**can-utils** , for `candump`, `cansend` and friends:
    
    
    sudo apt install can-utils

**cannelloni** , the CAN-over-Ethernet bridge. It isn't a distro package, so it's built from source. Paste this whole block, it clones, builds, and installs it in one go:
    
    
    sudo apt install build-essential cmake libsctp-dev
    git clone https://github.com/mguentner/cannelloni.git
    cd cannelloni && mkdir build && cd build
    cmake .. && make
    sudo make install

**isotpsend / isotprecv** , needed for Find the VIN, usually ship inside can-utils too. Confirm you have them:
    
    
    which isotpsend isotprecv

If that prints nothing, they weren't bundled on your distro's can-utils package. They're part of the same can-utils source repo, build the whole repo from [github.com/linux-can/can-utils](https://github.com/linux-can/can-utils) the same way as cannelloni above if you need to.

## Bus setup

The badge is a W5500 Ethernet module speaking **cannelloni** (CAN-over-Ethernet) at a static IP. Bridge it to a local `vcan0` interface, once per session, and every SocketCAN tool on your machine sees the badge's live bus.

Your laptop 10.42.0.1/24 · vcan0 Ethernet Badge · RJ1 (W5500) 10.42.0.10 : 20000 internal bus Dashboard IDs 0x244 0x316 0x188 0x19B UDS ECU req 0x7E0 / resp 0x7E8

Do this once per session, in order:

  1. Plug an Ethernet cable between your computer and the badge's RJ45 jack. Any cable works, modern network cards handle the crossover automatically.
  2. Find your Ethernet interface's name: 
         
         ip link show

Look for the wired one, often something like `eth0` or `enp2s0`. Use that name in place of `<iface>` below.
  3. Give it an address on the badge's subnet: 
         
         sudo ip addr add 10.42.0.1/24 dev <iface>
         sudo ip link set <iface> up

  4. Confirm the badge answers: 
         
         ping -c3 10.42.0.10

If this fails, check the cable and that the previous step actually assigned the address (`ip addr show <iface>` should list it).
  5. Create the virtual CAN interface: 
         
         sudo ip link add vcan0 type vcan
         sudo ip link set up vcan0

  6. Start the bridge and leave it running in this terminal, don't close it: 
         
         cannelloni -I vcan0 -R 10.42.0.10 -r 20000 -l 20000

  7. Open a second terminal and confirm it's working: 
         
         candump vcan0

You should see four IDs repeating at roughly 10 Hz. If you see nothing, go back and recheck the earlier steps before trying any individual challenge.



The RJ1 jack (WIZ850io module), photographed off the board. This is what the badge's one Ethernet port looks like.

CAN ID| Signal| Layout  
---|---|---  
0x244| Speed| bytes 3-4, big-endian, km/h  
0x316| RPM| engine RPM  
0x188| Turn signals| byte 0, bit0 left, bit1 right  
0x19B| Doors| byte 2, bits 0-3, FL/FR/RL/RR  
  
**Two terminals, the whole time.** Terminal 1 keeps `cannelloni` running and untouched for your entire session. Terminal 2 is where you actually run `candump`/`cansend`/etc. for every challenge below. If a command below seems to hang or do nothing, check terminal 1 is still alive first.

01

## Floor It

Easy

The dashboard's speed reading comes off one CAN ID, and nothing checks whether that frame actually came from the badge's own sensors. Find the ID, work out the byte layout against a couple of known speed readings, and forge a value past the badge's limiter threshold. No flag entry needed, it auto-solves the moment the frame lands.

#### Tool

`candump` / `cansend` (can-utils).

02

## Tester Present

Easy

The simplest valid UDS request there is. Real diagnostic tools send it periodically to keep a session alive. Here it's proof you can form a correctly structured request at all. The badge checks the actual service ID and sub-function, not just that something arrived on `0x7E0`. One correctly-formed frame is the whole interaction.

#### Tool

`cansend`. The badge parses raw ISO-TP single frames, so every byte string below starts with a length byte (the frame's PCI byte, `0N` where N is how many UDS bytes follow), then the UDS service ID and sub-function. Look up UDS service `0x3E` and its expected sub-function byte.

03

## Both Indicators

Easy

Get left and right turn signals active at the same moment. The badge's own UI can't do this, since it only ever cycles one at a time. Watch the indicator's CAN ID with the badge's own controls to see the bitfield only ever carries one bit, then forge a frame with both set together.

#### Tool

`candump` to observe, `cansend` to inject.

04

## Find the VIN

Medium

A real UDS `ReadDataByIdentifier` request for the vehicle's VIN. The 20-byte answer doesn't fit one CAN frame, so the ECU splits it over an **ISO-TP** First Frame and Consecutive Frames, and needs a Flow Control frame back from you to send the rest. Plain `cansend`/`candump` can't complete this alone. Recovering the VIN is only half the job. It's a real, correctly-formatted VIN belonging to an actual production car, and the flag is that car's name. Decode the VIN's manufacturer code (its first three characters) with any free VIN decoder to find it.

#### Tool

Something that speaks ISO-TP and handles the Flow Control handshake for you: `isotpsend`/`isotprecv`, or `python-can` with `can-isotp`, or `cantools`.

If a receive call hangs, check that your Flow Control is going out on the request ID, not the response ID. ISO-TP flow control always flows opposite the data.

05

## Seed & Key

Hard

A real UDS `SecurityAccess` seed/key handshake, gated behind an extended diagnostic session. Requesting a seed before opening that session gets refused outright. Once you're in the extended session, a request returns a fresh random seed each time. The badge gives two worked example seed/key pairs, and the relationship between each pair is identical across both. Find it, then apply it to your own live seed to compute the key.

#### Tool

`cansend`/`candump` and a calculator.

A wrong key invalidates the outstanding seed. Request a fresh one and try again. No lockout, but every wrong guess costs a round trip.
