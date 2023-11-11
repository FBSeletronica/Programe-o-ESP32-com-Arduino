/*********************************************************
* Exemplo para demonstrar o uso de semaforo binário dentro
* de uma ISR
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

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

const int LED = LED_VERMELHO;   // Pino do LED
const int BT = BOTAO_6;         // Pino do Botão


SemaphoreHandle_t semaforo;         // handle do semaforo
TaskHandle_t taskTrataBTHandle;     // handle da task que trata o botão

void vTaskTrataBT(void * pvParameters); // protótipo da task que trata o botão


void ISR_CallBack(){

  BaseType_t xHighPriorityTaskWoken = pdTRUE;                 // variável para verificar se a task de maior prioridade deve ser executada
  xSemaphoreGiveFromISR(semaforo,&xHighPriorityTaskWoken);    // libera o semaforo e verifica se a task de maior prioridade deve ser executada

}

void setup() {
  Serial.begin(9600);                                             // inicia a comunicação serial
  pinMode(LED,OUTPUT);                                            // configura o pino do LED como saída  
  pinMode(BT,INPUT_PULLUP);                                       // configura o pino do botão como entrada com pullup
  attachInterrupt(digitalPinToInterrupt(BT),ISR_CallBack,FALLING); // configura a interrupção para o botão

  semaforo = xSemaphoreCreateBinary();                                                              // cria o semaforo binário
  xTaskCreate(vTaskTrataBT, "Task BT",configMINIMAL_STACK_SIZE + 1024,NULL,3,&taskTrataBTHandle);   // cria a task que trata o botão

  
}

void loop() {
  digitalWrite(LED,HIGH);                     // acende o LED
  vTaskDelay(pdMS_TO_TICKS(100));             // espera 100ms
  digitalWrite(LED,LOW);                      // apaga o LED
  vTaskDelay(pdMS_TO_TICKS(1000));            // espera 1s
}

void vTaskTrataBT(void * pvParameters){

  int contador = 0;                             // variável para contar o número de interrupções

  while(1){
    xSemaphoreTake(semaforo,portMAX_DELAY);     // aguarda o semaforo ser liberado
    Serial.println("x: " + String(contador++)); // imprime o número de interrupções
  }
}
