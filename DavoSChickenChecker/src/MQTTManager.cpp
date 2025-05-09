#include "MQTTManager.h"

// Upravený konstruktor
MQTTManager::MQTTManager(PubSubClient& client, HeaterController& heaterController, ChickenLightController& lightController)
    : client(client), heaterController(heaterController), lightController(lightController) {}   

void MQTTManager::setup(const char* server, int port, const char* user, const char* password) {
    client.setServer(server, port);
    client.setCallback([this](char* topic, byte* payload, unsigned int length) {
        this->mqttCallback(topic, payload, length);
    });
}

boolean MQTTManager::connect(const char* clientId, const char* user, const char* password, const char* subscribeTopic) {
    while (!client.connected()) {
        Serial.println("Připojování k MQTT...");
        if (client.connect(clientId, user, password)) {
            Serial.println("Připojeno k MQTT!");
            client.subscribe(subscribeTopic);
            return true;
        } else {
            Serial.print("MQTT připojení selhalo, rc=");
            Serial.println(client.state());
            delay(2000);
        }
    }
    return false;
}

void MQTTManager::publish(const char* topic, float value) {
    if (client.connected()) {
        client.publish(topic, String(value).c_str());
        Serial.println(String("Data odeslána: ") + topic);
    }
}

// Nová metoda pro posílání textových zpráv
void MQTTManager::publish(const char* topic, const char* message) {
    if (client.connected()) {
        client.publish(topic, message);
        Serial.println(String("Data odeslána: ") + topic + " - " + message);
    }
}

void MQTTManager::loop() {
    client.loop();
}

void MQTTManager::mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.println("Přijat MQTT příkaz: " + message);

    // Pokud se jedná o příkaz pro řízení topného drátu
    if (String(topic) == "home/heater/set") {
        if (message == "ON") {
            heaterController.turnOn();  // Zapne topný drát
        } else if (message == "OFF") {
            heaterController.turnOff(); // Vypne topný drát
        }
    }
    // Pokud se jedná o příkaz pro řízení světla
    if (String(topic) == "home/light/set") {
        if (message == "ON") {
            lightController.turnOn(); 
        } else if (message == "OFF") {
            lightController.turnOff(); // Vypne světlo
        }
    }
}