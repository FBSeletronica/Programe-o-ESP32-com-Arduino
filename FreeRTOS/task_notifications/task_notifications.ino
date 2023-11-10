/*********************************************************
* Exemplo para demonstrar o uso de Task Notifications
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

const int LED1 = LED_VERMELHO;  // Pino do LED 1
const int LED2 = 21;     // Pino do LED 2
const int BUTTON1 = BOTAO_6;    // Pino do Botão 1

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

TaskHandle_t xTaskTrataBTHandle;  // Handle da Task que trata o Botão

void callback_isr_BT() {
  BaseType_t xHigherPriorityTaskWoken = pdTRUE; // Variável para indicar se a Task deve ser notificada
  vTaskNotifyGiveFromISR(xTaskTrataBTHandle,&xHigherPriorityTaskWoken); // Notifica a Task que trata o Botão
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);          // Define o pino do LED como saída
  pinMode(LED2, OUTPUT);          // Define o pino do LED como saída
  pinMode(BUTTON1, INPUT_PULLUP); // Define o pino do Botão como entrada
  attachInterrupt(BUTTON1, callback_isr_BT, FALLING); // Interrupção no Botão 1

  xTaskCreate(
    taskTrataBT,   // Função que implementa a Task
    "TaskTrataBT", // Nome da Task
    1024,          // Tamanho da pilha da Task
    NULL,          // Parâmetro de entrada da Task
    5,             // Prioridade da Task
    &xTaskTrataBTHandle); // Handle da Task
}

void loop() {
  //inverte LED 2 a cada 500 ms
  digitalWrite(LED2, !digitalRead(LED2));
  vTaskDelay(pdMS_TO_TICKS(500)); // Delay de 500ms
}

/* Task que trata o Botão */
void taskTrataBT(void *pvParameter) {
  while (1) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Aguarda a notificação da ISR
    Serial.println("Botão pressionado");
    digitalWrite(LED1, !digitalRead(LED1)); // Inverte o estado do LED
    vTaskDelay(pdMS_TO_TICKS(1000)); // Delay de 1s
  }
}
