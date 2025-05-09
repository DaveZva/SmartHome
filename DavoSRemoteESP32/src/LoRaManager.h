#ifndef LORAMANAGER_H
#define LORAMANAGER_H

#include <LoRa.h>

class LoRaManager {
public:
    void setup();
    void sendMessage(const String& message);
    String readMessage();
    bool wasLastSendSuccessful(); // Kontrola úspěchu posledního odeslání
    void sleep();                 // Uvedení LoRa do úsporného režimu

private:
    bool lastSendSuccessful;      // Stav posledního odeslání
};

#endif
