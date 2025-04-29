/*
 * Exemplo de deep sleep com wakeup GPIO externo(ext0)
 *
 * Autor: Fábio Souza
 * Data: 28/04/2025
 */

 //biblioteca do RTC IO para GPIO
#include "driver/rtc_io.h"

#define WAKEUP_GPIO GPIO_NUM_0  // GPIO que vai acordar o ESP32

 // Variável que armazena o número de vezes que o ESP32 acordou
// RTC_DATA_ATTR significa que a variável será armazenada na memória RTC
// e não será perdida durante o deep sleep
RTC_DATA_ATTR int bootCount = 0; 

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

  esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);  // GPIO_NUM_0 vai acordar o ESP32 quando estiver em nível baixo
  rtc_gpio_pullup_en(GPIO_NUM_0);               // Habilita o pull-up do GPIO_NUM_0 no RTC
  rtc_gpio_pulldown_dis(GPIO_NUM_0);            // Desabilita o pull-down do GPIO_NUM_0 no RTC

  Serial.println("Indo para Deep Sleep, Pressione o Botão para acordar");
  esp_deep_sleep_start();

}

void loop() {
  // nunca vai chegar aqui
}
