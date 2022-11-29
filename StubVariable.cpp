#include "StubVariable.h"

class StubSensor : public Sensor
{
private:
    const float *_values;
    const uint8_t _size;
    const uint16_t _update_interval_ms = 15000;

public:
    explicit StubSensor(const char *name, const float *values, int size)
        : _values(values), _size(size)
    {
        _sensorName = name;
    }

    bool addSingleMeasurementResult(void) override;
};

bool StubSensor::addSingleMeasurementResult(void)
{
    unsigned int index = (millis() / 15000) % _size;
    PRINTOUT(F("StubSensor adding Test Sensor result: "), _sensorName,
             "[", index, "] : ", _values[index]);

    // PRINTOUT("getting next_value: ");
    verifyAndAddMeasurementResult(0, _values[index++]);
    return true;
}

StubVariable::StubVariable(const char *name, const float *values, int size, const char *uuid)
{
    isCalculated = false;
    String *n = new String(name);
    n->concat("_SENSOR");
    attachSensor(new StubSensor(n->c_str(), values, size));
    setVarName(name);
    setVarCode(name);
}