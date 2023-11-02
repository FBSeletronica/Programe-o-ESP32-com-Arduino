/*********************************************************
* Exemplo para demonstrar o uso de semaforo binário
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

const int LED = LED_VERMELHO; //LED que será utilizado

TaskHandle_t xTaskADCHandle;    //Handle da Task do ADC
SemaphoreHandle_t xSemaphore;   //Handle do semaforo

void vTaskADC(void * pvParameters); //Task do ADC

void setup() 
{

  Serial.begin(9600);                     //Inicializa a Serial
  pinMode(LED,OUTPUT);                    //Configura o LED como saída

  xSemaphore = xSemaphoreCreateBinary();  //Cria o semaforo binário
  //Verifica se o semaforo foi criado com sucesso
  if(xSemaphore == NULL){
   Serial.println("Não foi possível criar o semaforo!");
   while(1);
  }
  //Cria a Task do ADC
  xTaskCreate(vTaskADC,"TaskADC",configMINIMAL_STACK_SIZE + 1024, NULL,1,&xTaskADCHandle);
  
}

void loop() {
  digitalWrite(LED,!digitalRead(LED));
  Serial.println("Valor do LED: " + String(digitalRead(LED)));
  vTaskDelay(pdMS_TO_TICKS(3000));
  xSemaphoreGive(xSemaphore);
}

void vTaskADC(void * pvParameters){

  int adcValue;

  while(1){
   xSemaphoreTake(xSemaphore,portMAX_DELAY);
   adcValue = analogRead(1);
   Serial.println("Valor do ADC: " + String(adcValue));
  }
}
