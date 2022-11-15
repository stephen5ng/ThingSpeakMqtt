#ifndef modem_h
#define modem_h

#include <Arduino.h>
#include <modems/SIMComSIM7080.h>

#define modemSerial Serial1

const int32_t modemBaud = 9600; //  SIM7080 does auto-bauding by default, but for simplicity we set to 9600

void setupModem(SIMComSIM7080 &modem);

#endif
