#ifndef INFLUXDBMANAGER_H
#define INFLUXDBMANAGER_H

#include <Arduino.h>
#include "mbedtls/base64.h" // Base64 pro ESP32

class InfluxDBManager {
public:
    InfluxDBManager(const String& url, const String& bucket, const String& username, const String& password);
    void getLastHeaterState(bool &state);
    void getLastLightState(bool &state);
private:
    String url;
    String bucket;
    String username;
    String password;

    String encodeBase64(const String& input);  // Pomocná metoda pro Base64
};

#endif // INFLUXDBMANAGER_H
