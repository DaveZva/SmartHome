#include "LoRaManager.h"

void LoRaManager::setup() {
    Serial.println("Inicializace LoRa...");
    LoRa.setPins(10, 18, 4); // Nastavení pinů pro LoRa modul (NSS, RST, DIO0)
    if (!LoRa.begin(433E6)) {
        Serial.println("Chyba při inicializaci LoRa!");
        while (1); // Zastav program při chybě inicializace
    }
    Serial.println("LoRa inicializována na frekvenci 433 MHz.");

    // Nastavení parametrů LoRa modulu
    LoRa.setTxPower(20);                // Maximální výkon 20 dBm
    LoRa.setSpreadingFactor(12);        // Vysoký spreading factor
    LoRa.setSignalBandwidth(62.5E3);    // Nižší šířka pásma
    LoRa.setCodingRate4(8);
    
}

void LoRaManager::sendMessage(const String& message) {
    LoRa.beginPacket();
    LoRa.print(message);
    lastSendSuccessful = LoRa.endPacket() == 0; // 0 znamená úspěch
    Serial.println("Odeslána LoRa zpráva: " + message);
}

String LoRaManager::readMessage() {
    int packetSize = LoRa.parsePacket(); // Zjistí velikost přijatého paketu
    if (packetSize) {
        String receivedMessage = "";

        // Čtení přijatých dat z bufferu
        while (LoRa.available()) {
            receivedMessage += (char)LoRa.read();
        }

        // Debugging - výstup přijaté zprávy
        Serial.println("Přijatá LoRa zpráva: " + receivedMessage);

        // Přidání kontroly kvality signálu
        Serial.println("RSSI: " + String(LoRa.packetRssi()));
        Serial.println("SNR: " + String(LoRa.packetSnr()));

        return receivedMessage; // Vrací přijatou zprávu
    }

    // Pokud žádný paket nebyl přijat, neprovádí se žádný výpis
    return ""; // Žádná zpráva
}

void LoRaManager::reset() {
    Serial.println("Resetování LoRa modulu...");
    LoRa.end(); // Ukončí komunikaci s LoRa modulem
    delay(100); // Krátké zpoždění
    setup(); // Opětovná inicializace LoRa modulu
}
