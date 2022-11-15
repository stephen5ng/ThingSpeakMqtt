#ifndef MAYFLY_H
#define MAYFLY_H

#include <stdint.h>
#include <sensors/ProcessorStats.h>

extern const char *UUIDs[];

// NOTE:  Use -1 for pins that do not apply
const int32_t serialBaud = 115200;
const int8_t greenLED = 8;
const int8_t redLED = 9;
const int8_t buttonPin = 21;
const int8_t wakePin = 31; // MCU interrupt/alarm pin to wake from sleep
// Mayfly 0.x D31 = A7
// Set the wake pin to -1 if you do not want the main processor to sleep.
// In a SAMD system where you are using the built-in rtc, set wakePin to 1

const int8_t sdCardPwrPin = -1;   // MCU SD card power pin
const int8_t sdCardSSPin = 12;    // SD card chip select/slave select pin
const int8_t sensorPowerPin = 22; // MCU pin controlling main sensor power

// Create the main processor chip "sensor" - for general metadata
extern const char *mcuBoardVersion;
void greenredflash(uint8_t numFlash, uint8_t rate);

// Note:  Please change these battery voltages to match your battery
#define BATTERY_VOLTAGE_LOW 3.4
#define BATTERY_VOLTAGE_MODERATE 3.55

float getBatteryVoltage(ProcessorStats &mcuBoard);

#endif
