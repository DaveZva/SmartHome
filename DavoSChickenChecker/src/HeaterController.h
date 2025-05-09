#ifndef HEATERCONTROLLER_H
#define HEATERCONTROLLER_H

#include <Arduino.h>
#include <ArduinoHttpClient.h>

class HeaterController {
public:
    HeaterController(uint8_t pin);
    void begin();
    void turnOn();
    void turnOn75();
    void turnOn50();
    void turnOn25();
    void turnOff();

private:
    uint8_t pin;
};

#endif
