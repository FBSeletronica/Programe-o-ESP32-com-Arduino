/*********************************************************
* Exemplo para demonstrar o uso de semaforo contador dentro
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

const int LED =  LED_VERMELHO;    // Pino do LED
const int BT = BOTAO_6;           // Pino do Botão

SemaphoreHandle_t semaforoContador;     // handle do semaforo contador
TaskHandle_t taskTrataBTHandle;         // handle da task que trata o botão

void vTaskTrataBT(void * pvParameters); // protótipo da task que trata o botão


void ISR_CallBack(){

  BaseType_t xHighPriorityTaskWoken = pdTRUE;                       // variável para verificar se a task de maior prioridade foi liberada
  xSemaphoreGiveFromISR(semaforoContador,&xHighPriorityTaskWoken);  // libera o semaforo contador

}

void setup() {
  Serial.begin(9600);             // inicia a comunicação serial
  pinMode(LED,OUTPUT);            // configura o pino do LED como saída
  pinMode(BT,INPUT_PULLUP);       // configura o pino do botão como entrada com pullup  
  attachInterrupt(digitalPinToInterrupt(BT),ISR_CallBack,FALLING);  // configura a interrupção para o botão

  semaforoContador = xSemaphoreCreateCounting(255,0); // cria o semaforo contador
  xTaskCreate(vTaskTrataBT, "Task BT",configMINIMAL_STACK_SIZE + 1024,NULL,3,&taskTrataBTHandle); // cria a task que trata o botão

}

void loop() {
  digitalWrite(LED,HIGH);             // liga o LED
  vTaskDelay(pdMS_TO_TICKS(100));     // espera 100ms
  digitalWrite(LED,LOW);              // desliga o LED
  vTaskDelay(pdMS_TO_TICKS(1000));    // espera 1s
}

void vTaskTrataBT(void * pvParameters){

  UBaseType_t x;  // variável para armazenar o valor do semaforo contador

  while(1){
    xSemaphoreTake(semaforoContador,portMAX_DELAY); // pega o semaforo contador
    Serial.print("Tratando a ISR do BT: ");         // imprime a mensagem

    x = uxSemaphoreGetCount(semaforoContador);      // pega o valor do semaforo contador
    Serial.println(x);                              // imprime o valor do semaforo contador
    vTaskDelay(pdMS_TO_TICKS(1000));                // espera 1s
  }

}
