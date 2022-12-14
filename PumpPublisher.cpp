#include <PumpPublisher.h>

String formatEpochTime(time_t t)
{
    return String(String(year(t)) + "/" + String(month(t)) + "/" + String(day(t)) +
                  " " + String(hour(t), 2) + ":" + String(minute(t), 2) + ":" + String(second(t), 2));
}

// Logger converts everything to strings, so this utility converts back to float.
float PumpPublisher::getValue(int i)
{
    return _baseLogger->getValueStringAtI(i).toFloat();
}

// Returns the index of the given variable.
int PumpPublisher::findVariable(Variable *v)
{
    for (int i = 0; i < _baseLogger->getArrayVarCount(); i++)
    {
        if (_baseLogger->getParentSensorNameAtI(i) == v->getParentSensorName())
        {
            return i;
        }
    }
    PRINTOUT(F("ERROR: Can't find "), v->getParentSensorName());
    return 0;
}

void PumpPublisher::maybeStartPump(time_t now)
{
    // Pump already started.
    if (_pump_started_time > 0 || _pump_stopped_time > 0) {
        return;
    }

    if (now < _storm_start_time)
    {
        PRINTOUT(F("Waiting for storm to start. start_time:"), formatEpochTime(_storm_start_time),
                 F(", now:"), formatEpochTime(now));
        return;
    }

    const float current_flow_level = getValue(_water_level_i);
    if (current_flow_level < _min_flow_level)
    {
        PRINTOUT(
            F("Waiting for water level. min_flow_level:"), _min_flow_level,
            F(", current_flow_level:"), current_flow_level);
        return;
    }

    const float current_salinity = getValue(_salinity_i);
    if (current_salinity > _max_salinity)
    {
        PRINTOUT(
            F("Waiting for salinity. _max_salinity:"), _max_salinity,
            F(", current_salinity:"), current_salinity);
        return;
    }
    _pump_started_time = now;
    PRINTOUT(F("PUMPING!")); // Add pumping code here.
}

void PumpPublisher::maybeStopPump(time_t now)
{
    if (_pump_started_time == 0 || _pump_stopped_time != 0)
    {
        return;
    }
    PRINTOUT(F("Pump running for: "), now - _pump_started_time);

    if (now - _pump_started_time > 60)
    {
        PRINTOUT(F("Turning off pump!"));
        _pump_stopped_time = now;
    }
}

void PumpPublisher::pump()
{
    time_t now = rtc.now().getEpoch();
    maybeStartPump(now);
    maybeStopPump(now);
}

String PumpPublisher::getEndpoint(void)
{
    return String("PUMP AT PIN: XXX");
}

void PumpPublisher::begin(Logger &baseLogger, Client *inClient)
{
    dataPublisher::begin(baseLogger, inClient);
    _water_level_i = findVariable(_water_level);
    _salinity_i = findVariable(_salinity);
}