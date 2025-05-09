#include "EEPROMManager.h"

EEPROMManager::EEPROMManager() {
  // Konstruktor, můžeme zde inicializovat nějaké nastavení, pokud je potřeba
}

// Uloží hodnotu dataCycle do EEPROM
void EEPROMManager::saveDataCycle(int cycle) {
  // Uložíme hodnotu do EEPROM (2 bajty pro celou hodnotu)
  EEPROM.write(dataCycleAddress, cycle & 0xFF);  // Uložíme nižší byte
  EEPROM.write(dataCycleAddress + 1, (cycle >> 8) & 0xFF);  // Uložíme vyšší byte
  Serial.println("Data cycle saved to EEPROM");
}

// Načte hodnotu dataCycle z EEPROM
int EEPROMManager::loadDataCycle() {
  int storedCycle = EEPROM.read(dataCycleAddress) | (EEPROM.read(dataCycleAddress + 1) << 8);  // Spojíme dva bajty
  if (storedCycle > 0) {  // Zkontrolujeme, jestli hodnota není neplatná
    Serial.println("Data cycle loaded from EEPROM: " + String(storedCycle) + " ms");
    return storedCycle;
  } else {
    Serial.println("No valid data cycle in EEPROM, using default.");
    return 10000;  // Vrátí výchozí hodnotu, pokud je hodnota v EEPROM neplatná
  }
}
