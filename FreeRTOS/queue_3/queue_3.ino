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

//estrutura para mensagem
typedef struct{
    int id;             // ID da mensagem
    char string[20];    // String da mensagem
} Mensagem_t;

// Função para enviar a mensagem
void sender_task(void *pvParameters) {
    Mensagem_t mensagem;
    mensagem.id = 1;                                // Inicializa o ID da mensagem
        
    while (1) 
    {
        snprintf(mensagem.string, sizeof(mensagem.string), "Mensagem %d", mensagem.id);   // Cria a string da mensagem
        xQueueSend(xQueue, &mensagem, portMAX_DELAY);                                     // Envia a mensagem para a fila
        mensagem.id++;                                                                   // Incrementa o ID da mensagem
        vTaskDelay(pdMS_TO_TICKS(1000));                                                 // Delay de 1 segundo
        
    }
}

// Função para receber a mensagem
void receiver_task1(void *pvParameters) {
    Mensagem_t mensagem_peek;                                                         // Variável que irá armazenar a mensagem recebida
    while (1) 
    {
        if (xQueuePeek(xQueue, &mensagem_peek, 0) == pdTRUE) {                                               // xQueuePeek não remove a mensagem da fila
           Serial.printf("Task 1 Peek a mensagem ID %d: %s\n", mensagem_peek.id, mensagem_peek.string);   // Imprime a mensagem
        }        
        vTaskDelay(pdMS_TO_TICKS(250));                                             // Atraso de 0,5 segundos                                                                                            // A mensagem continua na fila                                                              // Atraso de 0,5 segundos
    }
}

// Função para receber a mensagem
void receiver_task2(void *pvParameters) {
    Mensagem_t mensagem_recebida;
    
    while (1) 
    {
        if (xQueueReceive(xQueue, &mensagem_recebida, portMAX_DELAY) == pdPASS) {    // xQueueReceive remove a mensagem da fila
            Serial.printf("Task 2 retirou a mensagem ID %d: %s\n", mensagem_recebida.id, mensagem_recebida.string);   // Imprime a mensagem
        }
        vTaskDelay(pdMS_TO_TICKS(500));                                             // Atraso de 0,5 segundos
    }
}

void setup() {
    Serial.begin(9600);                                             // Inicializa a Serial
    pinMode(LED,OUTPUT);                                            // Configura o LED como saída
    xQueue = xQueueCreate(QUEUE_LENGTH, sizeof(Mensagem_t));        // Cria a fila com 2 elementos do tipo Mensagem_t

    xTaskCreate(sender_task, "Sender Task", 2048, NULL, 2, NULL);           // Cria a task Sender
    xTaskCreate(receiver_task1, "Receiver Task 1", 2048, NULL, 1, NULL);    // Cria a task Receiver
    xTaskCreate(receiver_task2, "Receiver Task 2", 2048, NULL, 1, NULL);    // Cria a task Receiver
}

void loop(){
  digitalWrite(LED,!digitalRead(LED));                        // Inverte o estado do LED
  vTaskDelay(pdMS_TO_TICKS(1000));                            // Delay de 1 segundo
 }
