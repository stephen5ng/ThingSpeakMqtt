#include <SPI.h>
#include "GsmClient.h"
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <ModularSensors.h>
#include <sensors/ProcessorStats.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "Config.h"
#include "DummyModem.h"
#include "Mayfly.h"
#include "Modem.h"
#include "PumpPublisher.h"
#include "SafeThingSpeakPublisher.h"
#include "StubVariable.h"
#include "ThingSpeakPublisher.h"

using namespace std;

const char *LoggerID = "logger"; // Logger ID and prefix for the name of the data file on SD card
const uint8_t loggingIntervalMinutes = 1;

ProcessorStats mcuBoard(mcuBoardVersion);

const float water_level_values[] = {1.0, 2, 8, 13, 127};
Variable *water_level = new StubVariable("WATER_LEVEL",
                                         water_level_values,
                                         sizeof(water_level_values) / sizeof(water_level_values[0]));

const float salinity_values[] = {3, 4, 5, 5, 4, 3};
Variable *salinity = new StubVariable("SALINITY", salinity_values,
                                      sizeof(salinity_values) / sizeof(salinity_values[0]));

VariableArray varArray;

Logger dataLogger(LoggerID, loggingIntervalMinutes, &varArray);

SafeThingSpeakPublisher tsMqtt(modem);

void setup()
{
    // Start the primary serial connection
    Serial.begin(serialBaud);

    PRINTOUT(F("Now running on Logger "), LoggerID, "\n");

    Serial.print(F("Using ModularSensors Library version "));
    Serial.println(MODULAR_SENSORS_VERSION);
    Serial.print(F("TinyGSM Library version "));
    Serial.println(TINYGSM_VERSION);

    JsonDocument *json = loadConfiguration("/config.txt");

    static Variable *variableList[] = {
        water_level,
        salinity,
    };
    int variableCount = sizeof(variableList) / sizeof(variableList[0]);

    modemSerial.begin(modemBaud);

    pinMode(greenLED, OUTPUT);
    digitalWrite(greenLED, LOW);
    pinMode(redLED, OUTPUT);
    digitalWrite(redLED, LOW);
    greenredflash(4, 75); // Blink the LEDs to show the board is on and starting up

    pinMode(20, OUTPUT); // for proper operation of the onboard flash memory chip's ChipSelect (Mayfly v1.0 and later)

    Logger::setRTCTimeZone(0); // It is STRONGLY RECOMMENDED that you set the RTC to be in UTC (UTC+0)

    dataLogger.attachModem(modem);
    modem.setModemLED(redLED);
    dataLogger.setLoggerPins(wakePin, sdCardSSPin, sdCardPwrPin, buttonPin, greenLED);

    PumpPublisher *pumpPublisher = new PumpPublisher(water_level, salinity, (*json)["storm_start_time"], 12, 4);

    varArray.begin(variableCount, variableList, UUIDs);
    dataLogger.begin(LoggerID, loggingIntervalMinutes, &varArray);
    tsMqtt.begin(dataLogger, &modem.gsmClient,
                 THINGSPEAK_CHANNEL_ID,
                 MQTT_USERNAME,
                 MQTT_PASSWORD,
                 MQTT_CLIENT_ID);
    pumpPublisher->begin(dataLogger, &modem.gsmClient);

    if (getBatteryVoltage(mcuBoard) > BATTERY_VOLTAGE_LOW)
    {
        Serial.println(F("Setting up sensors..."));
        varArray.setupSensors();
    }

    setupModem(modem);

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
        dataLogger.logDataAndPublish();
    }
}
