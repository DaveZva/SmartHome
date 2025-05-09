#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// WiFi a MQTT
const char* ssid = "your_WiFi_ssid";
const char* password = "your_WiFi_password";
const char* mqtt_server = "your_HW_IP";
const int mqtt_port = 1883;
const char* mqtt_user = "your_mqtt_user";
const char* mqtt_password = "your_mqtt_password";

#endif