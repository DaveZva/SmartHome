#ifndef SENSORDATA_H
#define SENSORDATA_H

#include <Arduino.h>

class SensorData {
public:
    float temperature = 0.0;
    float humidity = 0.0;
    float soilMoisture = 0.0;
    float airPressure = 0.0;
    float batteryVoltage = 0.0;

    void parseMessage(const String& message);
};

#endif
