#ifndef INSOMNIACDATALOGGER_H
#define INSOMNIACDATALOGGER_H

#include <stdint.h>
#include <dataPublisherBase.h>

class InsomniacDataLogger : public Logger
{
private:
    int8_t _mcuWakePinSave;

public:
    InsomniacDataLogger(const char *loggerID, uint16_t loggingIntervalMinutes,
                        VariableArray *inputArray) : Logger(loggerID, loggingIntervalMinutes, inputArray) {}
    void enableSleep(bool enable);
};

#endif