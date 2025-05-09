#include "ChickenLightController.h"

ChickenLightController::ChickenLightController(uint8_t pin) : pin(pin) {}

void ChickenLightController::begin() {
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
}

void ChickenLightController::turnOn() {
    analogWrite(pin, 255);  // 100% výkon
    Serial.println("Světlo zapnuto na 100%!");
}

void ChickenLightController::turnOn75() {
    analogWrite(pin, 192);
    Serial.println("Světlo zapnuto na 75%!");
}

void ChickenLightController::turnOn50() {
    analogWrite(pin, 128); 
    Serial.println("Světlo zapnuto na 50%!");
}

void ChickenLightController::turnOn25() {
    analogWrite(pin, 64);
    Serial.println("Světlo zapnuto na 25%!");
}

void ChickenLightController::turnOff() {
    analogWrite(pin, 0);   // Vypne světlo
    Serial.println("Světlo vypnuto!");
}
