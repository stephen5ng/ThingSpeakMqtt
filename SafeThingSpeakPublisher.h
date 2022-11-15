#ifndef SAFETHINGSPEAKPUBLISHER_H
#define SAFETHINGSPEAKPUBLISHER_H

// The DummyModem pretends it's always connected so that pumping happens even if the internet is down.
// The SafeThingSpeakPublisher won't publish unless the DummyModem has made a real connection.

#include "DummyModem.h"
#include "ThingSpeakPublisher.h"

class SafeThingSpeakPublisher : public ThingSpeakPublisher
{
private:
    DummyModem &_modem;

public:
    SafeThingSpeakPublisher(DummyModem &modem) : _modem(modem) {}
    int16_t publishData(Client *outClient) override;
};

#endif