#include "GsmClient.h"
#include "modem.h"

// Modem Pins - Describe the physical pin connection of your modem to your board
// NOTE:  Use -1 for pins that do not apply
const int8_t modemVccPin = 18;     // MCU pin controlling modem power --- Pin 18 is the power enable pin for the bee socket on Mayfly v1.0,
                                   //  use -1 if using Mayfly 0.5b or if the bee socket is constantly powered (ie you changed SJ18 on Mayfly1.0 to 3.3v)
const int8_t modemStatusPin = 19;  // MCU pin used to read modem status
const int8_t modemSleepRqPin = 23; // MCU pin for modem sleep/wake request

const char *apn = "hologram"; // APN connection name, typically Hologram unless you have a different provider's SIM card. Change as needed
SIMComSIM7080 modem(&modemSerial, modemVccPin, modemStatusPin,
                    modemSleepRqPin, apn);

void setupModem(SIMComSIM7080 &modem)
{
    modem.setModemWakeLevel(HIGH);  // ModuleFun Bee inverts the signal
    modem.setModemResetLevel(HIGH); // ModuleFun Bee inverts the signal
    Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
    modem.modemWake();                 // NOTE:  This will also set up the modem
    modem.gsmModem.setBaud(modemBaud); // Make sure we're *NOT* auto-bauding!
    modem.gsmModem.setNetworkMode(NETWORK_MODE_GSM_ONLY);
    modem.gsmModem.setPreferredMode(PREFERRED_MODE_CAT_M);
}