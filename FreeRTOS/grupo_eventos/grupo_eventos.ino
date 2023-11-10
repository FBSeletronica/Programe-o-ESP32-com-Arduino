/*********************************************************
* Exemplo para demonstrar o uso de grupos de eventos
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
const int LED2 = 33;            // Pino do LED 2
const int BUTTON1 = BOTAO_6;    // Pino do Botão 1

/*inclusão das Biblioteca do FreeRTOS*/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"
#include "freertos/event_groups.h"

//mapeamento de eventos
#define TASK1_FLAG (1<<0)  //1
#define TASK2_FLAG (1<<1)  //10

TaskHandle_t xTask1Handle,xTask2Handle,xTask3Handle;    //variável para armazenar o handle da task
TimerHandle_t xTimer;                                   //variável para armazenar o handle do timer
EventGroupHandle_t xEventos;                            //variável para armazenar o handle do grupo de eventos

void vTask1(void *pvParameters);            //protótipo da task 1
void vTask2(void *pvParameters);            //protótipo da task 2
void vTask3(void *pvParameters);            //protótipo da task 3
void callBackTimer1(TimerHandle_t pxTimer); //protótipo da função de callback do timer

void setup(){
  Serial.begin(115200);     //inicializa a comunicação serial
  pinMode(LED1,OUTPUT);     //configura o pino do LED como saída

  /*Cria o grupo de eventos*/
  xEventos = xEventGroupCreate(); //cria o grupo de eventos

  /*Cria o timer*/
  xTimer = xTimerCreate("TIMER1",pdMS_TO_TICKS(1000),pdTRUE,0,callBackTimer1);  //cria o timer

  /*Cria a tasks*/
  xTaskCreate(vTask1,"TASK1",configMINIMAL_STACK_SIZE + 1024,NULL,1,&xTask1Handle); //cria a task 1
  xTaskCreate(vTask2,"TASK2",configMINIMAL_STACK_SIZE + 1024,NULL,1,&xTask2Handle); //cria a task 2
  xTaskCreate(vTask3,"TASK3",configMINIMAL_STACK_SIZE + 1024,NULL,1,&xTask3Handle); //cria a task 3

  xTimerStart(xTimer,0);  //inicia o timer

}

void loop(){
  digitalWrite(LED1,!digitalRead(LED1));    //inverte o estado do LED
  vTaskDelay(pdMS_TO_TICKS(500));           //delay de 500ms
}


void vTask1(void *pvParameters)
{
  EventBits_t xBits;  //variável para armazenar os bits do evento
  while (1)
  {
    xBits = xEventGroupWaitBits(xEventos,TASK1_FLAG,pdTRUE,pdTRUE,portMAX_DELAY); //espera o evento
    Serial.println("Task 1 saiu do estado de bloqueio: "+ String(xBits));         //imprime o evento
  }
}
void vTask2(void *pvParameters)
{
  EventBits_t xBits;  //variável para armazenar os bits do evento
  while (1)
  {
    xBits = xEventGroupWaitBits(xEventos,TASK2_FLAG,pdTRUE,pdTRUE,portMAX_DELAY); //espera o evento
    Serial.println("Task 2 saiu do estado de bloqueio: " + String(xBits));        //imprime o evento
  }
}

void vTask3(void *pvParameters)
{
  EventBits_t xBits;  //variável para armazenar os bits do evento
  while (1)
  {
    xBits = xEventGroupWaitBits(xEventos,TASK1_FLAG | TASK2_FLAG,pdTRUE,pdTRUE,portMAX_DELAY);  //espera o evento
    Serial.println("Task 3 saiu do estado de bloqueio: " + String(xBits));                      //imprime o evento
  }
}

void callBackTimer1(TimerHandle_t pxTimer)
{
    static int tempo = 0; //variável para contar o tempo

    tempo++;  //incrementa o tempo

    if(tempo == 5){                             //se o tempo for igual a 5
      xEventGroupSetBits(xEventos,TASK1_FLAG);  //seta o bit do evento 1
    }
    else if(tempo == 10){                       //se o tempo for igual a 10
      xEventGroupSetBits(xEventos,TASK2_FLAG);  //seta o bit do evento 2
    }
    else if(tempo == 15){                       //se o tempo for igual a 15
      tempo = 0;
      xEventGroupSetBits(xEventos,TASK1_FLAG|TASK2_FLAG); //seta o bit do evento 1 e 2
    }
}
