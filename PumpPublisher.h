#ifndef PUMPPUBLISHER_H
#define PUMPPUBLISHER_H

#include <stdint.h>
#include <dataPublisherBase.h>
#include <TimeLib.h>

class PumpPublisher : public dataPublisher
{
private:
    int _water_level_i;
    int _salinity_i;
    Variable *_water_level;
    Variable *_salinity;
    const time_t _storm_start_time;
    const double _min_flow_level;
    const double _max_salinity;

    time_t _pump_started_time;

    float getValue(int i);
    int findVariable(Variable *v);

    void maybeStopPump(time_t now);
    void maybeStartPump(time_t now);

public:
    PumpPublisher(Variable *water_level, Variable *salinity,
                  time_t storm_start_time,
                  float min_flow_level, float max_salinity)
        : dataPublisher(),
          _water_level(water_level),
          _salinity(salinity),
          _storm_start_time(storm_start_time),
          _min_flow_level(min_flow_level),
          _max_salinity(max_salinity),
          _pump_started_time(0)
    {
    }

    int16_t publishData(Client *outClient);

    String getEndpoint(void) override;

    void begin(Logger &baseLogger, Client *inClient);
};

#endif