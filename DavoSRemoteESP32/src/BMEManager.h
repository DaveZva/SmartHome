#ifndef BME_MANAGER_H
#define BME_MANAGER_H

#include <Wire.h>
#include <Adafruit_BME280.h>

class BMEManager {
public:
    BMEManager();
    bool begin();  // Změněno na bool
    float readTemperature();
    float readHumidity();
    float readPressure();
    void sleep();
    
private:
    Adafruit_BME280 bme;
    bool sensorAvailable = false; // Přidán příznak
};

#endif
