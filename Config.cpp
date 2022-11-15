#include <ArduinoJson.h>
#include <ModSensorDebugger.h>
#include <SdFat.h>
#include <TimeLib.h>

#include "Mayfly.h"

const char *filename = "config.txt"; // <- SD library uses 8.3 filenames

SdFat sdFat;

void setupConfig()
{
    Serial.print(F("Initializing sd library\n"));

    // Initialize SD library
    while (!sdFat.begin(sdCardSSPin, SPI_FULL_SPEED))
    {
        PRINTOUT(F("Failed to initialize SD library..."));
        delay(1000);
    }
}

// Prints the content of a file to the Serial
void printFile(const char *filename)
{
    PRINTOUT(F("printFile opening file"), filename);
    File file = sdFat.open(filename);

    if (!file)
    {
        PRINTOUT(F("Failed to read file"));
        return;
    }

    while (file.available())
    {
        Serial.print((char)file.read());
    }

    file.close();
}

// Allocate a JsonDocument
// Don't forget to change the capacity to match your requirements.
// Use arduinojson.org/v6/assistant to compute the capacity.
StaticJsonDocument<512> doc;

JsonDocument *loadConfiguration(const char *filename)
{
    setupConfig();
    printFile(filename);
    File file = sdFat.open(filename);

    DeserializationError error = deserializeJson(doc, file);
    if (error)
    {
        PRINTOUT(F("Failed to read configuration file: "), error.c_str());
    }

    file.close();
    return &doc;
}
