/*
 * Exemplo de light Sleep com Wake up por Touch
 *
 * Autor: Fábio Souza
 * Data: 26/04/2025
 */

#include "esp_sleep.h"

#define TOUCH_PIN T2          // Pino do Touch
#define TOUCH_THRESHOLD 150   // Limite de toque (ajustar conforme necessário)

volatile bool touchDetected = false;  // Variável para indicar se o toque foi detectado
int i = 0;

// Função chamada quando o toque é detectado
void gotTouchEvent() {
  touchDetected = true;
}

// Função para imprimir a razão do wakeup
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
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("Configurando wakeup via Touch (touchAttachInterrupt)...");
  
  //Configura interrupção por toque
  touchAttachInterrupt(TOUCH_PIN, gotTouchEvent, TOUCH_THRESHOLD);  // Configura o pino de toque e a função de callback
  esp_sleep_enable_touchpad_wakeup();                               // Habilita o wakeup por toque                    

}

void loop() {
  if(!touchDetected){                                 // Se não houver toque detectado
    Serial.println("Dormindo...");                    // Inicia o modo de Light Sleep
    Serial.flush();                                   // Garante que todos os dados sejam enviados antes de dormir

    esp_light_sleep_start();                          // Inicia o Light Sleep                   

    Serial.println("Acordou do Light Sleep!");        // Acorda do Light Sleep
    print_wakeup_reason();                            // Imprime a razão do wakeup                 
    i++;
    Serial.println("Contador de Acordadas: " + String(i));  // Imprime o número de vezes que acordou
  }
  else{                                                       // Se o toque foi detectado        
    Serial.println("Toque detectado! Resetando o estado..."); // Reseta o estado do toque
    touchDetected = false;                                    // Reseta a variável de toque detectado
  }
}
