#include "SafeThingSpeakPublisher.h"

int16_t SafeThingSpeakPublisher::publishData(Client *outClient)
{
    if (_modem.isConnected())
    {
        PRINTOUT(F("SafeThingSpeakPublisher: good connection, publishing."));
        return ThingSpeakPublisher::publishData(outClient);
    }
    PRINTOUT(F("SafeThingSpeakPublisher: no connection, skipping publish."));

    return 0;
}
