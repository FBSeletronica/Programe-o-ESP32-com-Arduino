/*
 * Exemplo de Deep Sleep com Wake up por Timer
 *
 * Autor: Fábio Souza
 * Data: 28/04/2025
 */

RTC_DATA_ATTR int bootCount = 0;  // Variável RTC_DATA_ATTR para manter o valor entre os ciclos de deep sleep

// Função para mostrar o motivo do wakeup
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    default:                        Serial.printf("Wakeup not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  bootCount++;
  Serial.println("===============");
  Serial.println("ESP32 woke up!");
  Serial.println("Contador: "+ String(bootCount));
  print_wakeup_reason();
  Serial.println("===============");

  int sleepTimeSeconds = 10;
  Serial.println("Indo para Deep Sleep por " + String(sleepTimeSeconds) + "segundos...");
  esp_sleep_enable_timer_wakeup(sleepTimeSeconds*1000000);  // configurar o timer para acordar após 10 segundos
  esp_deep_sleep_start();                                   // Inicia o deep sleep

}

void loop() {
  // nunca vai chegar aqui
}
