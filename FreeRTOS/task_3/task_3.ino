/***************************************************************
 *  Exemplo para parar e reiniciar uma task
 *  Esse exemplo exibe como  parar e reiniciar tasks no FreeRTOS
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
#define LED LED_VERMELHO

/* Variáveis para armazenamento do handle das tasks*/
TaskHandle_t task1Handle = NULL;
TaskHandle_t task2Handle = NULL;

/*protítipos das Tasks*/
void vTask1(void *pvParameters);
void vTask2(void *pvParameters);

void setup() {
  Serial.begin(9600);
  xTaskCreate(vTask1,"TASK1",configMINIMAL_STACK_SIZE,NULL,1,&task1Handle);       //task1 com prioridade 1
  xTaskCreate(vTask2,"TASK2",configMINIMAL_STACK_SIZE+1024,NULL,2,&task2Handle);  //task2 com prioridade 2

}

void loop() {
  Serial.println("Mensagem do loop: ");    // imprime mensagem
  vTaskDelay(3000);                       // delay de 3 segundos
}

/*
vTask1 
inverte LED em intervalos de 200 ms
*/
void vTask1(void *pvParameters)
{
    pinMode(LED,OUTPUT);                    // configura pino do LED como saída

    while (1)
    {
      digitalWrite(LED,!digitalRead(LED));  // inverte o estado do LED
      vTaskDelay(pdMS_TO_TICKS(200));       // delay de 200 ms
    }
}

/*
vTask2
Imprime mensagem a cada 1 segundo
suspende a task1 quando cont=10
reinicia a task1 quando cont=15
*/

void vTask2(void *pvParameters)
{
  int cont = 0;

  while (1)
  {
    Serial.println("Task 2: " + String(cont++));  // imprime mensagem

    if(cont==10){                                 // se cont=10
      Serial.println("Supendendo a Task 1...");   // imprime mensagem
      digitalWrite(LED,LOW);                      // desliga o LED
      vTaskSuspend(task1Handle);                  // suspende a task1
    }
    else if(cont==15){                            // se cont=15
      Serial.println("Reiniciando a Task 1...."); // imprime mensagem
      vTaskResume(task1Handle);                   // reinicia a task1  
      cont = 0;                                   // zera cont
    }

    vTaskDelay(pdMS_TO_TICKS(1000));              // delay de 1 segundo
  }
}
