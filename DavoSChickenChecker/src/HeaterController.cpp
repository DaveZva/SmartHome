#include "HeaterController.h"

HeaterController::HeaterController(uint8_t pin) : pin(pin) {}

void HeaterController::begin() {
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
}

void HeaterController::turnOn() {
    analogWrite(pin, 255);  // 100% výkon
    Serial.println("Topný drát zapnut na 100%!");
}

void HeaterController::turnOn75() {
    analogWrite(pin, 192);  // 75% výkon (PWM hodnoty: 0-255)
    Serial.println("Topný drát zapnut na 75%!");
}

void HeaterController::turnOn50() {
    analogWrite(pin, 128);  // 50% výkon (PWM hodnoty: 0-255)
    Serial.println("Topný drát zapnut na 50%!");
}

void HeaterController::turnOn25() {
    analogWrite(pin, 64);  // 25% výkon (PWM hodnoty: 0-255)
    Serial.println("Topný drát zapnut na 25%!");
}

void HeaterController::turnOff() {
    analogWrite(pin, 0);   // Vypne topný drát (0% PWM)
    Serial.println("Topný drát vypnut!");
}
