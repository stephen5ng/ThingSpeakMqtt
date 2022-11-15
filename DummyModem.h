#ifndef DUMMYMODEM_H
#define DUMMYMODEM_H

#include <Arduino.h>
#include <modems/SIMComSIM7080.h>

class DummyModem : public SIMComSIM7080
{
private:
    bool _connected;

public:
    DummyModem(Stream *modemStream, int8_t powerPin, int8_t statusPin,
               int8_t modemSleepRqPin, const char *apn)
        : SIMComSIM7080(modemStream, powerPin, statusPin, modemSleepRqPin, apn) {}

    bool connectInternet(uint32_t maxConnectionTime);
    bool isConnected() const;
};

extern DummyModem modem;

#endif