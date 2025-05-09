#include <Arduino.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "LoRaManager.h"
#include "SensorData.h"
#include "OTAUpdater.h"
#include "config.h" // Konfigurační soubor s citlivými daty

WiFiManager wifiManager;
MQTTManager mqttManager;
LoRaManager loraManager;
SensorData sensorData;
OTAUpdater otaUpdater("ESP32-Main");


bool mqttConnected = false;

void setup() {
    Serial.begin(115200);
    Serial.println("Inicializace systému...");

    // Připojení k WiFi
    wifiManager.connect(ssid, password);

    // Snížení výkonu WiFi pro úsporu energie
    WiFi.setTxPower(WIFI_POWER_MINUS_1dBm); 

    // Nastavení MQTT
    mqttManager.setup(mqtt_server, mqtt_port, mqtt_user, mqtt_password);

    // Inicializace LoRa
    loraManager.setup();
    otaUpdater.begin();
}

void loop() {
    // Kontrola a obnova MQTT připojení
    if (!mqttConnected) {
        mqttConnected = mqttManager.connect("ESP32Client", "mqtt_user", "mqtt_password", "home/garden/irrigation_command");
        if (mqttConnected) {
            Serial.println("MQTT připojeno!");
        } else {
            Serial.println("MQTT připojení selhalo, pokračuji bez MQTT...");
            delay(5000); // Čekání před opakováním pokusu
        }
    }

    // Zpracování MQTT zpráv
    mqttManager.loop();
    otaUpdater.handle();

   // Čtení zpráv z LoRa
    String message = loraManager.readMessage();
    if (!message.isEmpty()) {
        Serial.println("Přijata LoRa zpráva: " + message);

        // Parsování přijatých dat
        sensorData.parseMessage(message);

        // Publikování dat do MQTT (pouze pokud je MQTT připojeno)
        if (mqttConnected) {
            mqttManager.publish("home/garden/temperature", sensorData.temperature);
            mqttManager.publish("home/garden/humidity", sensorData.humidity);
            mqttManager.publish("home/garden/soil_moisture", sensorData.soilMoisture);
            mqttManager.publish("home/garden/air_pressure", sensorData.airPressure);
        } else {
            Serial.println("MQTT není připojeno, data nelze odeslat.");
        }

        delay(300); // Pauza před ACK
        loraManager.sendMessage("ACK");
        Serial.println("Odesláno potvrzení zpět.");
    }

    delay(100); // Malé zpoždění pro stabilitu smyčky
}
