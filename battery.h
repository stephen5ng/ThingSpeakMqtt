#ifndef battery_h
#define battery_h


// Note:  Please change these battery voltages to match your battery
#define BATTERY_VOLTAGE_LOW 3.4
#define BATTERY_VOLTAGE_MODERATE 3.55

float getBatteryVoltage(ProcessorStats &mcuBoard);

#endif