#include <sensors/ProcessorStats.h>

// Reads the battery voltage
// NOTE: This will actually return the battery level from the previous update!
float getBatteryVoltage(ProcessorStats &mcuBoard)
{
    if (mcuBoard.sensorValues[0] == -9999)
        mcuBoard.update();
    return mcuBoard.sensorValues[0];
}