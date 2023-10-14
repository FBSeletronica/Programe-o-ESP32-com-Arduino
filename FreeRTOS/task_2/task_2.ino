/***************************************************************
 *  Exemplos para deletar tasks
 *  Esse exemplo exibe como criar tarefas no FreeRTOS
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
TaskHandle_t taks1Handle = NULL;
TaskHandle_t taks2Handle = NULL;

/*protítipos das Tasks*/
void vTask1(void *pvParameters);    
void vTask2(void *pvParameters);

/*função setup*/
void setup() {
  Serial.begin(9600); //configura comunicação serial com baudrate de 9600
  /*criação das tasks*/
  xTaskCreate(vTask1,"TASK1",configMINIMAL_STACK_SIZE,NULL,1,&taks1Handle);
  xTaskCreate(vTask2,"TASK2",configMINIMAL_STACK_SIZE+1024,NULL,2,&taks1Handle);
}

/*função loop*/
void loop() {
  Serial.println("Mensagem do loop: ");             // imprime mensagem
  vTaskDelay(3000);                                 //libera a CPU por 3 seg
}

/*
vTask1 
inverte LED em intervalos de 200 ms
*/
void vTask1(void *pvParameters)
{
    pinMode(LED,OUTPUT);                            // configura pino do LED como saída 

    while (1)   
    {
      digitalWrite(LED,!digitalRead(LED));          // inverte estado do LED
      vTaskDelay(pdMS_TO_TICKS(200));               // libera a CPU por 200 ms
    }
}

/*
vTask2 
imprime valor de contagem a cada 1 seg e deleta tasks
*/
void vTask2(void *pvParameters)
{
  int cont = 0;

  while (1)
  {
    Serial.println("Task 2: " + String(cont++));        // imprime valor de contagem

    if(cont>=10){                                       // se contagem for maior ou igual a 10
      if(task1Handle != NULL){                          // verifica se task1Handle é diferente de NULL
        Serial.println("Deletando Task 1");             // imprime mensagem
        vTaskDelete(task1Handle);                       // deleta task1
        digitalWrite(LED,LOW);                          // desliga LED    
        task1Handle = NULL;                             // atribui NULL a task1Handle
      }
    }
    if(cont>=15){
        Serial.println("Deletando Task 2");             // imprime mensagem
        vTaskDelete(NULL);                              // deleta task2
    }

    vTaskDelay(pdMS_TO_TICKS(1000));                    // libera a CPU por 1 seg
  }
}