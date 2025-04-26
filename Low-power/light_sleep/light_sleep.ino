/*
 * Exemplo de light Sleep com Wake up por Timer
 *
 * Autor: Fábio Souza
 * Data: 26/04/2025
 */

#include "esp_sleep.h"    // Biblioteca para Deep Sleep

int i = 0;  //variável para contagem de loops para demonstrar que a RAM não é perdida


// Função para imprimir o motivo do wakeup
// Essa função é chamada após o microcontrolador acordar do modo de sono
// Ela verifica o motivo do wakeup e imprime uma mensagem correspondente
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason; // Variável para armazenar o motivo do wakeup

  wakeup_reason = esp_sleep_get_wakeup_cause(); // Obtém o motivo do wakeup

  // Verifica o motivo do wakeup e imprime uma mensagem correspondente
  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  esp_sleep_enable_timer_wakeup(5*1000000); // Habilita o timer para acordar após 5 segundos

}

void loop() {
  Serial.println("Entrando no Light Sleep por 5 segundos..."); // Mensagem de debug   
  Serial.flush();                                              // Garante que todos os dados sejam enviados antes de entrar em sleep 
  esp_light_sleep_start();                                    // Coloca o ESP32 em Light Sleep
  Serial.println("Acordou do Light Sleep!");                  // Mensagem de debug  
  i++;                                                        // Incrementa o contador para verificar se a RAM não foi perdida                           
  Serial.println(i);                                          // Imprime o valor de i para verificar se a RAM não foi perdida
  print_wakeup_reason();                                      // Chama a função para imprimir o motivo do wakeup                
  delay(500);
}
