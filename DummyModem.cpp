#include "DummyModem.h"

bool DummyModem::connectInternet(uint32_t maxConnectionTime)
{
    _connected = SIMComSIM7080::connectInternet(maxConnectionTime);
    PRINTOUT(F("DummyModem connectInternet result: "), _connected);

    // Always return true even if internet connection failed, so that pump will run.
    return true;
}

bool DummyModem::isConnected() const
{
    return _connected;
}