#include "menu.h"
#include <stdint.h>

uint_fast16_t lmargin = 5;
static uint8_t tag_line[] = "bi0s Hardware";
static uint8_t lab_00_desc[] =
    "  UART Identification Method 1 using micro-controller pins and DMM "
    "(Digital Multimeter) conductivity test. The objective is to identify UART "
    "pinouts on the PCB board provided we have the knowledge of "
    "microcontroller UART pins, which can be found in the datasheet of the "
    "microcontroller.";
static uint8_t lab_01_desc[] =
    "  UART Identification Method 2 using DMM Voltage tests on the board "
    "pinouts. The objective is to identify UART pinouts on the PCB board when "
    "we do not have the knowledge of the microcontroller UART pins and the "
    "datasheet of the microcontroller.";
static uint8_t lab_02_desc[] =
    "  Once the UART port is identified, communicate with the target device to "
    "further analyze read/write access to the device. The objective is to "
    "understand the process of interfacing with the target device, identifying "
    "the correct baud rate that it uses to communicate and analyze the "
    "read/write access.";
static uint8_t lab_03_desc[] =
    "  JTAG Debug port identification method 1 using micro-controller pins and "
    "DMM (Digital Multimeter) conductivity test. The objective is to identify "
    "JTAG pinouts on the PCB board provided we have the knowledge of "
    "microcontroller JTAG pins, which can be found in the datasheet of the "
    "microcontroller.";
static uint8_t lab_04_desc[] =
    "  Automated JTAG Pin scanning and Identification using JtagEnum on "
    "Arduino. The objective is to understand the process and perform automated "
    "JTAG scanning using JtagEnum running on Arduino.";
static uint8_t lab_05_desc[] =
    "  Use the JTAG Debug port to access the microcontroller and extract the "
    "firmware. The objective is to understand how to connect over JTAG with "
    "the target board and how to extract data from the internal memory of the "
    "microcontroller.";
static uint8_t lab_06_desc[] =
    "  Find out the hardcoded password from the extracted firmware, modify it, "
    "write it back, and log in. The objective is to familiarize yourself with "
    "firmware extraction, analyze hard coded data, and patch the firmware on "
    "the fly.";
static uint8_t lab_07_desc[] =
    "  Use JTAG Debug port to manipulate GPIOs of microcontrollers and "
    "generate LED patterns with onboard LEDs. The objective is to understand "
    "how to control the GPIOs of the micro-controller over JTAG to change the "
    "behavior of the target as per your needs.";
static uint8_t lab_08_desc[] =
    "  To perform reconnaissance on EEPROM using the datasheet. The objective "
    "is to understand what information to obtain from the memory chip "
    "datasheet, which will help in further analysis.";
static uint8_t lab_09_desc[] =
    "  I2C communication sniffing using the Logic analyzer and bypass "
    "authentication. The objective is to get experience in interfacing with a "
    "Logic analyzer and reverse engineer the communication between the "
    "microcontroller and the I2C chip to identify any issues or "
    "vulnerabilities.";
static uint8_t lab_10_desc[] =
    "  I2C EEPROM memory dumping using Bus Pirate. The objective is to get "
    "familiar with the process of dumping I2C memory chip data.";
static uint8_t lab_11_desc[] =
    "  To perform reconnaissance on EEPROM using the datasheet. The objective "
    "is to understand what information to obtain from the memory chip "
    "datasheet, which will help in further analysis.";
static uint8_t lab_12_desc[] =
    "  SPI communication sniffing Using Logic analyzer and bypass "
    "authentication. The objective is to get experience in interfacing with a "
    "Logic analyzer and reverse engineer the communication between the "
    "micro-controller and the SPI chip to identify any issues or "
    "vulnerabilities.";
static uint8_t lab_13_desc[] =
    "  SPI Flash memory dumping using Bus Pirate. The objective is to get "
    "familiar with the process of dumping SPI memory chip data and extracting "
    "Linux-based firmware.";
static uint8_t lab_14_desc[] =
    "  The objective is to perform scanning for BLE devices in proximity.";
static uint8_t lab_15_desc[] =
    "  To enumerate the GATT services and characteristics of the BLE device. "
    "The objective is to understand what characteristics and services are and "
    "how do you get that information from a BLE device.";
static uint8_t lab_16_desc[] =
    "  Sniff the GATT protocol communication on Android. The objective is to "
    "understand how to sniff the GATT protocol communication, on an Android "
    "phone, with a BLE peripheral.";
static uint8_t lab_17_desc[] =
    "  To identify, write characteristic commands, their handles, and written "
    "values when you trigger an action on the BLE device. The objective is to "
    "understand how to identify and analyze which characteristics correspond "
    "to which commands/operations you perform on the device via the Android "
    "app.";
static uint8_t lab_18_desc[] =
    "  Control the behavior of the device by writing characteristic commands "
    "on the GATT protocol. The objective is to understand how to communicate "
    "with the device using tools instead of the actual app, send it specific "
    "values and analyze the behavior or control it remotely.";
static uint8_t lab_19_desc[] =
    "  Crack the LTK (Long Term Key) and decrypt the communication. The "
    "objective is to understand the process of cracking the LTK and what data "
    "is required for cracking.";
static uint8_t chall_01_desc[] = "Can youu unlock the door? ";

///////////////////////////////////////////////////////////

static uint8_t lab_00[] = " UART - Identification Method 1";
static uint8_t lab_01[] = " UART - Identification Method 2";
static uint8_t lab_02[] = " UART - Communication With Target";
static uint8_t lab_03[] = " JTAG - Identification Method 1";
static uint8_t lab_04[] = " JTAG - Identification Method 2";
static uint8_t lab_05[] = " JTAG - Microcontroller firmware extraction over JTAG";
static uint8_t lab_06[] = " JTAG - Microcontroller firmware patching over JTAG";
static uint8_t lab_07[] = " JTAG - Microcontroller peripheral access over JTAG";
static uint8_t lab_08[] = " I2C - chip reconnaissance";
static uint8_t lab_09[] = " I2C - communication sniffing to bypass authentication";
static uint8_t lab_10[] = " I2C - chip memory dumping";
static uint8_t lab_11[] = " SPI - chip recon";
static uint8_t lab_12[] = " SPI - communication sniffing to bypass authentication";
static uint8_t lab_13[] = " SPI - chip memory dumping and firmware extraction";
static uint8_t lab_14[] = " BLE - Recon";
static uint8_t lab_15[] = " BLE - GATT Services and Characteristics enumeration";
static uint8_t lab_16[] = " BLE - Sniffing GATT protocol communication (Android)";
static uint8_t lab_17[] = " BLE - Analyze GATT Protocol communication";
static uint8_t lab_18[] = " BLE - Controlling the Device";
static uint8_t lab_19[] = " BLE - Crack BLE LTK";
static uint8_t chall_01[] = " Password Lock (LEVEL I)";

////////////////////////////////////////////////////
// (0xA0|0x0A)=> (UART | COLOR)
// 0 : UART
// 1 : JTAG
// 2 : I2C
// 3 : SPI
// 4 : BLE
////////////////////////////////////////////////////

Menuitem menu[] = {
    {lab_00, lab_00_desc, (0x00 | 0x0A), LEN(lab_00), LEN(lab_00_desc), NULL},
    {lab_01, lab_01_desc, (0x00 | 0x0A), LEN(lab_01), LEN(lab_01_desc), NULL},
    {lab_02, lab_02_desc, (0x00 | 0x0A), LEN(lab_02), LEN(lab_02_desc), NULL},
    {lab_03, lab_03_desc, (0x10 | 0x0A), LEN(lab_03), LEN(lab_03_desc), NULL},
    {lab_04, lab_04_desc, (0x10 | 0x0A), LEN(lab_04), LEN(lab_04_desc), NULL},
    {lab_05, lab_05_desc, (0x10 | 0x0A), LEN(lab_05), LEN(lab_05_desc), NULL},
    {lab_06, lab_06_desc, (0x10 | 0x0A), LEN(lab_06), LEN(lab_06_desc), NULL},
    {lab_07, lab_07_desc, (0x10 | 0x0A), LEN(lab_07), LEN(lab_07_desc), NULL},
    {lab_08, lab_08_desc, (0x20 | 0x0A), LEN(lab_08), LEN(lab_08_desc), NULL},
    {lab_09, lab_09_desc, (0x20 | 0x0A), LEN(lab_09), LEN(lab_09_desc), NULL},
    {lab_10, lab_10_desc, (0x20 | 0x0A), LEN(lab_10), LEN(lab_10_desc), NULL},
    {lab_11, lab_11_desc, (0x30 | 0x0A), LEN(lab_11), LEN(lab_11_desc), NULL},
    {lab_12, lab_12_desc, (0x30 | 0x0A), LEN(lab_12), LEN(lab_12_desc), NULL},
    {lab_13, lab_13_desc, (0x30 | 0x0A), LEN(lab_13), LEN(lab_13_desc), NULL},
    {lab_14, lab_14_desc, (0x40 | 0x0A), LEN(lab_14), LEN(lab_14_desc), NULL},
    {lab_15, lab_15_desc, (0x40 | 0x0A), LEN(lab_15), LEN(lab_15_desc), NULL},
    {lab_16, lab_16_desc, (0x40 | 0x0A), LEN(lab_16), LEN(lab_16_desc), NULL},
    {lab_17, lab_17_desc, (0x40 | 0x0A), LEN(lab_17), LEN(lab_17_desc), NULL},
    {lab_18, lab_18_desc, (0x40 | 0x0A), LEN(lab_18), LEN(lab_18_desc), NULL},
    {lab_19, lab_19_desc, (0x40 | 0x0A), LEN(lab_19), LEN(lab_19_desc), NULL},
    {lab_19, lab_19_desc, (0x40 | 0x0A), LEN(lab_19), LEN(lab_19_desc), NULL},
    /* {lab_20, lab_20_desc, (0x40 | 0x0A), LEN(lab_20), LEN(lab_20_desc), NULL}, */
    {chall_01, chall_01_desc, (0xF0 | 0x0A), LEN(chall_01), LEN(chall_01_desc), NULL}};

// typedef struct {
//   uint8_t *title;
//   uint8_t *desc; // desciption
//   uint8_t property; // property = protocol(id) + |color id[2]|issolved[1]|isclickable[1] solved[1]|is_runabble[1] uint8_t
//   title_len; uint8_t desc_len; // not used
// } Menuitem;
