#include "WiFiManager.h"

void WiFiManager::connect(const char* ssid, const char* password) {
    Serial.println("Připojování k WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Připojování...");
    }
    Serial.println("WiFi připojeno!");
}
