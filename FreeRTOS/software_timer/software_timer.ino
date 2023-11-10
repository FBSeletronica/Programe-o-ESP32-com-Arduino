/*********************************************************
* Exemplo para demonstrar o uso de software timer
* Por: Fábio Souza
*********************************************************/
#define LED_VERMELHO 14   // Pino do LED Vermelho
#define LED_VERDE 13      // Pino do LED Verde
#define LED_AZUL 12       // Pino do LED Azul
#define BOTAO_1 7         // Pino do Botão 1
#define BOTAO_2 6         // Pino do Botão 2
#define BOTAO_3 5         // Pino do Botão 3
#define BOTAO_4 4         // Pino do Botão 4
#define BOTAO_5 3         // Pino do Botão 5
#define BOTAO_6 2         // Pino do Botão 6
#define BUZZER  17        // Pino Do Buzzer

const int LED1 = 21;            // Pino do LED 1
const int LED2 = LED_VERMELHO;  // Pino do LED 2
const int BUTTON1 = BOTAO_6;    // Pino do Botão 1

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

TaskHandle_t xTask1_Handle;                   // Handle da Task1
TimerHandle_t xTimer1, xTimer2; // Handle do Timer1 e Timer2

/*Protótipo das Tasks*/
void vTask1(void *pvParameters);              // Task1
void vTimer1_Callback(TimerHandle_t xTimer);  // Callback do Timer1
void vTimer2_Callback(TimerHandle_t xTimer);  // Callback do Timer2


void setup() {
  Serial.begin(115200);             // Inicializa a Serial
  pinMode(LED1, OUTPUT);            // Configura o LED1 como saída
  pinMode(LED2, OUTPUT);            // Configura o LED2 como saída
  pinMode(BUTTON1, INPUT_PULLUP);   // Configura o Botão 1 como entrada

  
  xTimer1 = xTimerCreate("Timer1", pdMS_TO_TICKS(1000), pdTRUE, (void*)0, vTimer1_Callback);     // Cria o Timer1
  xTimer2 = xTimerCreate("Timer2", pdMS_TO_TICKS(5000), pdFALSE, (void*)0, vTimer2_Callback);    // Cria o Timer2

  xTaskCreate(vTask1, "Task1", configMINIMAL_STACK_SIZE + 1024, NULL, 1, &xTask1_Handle);               // Cria a Task1

  xTimerStart(xTimer1, 0);   // Inicia o Timer1

}

void loop() {
  //nothing to do here
  vTaskDelay(pdMS_TO_TICKS(1000));
}

/* Task1 */
void vTask1(void *pvParameters) 
{
  unsigned long debounceDelay = 50; // Atraso para evitar o efeito de bounce
  unsigned long lastDebounceTime = 0; // Último tempo de debounce
  bool buttonState = HIGH;   // Estado atual do botão
  int lastButtonState = HIGH; // Último estado do botão

   while(1) 
 {
    int reading = digitalRead(BUTTON1);                           // Lê o estado do botão

    if (reading != lastButtonState) {                               // Verifica se o estado do botão mudou
      lastDebounceTime = millis();                                  // Atualiza o tempo de debounce
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {            // Verifica se o tempo de debounce expirou
      if (reading != buttonState) {                                 // Verifica se o estado atual da leitura é diferente do estado do botão
        buttonState = reading;                                      // Atualiza o estado do botão

        if (buttonState == LOW) {                                   // Verifica se o botão foi pressionado
          digitalWrite(LED2,HIGH);                                  // Liga o LED2
          xTimerStart(xTimer2,0);                                   // Inicia o Timer2
          Serial.println("Botão pressionado!");
        }
      }
    }
    lastButtonState = reading;                                      // Atualiza o último estado do botão
    vTaskDelay(pdMS_TO_TICKS(1));                                  // Aguarda 1ms
  }
}

/* Callback do Timer1 */
void vTimer1_Callback(TimerHandle_t xTimer)
{
   digitalWrite(LED1,!digitalRead(LED1));     // Inverte o estado do LED1
}

/* Callback do Timer2 */
void vTimer2_Callback(TimerHandle_t xTimer)
{
  digitalWrite(LED2,LOW);           // Desliga o LED2
}