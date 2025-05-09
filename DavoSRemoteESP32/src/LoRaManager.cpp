#include "LoRaManager.h"

void LoRaManager::setup() {
    Serial.println("Inicializace LoRa...");
    LoRa.setPins(7, 6, 10); // CS, RESET, IRQ
    if (!LoRa.begin(433E6)) {
        Serial.println("Chyba při inicializaci LoRa!");
        while (1);
    }
    
    LoRa.setTxPower(20);                // Maximální výkon 20 dBm
    LoRa.setSpreadingFactor(12);        // Vysoký spreading factor
    LoRa.setSignalBandwidth(62.5E3);    // Nižší šířka pásma
    LoRa.setCodingRate4(8);             // Maximální korekce chyb

    Serial.println("LoRa inicializována na frekvenci 433 MHz.");
    lastSendSuccessful = false;
}

void LoRaManager::sendMessage(const String& message) {
    LoRa.beginPacket();
    LoRa.print(message);
    lastSendSuccessful = LoRa.endPacket() == 0; // 0 znamená úspěch
    Serial.println("Odeslána LoRa zpráva: " + message);
}

String LoRaManager::readMessage() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String receivedMessage = "";
        while (LoRa.available()) {
            receivedMessage += (char)LoRa.read();
        }
        return receivedMessage;
    }
    return "";
}

bool LoRaManager::wasLastSendSuccessful() {
    return lastSendSuccessful;
}

void LoRaManager::sleep() {
    // Vypnutí LoRa modulu (např. snížení spotřeby)
    LoRa.end(); // Ukončení LoRa práce pro snížení spotřeby
    Serial.println("LoRa modul uveden do úsporného režimu.");
}
