#ifndef MQTTMANAGER_H
#define MQTTMANAGER_H

#include <PubSubClient.h>
#include "HeaterController.h"
#include "ChickenLightController.h"

class MQTTManager {
public:
    MQTTManager(PubSubClient& client, HeaterController& heaterController, ChickenLightController& lightController);
    
    void setup(const char* server, int port, const char* user, const char* password);
    boolean connect(const char* clientId, const char* user, const char* password, const char* subscribeTopic);
    
    // Změna: umožní posílat jak float, tak textové zprávy
    void publish(const char* topic, float value);
    void publish(const char* topic, const char* message);
    void loop();
    
    void mqttCallback(char* topic, byte* payload, unsigned int length);
    
private:
    PubSubClient& client;
    HeaterController& heaterController;
    ChickenLightController& lightController; // Přidáno
};

#endif