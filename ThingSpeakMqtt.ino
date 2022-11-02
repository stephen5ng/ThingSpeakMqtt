#include "GsmClient.h"
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <ModularSensors.h>
#include <sensors/ProcessorStats.h>
#include <sensors/MaximDS3231.h>
#include <sensors/MeterHydros21.h>
#include "ThingSpeakPublisher.h"
#include "battery.h"
#include "modem.h"

const char *LoggerID = "logger"; // Logger ID and prefix for the name of the data file on SD card
const uint8_t loggingIntervalMinutes = 1;

// NOTE:  Daylight savings time will not be applied!  Please use standard time!
const int8_t timeZone = -8; // Pacific Standard Time

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
const char *mcuBoardVersion = "v0.5b"; // is only used for onboard battery voltage calculation,
                                       // "v0.5b" is safe to use on Mayfly v1.0 boards because the formula is the same
ProcessorStats mcuBoard(mcuBoardVersion);

MaximDS3231 ds3231(1);

Variable *variableList[] = {
    new ProcessorStats_Battery(&mcuBoard),
};
int variableCount = sizeof(variableList) / sizeof(variableList[0]);
VariableArray varArray;

Logger dataLogger(LoggerID, loggingIntervalMinutes, &varArray);

ThingSpeakPublisher TsMqtt;

void greenredflash(uint8_t numFlash = 4, uint8_t rate = 75)
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


void setup()
{
    // Start the primary serial connection
    Serial.begin(serialBaud);

    // Print a start-up note to the first serial port
    Serial.print(F("Now running "));
    Serial.print(F(" on Logger "));
    Serial.println(LoggerID);
    Serial.println();

    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);
    Serial.print(F("TinyGSM Library version "));
    Serial.println(TINYGSM_VERSION);
    Serial.println();

    // Start the serial connection with the modem
    modemSerial.begin(modemBaud);

    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);
    greenredflash(); // Blink the LEDs to show the board is on and starting up

    pinMode(20, OUTPUT); // for proper operation of the onboard flash memory chip's ChipSelect (Mayfly v1.0 and later)

    Logger::setLoggerTimeZone(timeZone);
    // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)
    Logger::setRTCTimeZone(0);

    dataLogger.attachModem(modem);
    modem.setModemLED(redLED);
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin,
                             greenLED);

    varArray.begin(variableCount, variableList);
    dataLogger.begin(LoggerID, loggingIntervalMinutes, &varArray);
    TsMqtt.begin(dataLogger, &modem.gsmClient,
                 THINGSPEAK_CHANNEL_ID,
                 MQTT_USERNAME,
                 MQTT_PASSWORD,
                 MQTT_CLIENT_ID);

    if (getBatteryVoltage(mcuBoard) > BATTERY_VOLTAGE_LOW)
    {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    modem.setModemWakeLevel(HIGH);  // ModuleFun Bee inverts the signal
    modem.setModemResetLevel(HIGH); // ModuleFun Bee inverts the signal
    Serial.println(F("Waking modem and setting Cellular Carrier Options..."));
    modem.modemWake();                 // NOTE:  This will also set up the modem
    modem.gsmModem.setBaud(modemBaud); // Make sure we're *NOT* auto-bauding!
    modem.gsmModem.setNetworkMode(NETWORK_MODE_GSM_ONLY);
    modem.gsmModem.setPreferredMode(PREFERRED_MODE_CAT_M);

    // Sync the clock if it isn't valid or we have battery to spare
    if (getBatteryVoltage(mcuBoard) > BATTERY_VOLTAGE_MODERATE || !dataLogger.isRTCSane())
    {
        // Synchronize the RTC with NIST
        // This will also set up the modem
        dataLogger.syncRTC();
    }

    // Create the log file, adding the default header to it
    // Do this last so we have the best chance of getting the time correct and
    // all sensor names correct
    // Writing to the SD card can be power intensive, so if we're skipping
    // the sensor setup we'll skip this too.
    if (getBatteryVoltage(mcuBoard) > BATTERY_VOLTAGE_LOW)
    {
        Serial.println(F("Setting up file on SD card"));
        dataLogger.turnOnSDcard(true);  // wait for card to settle after power up
        dataLogger.createLogFile(true); // write a new header
        dataLogger.turnOffSDcard(true); // wait for internal housekeeping after write
    }

    Serial.println(F("Putting processor to sleep\n"));
    dataLogger.systemSleep();
}

void loop()
{
    // At very low battery, just go back to sleep
    if (getBatteryVoltage(mcuBoard) < BATTERY_VOLTAGE_LOW)
    {
        dataLogger.systemSleep();
    }
    // At moderate voltage, log data but don't send it over the modem
    else if (getBatteryVoltage(mcuBoard) < BATTERY_VOLTAGE_MODERATE)
    {
        dataLogger.logData();
    }
    // If the battery is good, send the data to the world
    else
    {
        Serial.println(F("dataLogger.logDataAndPublish()\n"));
        dataLogger.logDataAndPublish();
    }
}
