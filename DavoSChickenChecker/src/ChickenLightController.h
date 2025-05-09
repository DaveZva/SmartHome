#ifndef CHICKENLIGHTCONTROLLER_H
#define CHICKENLIGHTCONTROLLER_H

#include <Arduino.h>

class ChickenLightController {
public:
    ChickenLightController(uint8_t pin);
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
