#include <Arduino.h>
#include <PubSubClient.h>
#include "WiFiManager.h"
#include "MQTTManager.h"
#include "OTAUpdater.h"
#include "HeaterController.h"
#include "ChickenLightController.h"
#include "InfluxDBManager.h"
#include "config.h" 

// Set pin for heater
#define HEATER_PIN 2
#define LIGHT_PIN 3

//State of heater
bool heaterState = false;
bool currrentHeaterState;
bool lightState = false;
bool currrentLightState;

unsigned long previousMillis = 0; 
const long interval = 3600000; // 1 hodina

WiFiManager wifiManager;
WiFiClient espClient;
PubSubClient client(espClient);
HeaterController heater(HEATER_PIN);  // Pin D2 pro topný drát
ChickenLightController light(LIGHT_PIN);  // Pin D3 pro světlo
MQTTManager mqttManager(client, heater, light);  // MQTTManager předává referenci na HeaterController
OTAUpdater otaUpdater("ESP32C3-Heater");
InfluxDBManager influxDB(influxUrl, influxBucket, influxUsername, influxPassword);

void setup() {
    Serial.begin(115200);
    // Připojení k WiFi
    wifiManager.connect(ssid, password);
    // Nastavení MQTT klienta
    mqttManager.setup(mqtt_server, mqtt_port, mqtt_user, mqtt_password);

    // Připojení k MQTT
    if (mqttManager.connect("ESP32C3_Client", mqtt_user, mqtt_password, "home/heater/set")) {
        mqttManager.publish("home/garden/status", "Připojeno!");
        client.subscribe("home/light/set");  // Přihlášení k odběru druhého tématu
    }

    // Inicializace topného drátu
    heater.begin();

    // Inicializace světla
    light.begin();

    // Spuštění OTA aktualizací
    otaUpdater.begin();

    influxDB.getLastHeaterState(heaterState);  // Zkontroluje poslední stav topení
    // Nastavení topení podle stavu z InfluxDB
    if (heaterState) {
        heater.turnOn();
        currrentHeaterState = true;
    } else {
        heater.turnOff();  // Pokud byl poslední stav "OFF", vypneme topení
        currrentHeaterState = false;
    }

    influxDB.getLastLightState(lightState);  // Zkontroluje poslední stav světla
    // Nastavení světla podle stavu z InfluxDB
    if (lightState) {
        light.turnOn();
        currrentLightState = true;
    } else {
        light.turnOff();  // Pokud byl poslední stav "OFF", vypneme světlo
        currrentLightState = false;
    }
}

void loop() {
    // MQTT smyčka pro příjem příkazů
    mqttManager.loop();

    // Zpracování OTA aktualizací
    otaUpdater.handle();

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // Získání posledního stavu topení z InfluxDB
        influxDB.getLastHeaterState(heaterState);

        // Získání posledního stavu světla z InfluxDB
        influxDB.getLastLightState(lightState);

        // Nastavení topení podle stavu z InfluxDB
        if (heaterState && currrentHeaterState != true) {
            heater.turnOn();  // Pokud byl poslední stav "ON", zapneme topení
            currrentHeaterState = true;
        } else if (heaterState == false && currrentHeaterState != false) {
            heater.turnOff();  // Pokud byl poslední stav "OFF", vypneme topení
            currrentHeaterState = false;
        }
        else {
            // Pokud se stav nezměnil, nebudeme nic dělat
        }

        // Nastavení světla podle stavu z InfluxDB
        if (lightState && currrentLightState != true) {
            light.turnOn();  // Pokud byl poslední stav "ON", zapneme světlo
            currrentLightState = true;
        } else if (lightState == false && currrentLightState != false) {
            light.turnOff();  // Pokud byl poslední stav "OFF", vypneme světlo
            currrentLightState = false;
        }
        else {
            // Pokud se stav nezměnil, nebudeme nic dělat
        }
    }

}
