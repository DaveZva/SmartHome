#ifndef OTA_UPDATER_H
#define OTA_UPDATER_H

#include <Arduino.h>
#include <ArduinoOTA.h>

class OTAUpdater {
public:
    OTAUpdater(const char* hostname);
    
    void begin();
    void handle();
    
private:
    const char* _hostname;
};

#endif
