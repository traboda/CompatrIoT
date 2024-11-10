#include "menu.h"

uint_fast16_t lmargin = 5;

static uint8_t lab_0_Desc[]  ="  UART Identification Method 1 using micro-controller pins and DMM (Digital Multimeter) conductivity test. The objective is to identify UART pinouts on the PCB board provided we have the knowledge of microcontroller UART pins, which can be found in the datasheet of the microcontroller.";
static uint8_t lab_1_Desc[]  ="  UART Identification Method 2 using DMM Voltage tests on the board pinouts. The objective is to identify UART pinouts on the PCB board when we do not have the knowledge of the microcontroller UART pins and the datasheet of the microcontroller.";
static uint8_t lab_2_Desc[]  ="  Once the UART port is identified, communicate with the target device to further analyze read/write access to the device. The objective is to understand the process of interfacing with the target device, identifying the correct baud rate that it uses to communicate and analyze the read/write access.";
static uint8_t lab_3_Desc[]  ="  JTAG Debug port identification method 1 using micro-controller pins and DMM (Digital Multimeter) conductivity test. The objective is to identify JTAG pinouts on the PCB board provided we have the knowledge of microcontroller JTAG pins, which can be found in the datasheet of the microcontroller.";
static uint8_t lab_4_Desc[]  ="  Automated JTAG Pin scanning and Identification using JtagEnum on Arduino. The objective is to understand the process and perform automated JTAG scanning using JtagEnum running on Arduino.";
static uint8_t lab_5_Desc[]  ="  Use the JTAG Debug port to access the microcontroller and extract the firmware. The objective is to understand how to connect over JTAG with the target board and how to extract data from the internal memory of the microcontroller.";
static uint8_t lab_6_Desc[]  ="  Find out the hardcoded password from the extracted firmware, modify it, write it back, and log in. The objective is to familiarize yourself with firmware extraction, analyze hard coded data, and patch the firmware on the fly.";
static uint8_t lab_7_Desc[]  ="  Use JTAG Debug port to manipulate GPIOs of microcontrollers and generate LED patterns with onboard LEDs. The objective is to understand how to control the GPIOs of the micro-controller over JTAG to change the behavior of the target as per your needs.";
static uint8_t lab_8_Desc[]  ="  To perform reconnaissance on EEPROM using the datasheet. The objective is to understand what information to obtain from the memory chip datasheet, which will help in further analysis.";
static uint8_t lab_9_Desc[] ="  I2C communication sniffing using the Logic analyzer and bypass authentication. The objective is to get experience in interfacing with a Logic analyzer and reverse engineer the communication between the microcontroller and the I2C chip to identify any issues or vulnerabilities.";
static uint8_t lab_10_Desc[] ="  I2C EEPROM memory dumping using Bus Pirate. The objective is to get familiar with the process of dumping I2C memory chip data.";
static uint8_t lab_11_Desc[] ="  To perform reconnaissance on EEPROM using the datasheet. The objective is to understand what information to obtain from the memory chip datasheet, which will help in further analysis.";
static uint8_t lab_12_Desc[] ="  SPI communication sniffing Using Logic analyzer and bypass authentication. The objective is to get experience in interfacing with a Logic analyzer and reverse engineer the communication between the micro-controller and the SPI chip to identify any issues or vulnerabilities.";
static uint8_t lab_13_Desc[] ="  SPI Flash memory dumping using Bus Pirate. The objective is to get familiar with the process of dumping SPI memory chip data and extracting Linux-based firmware.";
static uint8_t lab_14_Desc[] ="  The objective is to perform scanning for BLE devices in proximity.";
static uint8_t lab_15_Desc[] ="  To enumerate the GATT services and characteristics of the BLE device. The objective is to understand what characteristics and services are and how do you get that information from a BLE device.";
static uint8_t lab_16_Desc[] ="  Sniff the GATT protocol communication on Android. The objective is to understand how to sniff the GATT protocol communication, on an Android phone, with a BLE peripheral.";
static uint8_t lab_17_Desc[] ="  To identify, write characteristic commands, their handles, and written values when you trigger an action on the BLE device. The objective is to understand how to identify and analyze which characteristics correspond to which commands/operations you perform on the device via the Android app.";
static uint8_t lab_18_Desc[] ="  Control the behavior of the device by writing characteristic commands on the GATT protocol. The objective is to understand how to communicate with the device using tools instead of the actual app, send it specific values and analyze the behavior or control it remotely.";
static uint8_t lab_19_Desc[] ="  Crack the LTK (Long Term Key) and decrypt the communication. The objective is to understand the process of cracking the LTK and what data is required for cracking.";

///////////////////////////////////////////////////////////

static uint8_t lab_0[]  =" UART - Identification Method 1";
static uint8_t lab_1[]  =" UART - Identification Method 2";
static uint8_t lab_2[]  =" UART - Communication With Target";
static uint8_t lab_3[]  =" JTAG - Identification Method 1";
static uint8_t lab_4[]  =" JTAG - Identification Method 2";
static uint8_t lab_5[]  =" JTAG - Microcontroller firmware extraction over JTAG";
static uint8_t lab_6[]  =" JTAG - Microcontroller firmware patching over JTAG";
static uint8_t lab_7[]  =" JTAG - Microcontroller peripheral access over JTAG";
static uint8_t lab_8[]  =" I2C - chip reconnaissance";
static uint8_t lab_9[]  =" I2C - communication sniffing to bypass authentication";
static uint8_t lab_10[] =" I2C - chip memory dumping";
static uint8_t lab_11[] =" SPI - chip recon";
static uint8_t lab_12[] =" SPI - communication sniffing to bypass authentication";
static uint8_t lab_13[] =" SPI - chip memory dumping and firmware extraction";
static uint8_t lab_14[] =" BLE - Recon";
static uint8_t lab_15[] =" BLE - GATT Services and Characteristics enumeration";
static uint8_t lab_16[] =" BLE - Sniffing GATT protocol communication (Android)";
static uint8_t lab_17[] =" BLE - Analyze GATT Protocol communication";
static uint8_t lab_18[] =" BLE - Controlling the Device";
static uint8_t lab_19[] =" BLE - Crack BLE LTK";

////////////////////////////////////////////////////
// (0xA0|0x0A)=> (UART | COLOR) 
// 0 : UART
// 1 : JTAG
// 2 : I2C
// 3 : SPI
// 4 : BLE
////////////////////////////////////////////////////

Menuitem menu[] = {
    {lab_0 , lab_0_Desc,  (0x00|0x0A), LEN(lab_0) , LEN(lab_0_Desc)},
    {lab_1 , lab_1_Desc,  (0x00|0x0A), LEN(lab_1) , LEN(lab_1_Desc)},
    {lab_2 , lab_2_Desc,  (0x00|0x0A), LEN(lab_2) , LEN(lab_2_Desc)},
    {lab_3 , lab_3_Desc,  (0x10|0x0A), LEN(lab_3) , LEN(lab_3_Desc)},
    {lab_4 , lab_4_Desc,  (0x10|0x0A), LEN(lab_4) , LEN(lab_4_Desc)},
    {lab_5 , lab_5_Desc,  (0x10|0x0A), LEN(lab_5) , LEN(lab_5_Desc)},
    {lab_6 , lab_6_Desc,  (0x10|0x0A), LEN(lab_6) , LEN(lab_6_Desc)},
    {lab_7 , lab_7_Desc,  (0x10|0x0A), LEN(lab_7) , LEN(lab_7_Desc)},
    {lab_8 , lab_8_Desc,  (0x20|0x0A), LEN(lab_8) , LEN(lab_8_Desc)},
    {lab_9 , lab_9_Desc,  (0x20|0x0A), LEN(lab_9) , LEN(lab_9_Desc)},
    {lab_10, lab_10_Desc, (0x20|0x0A), LEN(lab_10), LEN(lab_10_Desc)},
    {lab_11, lab_11_Desc, (0x30|0x0A), LEN(lab_11), LEN(lab_11_Desc)},
    {lab_12, lab_12_Desc, (0x30|0x0A), LEN(lab_12), LEN(lab_12_Desc)},
    {lab_13, lab_13_Desc, (0x30|0x0A), LEN(lab_13), LEN(lab_13_Desc)},
    {lab_14, lab_14_Desc, (0x40|0x0A), LEN(lab_14), LEN(lab_14_Desc)},
    {lab_15, lab_15_Desc, (0x40|0x0A), LEN(lab_15), LEN(lab_15_Desc)},
    {lab_16, lab_16_Desc, (0x40|0x0A), LEN(lab_16), LEN(lab_16_Desc)},
    {lab_17, lab_17_Desc, (0x40|0x0A), LEN(lab_17), LEN(lab_17_Desc)},
    {lab_18, lab_18_Desc, (0x40|0x0A), LEN(lab_18), LEN(lab_18_Desc)},
    {lab_19, lab_19_Desc, (0x40|0x0A), LEN(lab_19), LEN(lab_19_Desc)}};

// typedef struct {
//   uint8_t *title;
//   uint8_t *desc; // desciption
//   uint8_t property; // property = protocol(id) + color(id)
//   uint8_t title_len;
//   uint8_t desc_len; // not used
// } Menuitem;