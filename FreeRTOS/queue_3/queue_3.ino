/*********************************************************
* Exemplo de fila com FreeRTOS - Enviando e recebendo mensagens
* Utiliza a função xQueuePeek para receber a mensagem sem removê-la da fila
* Utiliza a função xQueueReceive para receber a mensagem e removê-la da fila
* Utiliza a função xQueueSend para enviar a mensagem para a fila
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

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

const int LED =  LED_VERMELHO;    // Pino do LED

#define QUEUE_LENGTH 5

// Definição da fila
QueueHandle_t xQueue;

void sender_task(void *pvParameters) {
    int message = 1;
    while (1) 
    {
        xQueueSend(xQueue, &message, portMAX_DELAY);        // Envia a mensagem para a fila
        vTaskDelay(pdMS_TO_TICKS(1000));                    // Delay de 1 segundo
        message++;                                          // Altera a mensagem
    }
}

void receiver_task1(void *pvParameters) {
    int received_message;                                    // Variável que irá armazenar a mensagem recebida
    while (1) 
    {
        if (xQueuePeek(xQueue, &received_message, 0) == pdTRUE) {                   // xQueuePeek não remove a mensagem da fila
            Serial.printf("Task 1 recebeu a mensagem: %d\n", received_message);     // Imprime a mensagem
        }
        vTaskDelay(pdMS_TO_TICKS(500));                                             // Atraso de 0,5 segundos
    }
}

void receiver_task2(void *pvParameters) {
    int received_message;
    while (1) 
    {
        if (xQueueReceive(xQueue, &received_message, portMAX_DELAY) == pdPASS) {    // xQueueReceive remove a mensagem da fila
            Serial.printf("Task 2 recebeu a mensagem: %d\n", received_message);     // Imprime a mensagem
        }
        vTaskDelay(pdMS_TO_TICKS(500));                                             // Atraso de 0,5 segundos
    }
}

void setup() {
    Serial.begin(9600);                                             // Inicializa a Serial
    pinMode(LED,OUTPUT);                                            // Configura o LED como saída
    xQueue = xQueueCreate(QUEUE_LENGTH, sizeof(int));

    xTaskCreate(sender_task, "Sender Task", 2048, NULL, 2, NULL);
    xTaskCreate(receiver_task1, "Receiver Task 1", 2048, NULL, 1, NULL);
    xTaskCreate(receiver_task2, "Receiver Task 2", 2048, NULL, 1, NULL);
}

void loop(){
  digitalWrite(LED,!digitalRead(LED));                        // Inverte o estado do LED
  vTaskDelay(pdMS_TO_TICKS(1000));                            // Delay de 1 segundo
 }
