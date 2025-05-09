#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include <WiFi.h>

class MQTTManager {
private:
    WiFiClient espClient;
    PubSubClient client;
    bool irrigationCommand = false;

    void mqttCallback(char* topic, byte* payload, unsigned int length);

public:
    MQTTManager();
    void setup(const char* server, int port, const char* user, const char* password);
    boolean connect(const char* clientId, const char* user, const char* password, const char* subscribeTopic);
    void publish(const char* topic, float value);
    void loop();
    bool getIrrigationCommand();
    void resetIrrigationCommand();
};

#endif
