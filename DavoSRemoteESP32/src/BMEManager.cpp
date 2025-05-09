#include "BMEManager.h"

BMEManager::BMEManager() {
  // Konstruktor může být prázdný
}

bool BMEManager::begin() {    
    if (!bme.begin(0x76)) {  // Používáme výchozí adresu 0x76
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        sensorAvailable = false;
        return false;  // Vrátíme false místo blokování programu
    }
    Serial.println("BME280 sensor found.");
    sensorAvailable = true;
    return true;
}

float BMEManager::readTemperature() {
    return sensorAvailable ? bme.readTemperature() : NAN; 
}

float BMEManager::readHumidity() {
    return sensorAvailable ? bme.readHumidity() : NAN;
}

float BMEManager::readPressure() {
    return sensorAvailable ? (bme.readPressure() / 100.0F) : NAN;
}

void BMEManager::sleep() {
    bme.setSampling(Adafruit_BME280::MODE_SLEEP,
                    Adafruit_BME280::SAMPLING_NONE,
                    Adafruit_BME280::SAMPLING_NONE,
                    Adafruit_BME280::SAMPLING_NONE,
                    Adafruit_BME280::FILTER_OFF);
  }
