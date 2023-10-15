/***************************************************************
 *  Exemplo para passagem de parametros na criação da Task
 *  Por: Fábio Souza
***************************************************************/

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

/*mapeamento de pinos*/ 
#define LED1 LED_VERMELHO
#define LED2 21

/* Variáveis para armazenamento do handle das tasks*/
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;
TaskHandle_t task3Handle = NULL;

/*protítipos das Tasks*/
void vTaskBlink(void *pvParameters);  
void vTask2(void *pvParameters);

/*variáveis auxiliares*/
int valor = 500;

void setup() {
  Serial.begin(9600);
    
  xTaskCreate(vTaskBlink,"TASK1",configMINIMAL_STACK_SIZE,(void*)LED1,1,&task1Handle);    //Cria a Task1
  xTaskCreate(vTask2,"TASK2",configMINIMAL_STACK_SIZE+1024,(void*)valor,2,&task2Handle);  //Cria a Task2
  xTaskCreate(vTaskBlink,"TASK3",configMINIMAL_STACK_SIZE,(void*)LED2,1,&task3Handle);    //Cria a Task3
}

void loop() {
  Serial.println("Mensagem do loop: ");       //Mensagem do loop principal
  vTaskDelay(3000);                           //libera a CPU por 3 seg
}

/*
 Task pisca led
*/
void vTaskBlink(void *pvParameters)
{
    int pin = (int)pvParameters;            //Pega o valor passado como parâmetro
    pinMode(pin,OUTPUT);                    //Configura o pino como saída

    while (1)
    {
      digitalWrite(pin,!digitalRead(pin));   //Inverte o estado do LED
      vTaskDelay(pdMS_TO_TICKS(200));        //
    }
}

/*
  Task 2 - imprime o valor do 
  contador a cada 1 segundo
*/
void vTask2(void *pvParameters)
{
  int cont = (int)pvParameters;                   //Pega o valor passado como parâmetro

  while (1)
  {
    Serial.println("Task 2: " + String(cont++));  //Imprime o valor do contador
    vTaskDelay(pdMS_TO_TICKS(1000));              //libera a CPU por 1 segundo
  }
}
