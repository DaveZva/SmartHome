#include "SensorData.h"

void SensorData::parseMessage(const String& message) {
    int tempIndex = message.indexOf("T:");
    int humidityIndex = message.indexOf("H:");
    int soilIndex = message.indexOf("S:");
    int airIndex = message.indexOf("P:");
    int batteryIndex = message.indexOf("V:");

    if (tempIndex != -1 && humidityIndex != -1 && soilIndex != -1 && airIndex != -1 && batteryIndex != -1) {
        temperature = message.substring(tempIndex + 2, humidityIndex).toFloat();
        humidity = message.substring(humidityIndex + 2, soilIndex).toFloat();
        soilMoisture = message.substring(soilIndex + 2, airIndex).toFloat();
        airPressure = message.substring(airIndex + 2, batteryIndex).toFloat();
        batteryVoltage = message.substring(batteryIndex + 2).toFloat();
    }
}
