/*********************************************************
* Exemplo que demonstra como enviar valores para uma fila 
* a partir de uma ISR (Interrup Service Routine)
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

#include"freertos/FreeRTOS.h"
#include"freertos/task.h"
#include"freertos/queue.h"

const int LED =  LED_VERMELHO;    // Pino do LED
const int BT = BOTAO_6;           // Pino do Botão

QueueHandle_t xFila;              // Cria a fila
TaskHandle_t xTask1Handle;        // Cria a TaskHandle_t

void vTask1(void *pvParameters);  // Protótipo da Task1

// Função de callback da interrupção
void trataISR_BT(){
  static int valor;                     // Variável estática para contar o número de interrupções
  valor++;                              // Incrementa a variável
  xQueueSendFromISR(xFila,&valor,NULL); // Envia o valor para a fila a partir da ISR 
}



void setup() {
  Serial.begin(9600);                                             // Inicializa a Serial
  pinMode(LED,OUTPUT);                                            // Configura o LED como saída
  pinMode(BT,INPUT_PULLUP);                                       // Configura o Botão como entrada
  attachInterrupt(digitalPinToInterrupt(BT),trataISR_BT,FALLING); // Configura a interrupção para o Botão borda de descida

  xFila = xQueueCreate(255,sizeof(int));                          // Cria a fila com 255 elementos do tipo int
  xTaskCreate(vTask1,"Task 1",configMINIMAL_STACK_SIZE + 1024,NULL,1,&xTask1Handle); // Cria a Task1
}

void loop() {
  digitalWrite(LED,!digitalRead(LED));                        // Inverte o estado do LED
  vTaskDelay(pdMS_TO_TICKS(1000));                            // Delay de 1 segundo
}

void vTask1(void *pvParameters){

  int valorRecebido;                                          // Variável para armazenar o valor recebido da fila

  while(1){
    xQueueReceive(xFila,&valorRecebido,portMAX_DELAY);          // Aguarda um valor ser enviado para a fila
    Serial.println("BT Pressionado: " + String(valorRecebido)); // Imprime o valor recebido
  }
}