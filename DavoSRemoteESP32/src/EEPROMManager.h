#ifndef EEPROMManager_h
#define EEPROMManager_h

#include <Arduino.h>
#include <EEPROM.h>
#include <Wire.h>

class EEPROMManager {
  public:
    EEPROMManager();  // Konstruktor
    void saveDataCycle(int cycle);  // Uložení dataCycle do EEPROM
    int loadDataCycle();  // Načtení dataCycle z EEPROM

  private:
    const int dataCycleAddress = 0;  // Adresa v EEPROM pro uložení dataCycle
};

#endif
