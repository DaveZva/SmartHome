#include "WiFi.h"
#include "LoRaManager.h"
#include "BMEManager.h"
#include "EEPROMManager.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"

LoRaManager loraManager;
BMEManager bmeManager;
EEPROMManager eepromManager;

uint64_t dataCycle = 3600000; // Defaultní cyklus odesílání dat v ms

String bmeMessage();
bool waitForAcknowledgement();

void setup() {
  Serial.begin(115200);

  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();
    if (wakeup_reason == ESP_SLEEP_WAKEUP_TIMER) {
      Serial.println("ESP32 se probudilo z deep sleepu díky časovači.");
    } else {
      Serial.println("ESP32 se spustilo normálně.");
    }
 
   // Vypnutí WiFi pro úsporu energie
  WiFi.mode(WIFI_OFF);
  btStop();
 
  Wire.begin();

  // Inicializace BME senzoru
  bmeManager.begin();

  // Inicializace LoRa
  loraManager.setup();

  // Načtení cyklu odesílání dat z EEPROM
  // dataCycle = eepromManager.loadDataCycle();
}

void loop() {
  // Čtení dat ze senzoru
  if (esp_sleep_get_wakeup_cause() == ESP_SLEEP_WAKEUP_TIMER) {
    loraManager.setup();
    bmeManager.begin();
  }

  String message = bmeMessage();

  // Odesílání dat a kontrola potvrzení
  while (true) {
    loraManager.sendMessage(message);
    delay(100); // Krátké čekání na dokončení odeslání
    if(waitForAcknowledgement()) {
      Serial.println("Zpráva potvrzena.");
      break; // Pokud je zpráva potvrzena, pokračujeme
    } else {
      Serial.println("Zpráva nebyla potvrzena, opakuji odeslání.");
    }
  }

  // Zpracování přijatých zpráv (např. změna cyklu)
  String text = loraManager.readMessage();
  if (!text.isEmpty()) {
    Serial.println("Přijata zpráva: " + text);
    if (text.startsWith("Cycle:")) {
      int newCycle = text.substring(6).toInt();
      if (newCycle > 0) {
        dataCycle = newCycle;
        //eepromManager.saveDataCycle(dataCycle);
        Serial.println("Cyklus změněn na: " + String(dataCycle) + " ms");
      }
    }
  }

  // Příprava na úsporný režim
  loraManager.sleep();
  bmeManager.sleep();
  delay(100);

  // Deep sleep na nastavenou dobu
  Serial.println("Usínám na " + String(dataCycle) + " ms");
  esp_sleep_enable_timer_wakeup(dataCycle * 1000ULL);
  delay(100);
  esp_light_sleep_start();
}

String bmeMessage() {
  float temperature = bmeManager.readTemperature();
  float humidity = bmeManager.readHumidity();
  float pressure = bmeManager.readPressure();

  // Kontrola hodnot a sestavení zprávy
  String message = "";
  if (temperature == 0 && humidity == 0 && pressure == 0) {
    message = "T:0;H:0;S:90;P:0;V:0";
  } else if(temperature > 60 || temperature < -40 || humidity > 100 || humidity < 0 || pressure < 600 || pressure > 1300) {
    message = "T:0;H:0;S:90;P:0;V:0";
  } else {
    message = "T:" + String(temperature) + ";H:" + String(humidity) + ";S:90;P:" + String(pressure) + ";V:" + String(0);
  }
  return message;
}

bool waitForAcknowledgement() {
  unsigned long startTime = millis();
  String response = "";

  while (millis() - startTime < 5000) {
    response = loraManager.readMessage();
    if (response == "ACK") {
      Serial.println("Zpráva potvrzena.");
      return true;
    }
    delay(100);
  }
  return false;
}