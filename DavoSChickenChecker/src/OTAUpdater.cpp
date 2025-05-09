#include "OTAUpdater.h"

OTAUpdater::OTAUpdater(const char* hostname) : _hostname(hostname) {}

void OTAUpdater::begin() {
    ArduinoOTA.setHostname(_hostname);

    ArduinoOTA.onStart([]() {
        Serial.println("Začíná OTA update...");
    });

    ArduinoOTA.onEnd([]() {
        Serial.println("OTA update dokončen, restartuji...");
    });

    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA update: %u%%\n", (progress * 100) / total);
    });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA chyba [%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Autentizační chyba");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Chyba při zahájení");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Chyba připojení");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Chyba přijímání");
        else if (error == OTA_END_ERROR) Serial.println("Chyba ukončení");
    });

    ArduinoOTA.begin();
}

void OTAUpdater::handle() {
    ArduinoOTA.handle();
}
