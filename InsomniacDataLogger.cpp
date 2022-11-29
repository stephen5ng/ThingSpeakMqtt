#include "InsomniacDataLogger.h"

void InsomniacDataLogger::enableSleep(bool enable)
{
    if (!enable)
    {
        _mcuWakePinSave = _mcuWakePin;
        _mcuWakePin = -1;
    }
    else
    {
        _mcuWakePin = _mcuWakePinSave;
    }
}