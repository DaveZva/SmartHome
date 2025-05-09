#ifndef LORAMANAGER_H
#define LORAMANAGER_H

#include <LoRa.h>

class LoRaManager {
public:
    void setup();
    void sendMessage(const String& message);
    String readMessage();
    void reset();

private:
    bool lastSendSuccessful;
};

#endif
