/*
 * Exemplo de deep sleep com wakeup GPIO externo(ext1)
* O exemplo utiliza dois pinos GPIO para acordar o ESP32 do modo deep sleep.
* O código configura os pinos GPIO0 e GPIO1 como entradas com pull-up e pull-down.
* O ESP32 entra em modo deep sleep e aguarda um sinal de nível baixo em qualquer um dos pinos GPIO.
* Quando um sinal de nível baixo é detectado, o ESP32 acorda e imprime o motivo do wakeup e o número de vezes que acordou.
* O código também imprime o pino que gerou o wakeup.
* O exemplo utiliza a biblioteca RTC para configurar os pinos GPIO como entradas de baixa potência. 
 *
 * Autor: Fábio Souza
 * Data: 28/04/2025
 */

#include "driver/rtc_io.h"  // Biblioteca para configurar os pinos GPIO como entradas de baixa potência

// Definições dos pinos GPIO que serão usados para acordar o ESP32
#define WAKEUP_GPIO0 GPIO_NUM_0
#define WAKEUP_GPIO1 GPIO_NUM_1

// Definição da máscara de pinos para o wakeup externo
#define BUTTON_PIN_MASK ((1ULL<<WAKEUP_GPIO0)|(1ULL<<WAKEUP_GPIO1))

RTC_DATA_ATTR int bootCount = 0;  // Variável que armazena o número de vezes que o ESP32 acordou

//função para pegar o pino que gerou o wakeup
void print_gpio_wake_up(){
  int gpio_woke_up = esp_sleep_get_ext1_wakeup_status();  // Pega o status do GPIO que gerou o wakeup
  Serial.print("GPIO 0x");                                // Imprime o pino que gerou o wakeup
  Serial.println(gpio_woke_up,HEX);                       // Imprime o pino em hexadecimal
}

// Função para mostrar o motivo do wakeup
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); print_gpio_wake_up(); break;
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

  esp_sleep_enable_ext1_wakeup(BUTTON_PIN_MASK, ESP_EXT1_WAKEUP_ANY_LOW); // Habilita o wakeup externo com máscara de pinos
  rtc_gpio_pullup_en(GPIO_NUM_0);     // Habilita o pull-up no GPIO0
  rtc_gpio_pulldown_dis(GPIO_NUM_0);  // Desabilita o pull-down no GPIO0
  rtc_gpio_pullup_en(GPIO_NUM_1);     // Habilita o pull-up no GPIO1
  rtc_gpio_pulldown_dis(GPIO_NUM_1);  // Desabilita o pull-down no GPIO1  

  Serial.println("Indo para Deep Sleep, Pressione o Botão GPIO ou GPIO1 para acordar");
  esp_deep_sleep_start();

}

void loop() {
  // nunca vai chegar aqui
}
