#include <Arduino.h>
#include <modems/SIMComSIM7080.h>

#define modemSerial Serial1

const int32_t modemBaud = 9600; //  SIM7080 does auto-bauding by default, but for simplicity we set to 9600

// Modem Pins - Describe the physical pin connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply

const int8_t modemVccPin = 18;     // MCU pin controlling modem power --- Pin 18 is the power enable pin for the bee socket on Mayfly v1.0,
                                   //  use -1 if using Mayfly 0.5b or if the bee socket is constantly powered (ie you changed SJ18 on Mayfly1.0 to 3.3v)
const int8_t modemStatusPin = 19;  // MCU pin used to read modem status
const int8_t modemSleepRqPin = 23; // MCU pin for modem sleep/wake request

const char *apn = "hologram"; // APN connection name, typically Hologram unless you have a different provider's SIM card. Change as needed

SIMComSIM7080 modem7080(&modemSerial, modemVccPin, modemStatusPin,
                        modemSleepRqPin, apn);
SIMComSIM7080 modem = modem7080;
