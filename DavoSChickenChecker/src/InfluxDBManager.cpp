#include "InfluxDBManager.h"
#include <WiFiClient.h>
#include <ArduinoHttpClient.h>

// Konstruktor třídy InfluxDBManager
InfluxDBManager::InfluxDBManager(const String& url, const String& bucket, const String& username, const String& password)
    : url(url), bucket(bucket), username(username), password(password) {}

// Funkce pro získání posledního stavu zařízení "start_heater_2" z InfluxDB
void InfluxDBManager::getLastHeaterState(bool &state) {
    // Použití WiFiClient pro HTTP
    WiFiClient wifiClient;

    // HTTP klient pro komunikaci s InfluxDB serverem
    HttpClient http(wifiClient, url, 8086); // Port 8086 pro HTTP

    // Definice dotazu pro získání posledního stavu zařízení "start_heater_2"
    String query = R"(
        from(bucket: "homeassistant")
        |> range(start: -30d)
        |> filter(fn: (r) => r._field == "state")
        |> filter(fn: (r) => r.entity_id == "start_heater_2")
        |> sort(columns: ["_time"], desc: true)
        |> limit(n: 1)
    )";

    // Autentizace (Base64 Basic Auth)
    String auth = username + ":" + password;
    String encodedAuth = encodeBase64(auth);

    Serial.println("Dotaz na InfluxDB: " + query);

    // Odeslání POST požadavku na InfluxDB
    http.beginRequest();
    http.post("/api/v2/query");
    http.sendHeader("Authorization", "Basic " + encodedAuth);
    http.sendHeader("Content-Type", "application/vnd.flux");
    http.sendHeader("Content-Length", query.length());
    http.beginBody();
    http.print(query);
    http.endRequest();

    // Získání odpovědi
    int httpResponseCode = http.responseStatusCode();  // Získání status kódu odpovědi
    Serial.println("HTTP Response Code: " + String(httpResponseCode));

    String response = http.responseBody();  // Získání těla odpovědi
    Serial.println("Odpověď od InfluxDB: " + response);

    // Zpracování odpovědi
    if (httpResponseCode == 200) {
        if (response.indexOf(",on,") != -1) {
            state = true;  // Zařízení je zapnuto
            Serial.println("Zařízení je zapnuto");
        } else if (response.indexOf(",off,") != -1) {
            state = false;  // Zařízení je vypnuto
            Serial.println("Zařízení je vypnuto");
        } else {
            Serial.println("Chyba: neznámá hodnota stavu");
        }
    } else {
        // Pokud došlo k chybě při komunikaci s InfluxDB
        Serial.print("Chyba při dotazu na InfluxDB: ");
        Serial.println(httpResponseCode);
    }

    // Uzavření spojení
    http.stop();
}

void InfluxDBManager::getLastLightState(bool &state) {
    // Použití WiFiClient pro HTTP
    WiFiClient wifiClient;

    // HTTP klient pro komunikaci s InfluxDB serverem
    HttpClient http(wifiClient, url, 8086); // Port 8086 pro HTTP

    // Definice dotazu pro získání posledního stavu zařízení "start_light_2"
    String query = R"(
        from(bucket: "homeassistant")
        |> range(start: -30d)
        |> filter(fn: (r) => r._field == "state")
        |> filter(fn: (r) => r.entity_id == "start_light")
        |> sort(columns: ["_time"], desc: true)
        |> limit(n: 1)
    )";

    // Autentizace (Base64 Basic Auth)
    String auth = username + ":" + password;
    String encodedAuth = encodeBase64(auth);

    Serial.println("Dotaz na InfluxDB: " + query);

    // Odeslání POST požadavku na InfluxDB
    http.beginRequest();
    http.post("/api/v2/query");
    http.sendHeader("Authorization", "Basic " + encodedAuth);
    http.sendHeader("Content-Type", "application/vnd.flux");
    http.sendHeader("Content-Length", query.length());
    http.beginBody();
    http.print(query);
    http.endRequest();

    // Získání odpovědi
    int httpResponseCode = http.responseStatusCode();  // Získání status kódu odpovědi
    Serial.println("HTTP Response Code: " + String(httpResponseCode));

    String response = http.responseBody();  // Získání těla odpovědi
    Serial.println("Odpověď od InfluxDB: " + response);

    // Zpracování odpovědi
    if (httpResponseCode == 200) {
        if (response.indexOf(",on,") != -1) {
            state = true;  // Zařízení je zapnuto
            Serial.println("Zařízení je zapnuto");
        } else if (response.indexOf(",off,") != -1) {
            state = false;  // Zařízení je vypnuto
            Serial.println("Zařízení je vypnuto");
        } else {
            Serial.println("Chyba: neznámá hodnota stavu");
        }
    } else {
        // Pokud došlo k chybě při komunikaci s InfluxDB
        Serial.print("Chyba při dotazu na InfluxDB: ");
        Serial.println(httpResponseCode);
    }
}
// Funkce pro Base64 kódování
String InfluxDBManager::encodeBase64(const String& input) {
    String encoded;
    const char* data = input.c_str();
    size_t len = input.length();
    unsigned char encodedData[(len * 4 / 3) + 4]; // Maximální délka Base64 výstupu
    size_t outputLength;

    // Kódování Base64
    mbedtls_base64_encode(encodedData, sizeof(encodedData), &outputLength, (const unsigned char*)data, len);
    encoded = String((char*)encodedData);
    return encoded;
}
