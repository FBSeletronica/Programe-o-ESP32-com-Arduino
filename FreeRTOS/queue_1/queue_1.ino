/*********************************************************
* Exemplo que demonstra como criar uma fila, adicionar e ler
* dados da fila através de duas tasks
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

/*Bibliotecas FreeRTOS */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

const int LED = LED_VERMELHO;           // Pino do LED  

QueueHandle_t xFila;                    // Cria handle da fila
TaskHandle_t xTask1Handle,xTask2Handle; // Cria a handle das tasks

void vTask1(void *pvParameters);        // Protótipo da Task 1
void vTask2(void *pvParameters);        // Protótipo da Task 2

void setup() {

  BaseType_t xReturned;                 // Variável para armazenar o retorno das funções do FreeRTOS

  Serial.begin(9600);                   // Inicializa a Serial
  pinMode(LED,OUTPUT);                  // Configura o pino do LED como saída

  xFila = xQueueCreate(1,sizeof(int));  // Cria uma fila com 1 elemento do tipo int

  if(xFila == NULL){                                 // Verifica se a fila foi criada
    Serial.println("Nao foi possível criar a fila"); // Imprime uma mensagem de erro 
    while(1);                                        // Loop infinito
  }

  xReturned = xTaskCreate(vTask1,"Task1",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask1Handle); // Cria a Task 1

  // Verifica se a Task foi criada com sucesso
  if(xReturned == pdFAIL){
    Serial.println("Não foi possível criar a Task 1");
    while(1);
  }

  xReturned = xTaskCreate(vTask2,"Task2",configMINIMAL_STACK_SIZE+1024,NULL,1,&xTask2Handle);   // Cria a Task 2
  // Verifica se a Task foi criada com sucesso
  if(xReturned == pdFAIL){
    Serial.println("Não foi possível criar a Task 2");
    while(1);
  }
}

void loop() {
  digitalWrite(LED, !digitalRead(LED));
  vTaskDelay(pdMS_TO_TICKS(1000));
}

/*
  Task responsável por enviar dados para a fila
*/
void vTask1(void *pvParameters){

  int count = 0;                                // Variável para contar

  while (1)
  {
    if(count<10){
        xQueueSend(xFila, &count,portMAX_DELAY);  // Envia o valor de count para a fila
        count++;                                  // Incrementa o valor de count
    }
    else{
      count = 0;                                  // Reinicia o valor de count
      vTaskDelay(pdMS_TO_TICKS(5000));            // Delay de 5 segundos
    }
    vTaskDelay(pdMS_TO_TICKS(500));               // Delay de 500ms
  }
}

/*
  Task responsável por receber dados da fila
*/
void vTask2(void *pvParameters){

  int valorRecebido = 0;                      // Variável para armazenar o valor recebido

  while (1)
  {
    if(xQueueReceive(xFila,&valorRecebido,pdMS_TO_TICKS(1000))==pdTRUE){  // Verifica se existe dado na fila 
      Serial.println("Valor Recebido: " + String(valorRecebido));         // Imprime o valor recebido
    }
    else{
      Serial.println("TIMEOUT");                                          // Caso ocorra timeout imprime uma mensagem de erro  
    }
  }
}
