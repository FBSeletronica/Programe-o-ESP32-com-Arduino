/*
 * Exemplo de light Sleep com Wake up por UART
 *
 * Autor: Fábio Souza
 * Data: 26/04/2025
 */

#include "esp_sleep.h"      // Biblioteca para Deep Sleep
#include <driver/uart.h>    // Biblioteca para UART

String receivedMessage = "";  // Variável para armazenar a mensagem recebida
int counter = 0;                // Contador para exibir no Serial Monitor          

// imprime o motivo do wakeup
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_GPIO:     Serial.println("Wakeup caused by GPIO (Light Sleep)"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    case ESP_SLEEP_WAKEUP_UART:     Serial.println("Wakeup caused by UART"); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Configurando wakeup via UART...");

  uart_set_wakeup_threshold(UART_NUM_0, 3);             // Configura o threshold de wakeup para 3 bytes
  esp_sleep_enable_uart_wakeup(UART_NUM_0);             // Habilita o wakeup por UART   

  Serial.println("Sistema pronto para Light Sleep.");

}

void loop() {
  Serial.printf("Contador: %d\n", counter);
  counter++;

  Serial.println("Indo para Light Sleep...");
  delay(500);

  esp_light_sleep_start();

  Serial.println("----------------------");
  Serial.println("Acordou do Light Sleep!");
  print_wakeup_reason();

  delay(150);

  Serial.write(' ');

  receivedMessage = "";

  unsigned long timeout = millis() + 3000;
  while (millis() < timeout){
    while (Serial.available()) {
      char incomingChar = Serial.read();
      if (incomingChar == '\n') { // Se terminar com \n
        Serial.print("Comando recebido: ");
        Serial.println(receivedMessage);
        receivedMessage = "";
        break;
      } else {
        receivedMessage += incomingChar;
      }
    }
  }

  delay(2000);
  
}
