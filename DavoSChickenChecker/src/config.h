#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// WiFi a MQTT
const char* ssid = "your_WiFi_ssid";
const char* password = "your_WiFi_passoword";
const char* mqtt_server = "your_HW_ip";
const int mqtt_port = 1883;
const char* mqtt_user = "your_mqtt_user";
const char* mqtt_password = "your_mqtt_password";

// InfluxDB konfigurace
const String influxUrl = "your_HW_ip";  // URL InfluxDB
const String influxBucket = "homeassistant";
const String influxUsername = "homeassistant";
const String influxPassword = "your_password_influx";

#endif