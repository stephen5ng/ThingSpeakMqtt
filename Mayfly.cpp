#include <Arduino.h>
#include "Mayfly.h"

const char *mcuBoardVersion = "v0.5b"; // is only used for onboard battery voltage calculation,
                                       // "v0.5b" is safe to use on Mayfly v1.0 boards because the formula is the same

const char *UUIDs[] = {
    "12345678-abcd-1234-ef00-1234567890a0",
    "12345678-abcd-1234-ef00-1234567890a1",
    "12345678-abcd-1234-ef00-1234567890a2",
    "12345678-abcd-1234-ef00-1234567890a3",
    "12345678-abcd-1234-ef00-1234567890a4",
    "12345678-abcd-1234-ef00-1234567890a5",
    "12345678-abcd-1234-ef00-1234567890a6",
    "12345678-abcd-1234-ef00-1234567890a7",
    "12345678-abcd-1234-ef00-1234567890a8",
    "12345678-abcd-1234-ef00-1234567890a9",
    "12345678-abcd-1234-ef00-1234567890aa",
    "12345678-abcd-1234-ef00-1234567890ab",
    "12345678-abcd-1234-ef00-1234567890ac",
    "12345678-abcd-1234-ef00-1234567890ad",
    "12345678-abcd-1234-ef00-1234567890ae",
    "12345678-abcd-1234-ef00-1234567890af",
};

void greenredflash(uint8_t numFlash, uint8_t rate)
{
    for (uint8_t i = 0; i < numFlash; i++)
    {
        digitalWrite(greenLED, HIGH);
        digitalWrite(redLED, LOW);
        delay(rate);
        digitalWrite(greenLED, LOW);
        digitalWrite(redLED, HIGH);
        delay(rate);
    }
    digitalWrite(redLED, LOW);
}

// Reads the battery voltage
// NOTE: This will actually return the battery level from the previous update!
float getBatteryVoltage(ProcessorStats &mcuBoard)
{
    if (mcuBoard.sensorValues[0] == -9999)
        mcuBoard.update();
    return mcuBoard.sensorValues[0];
}