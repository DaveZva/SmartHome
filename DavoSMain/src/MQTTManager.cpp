#include "MQTTManager.h"

MQTTManager::MQTTManager() : client(espClient) {}

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
            return false;
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

void MQTTManager::loop() {
    client.loop();
}

bool MQTTManager::getIrrigationCommand() {
    return irrigationCommand;
}

void MQTTManager::resetIrrigationCommand() {
    irrigationCommand = false;
}

void MQTTManager::mqttCallback(char* topic, byte* payload, unsigned int length) {
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }

    Serial.println("Přijat MQTT příkaz: " + message);

    if (String(topic) == "home/garden/irrigation_command" && message == "ON") {
        irrigationCommand = true;
    }
}
