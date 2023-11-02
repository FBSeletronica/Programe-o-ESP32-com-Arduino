/*********************************************************
* Exemplo para demonstrar o uso de MUTEX para acesso a recursos
* e a inversão de prioridade
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

SemaphoreHandle_t mutex;                //handle do semaforo mutex
TaskHandle_t task1Handle,task2Handle;   //handle das tasks

void vTask1(void * pvParameters);       //prototipo da Task1 (vTask1)
void vTask2(void * pvParameters);       //prototipo da Task2 (vTask2)

/*
  Função de envio de informação
  Recebe como parâmetro o número da task que está enviando a informação
*/
void enviaInformacao(char msg[],int i){
  xSemaphoreTake(mutex,portMAX_DELAY);                          //obtem o semaforo
  Serial.print(msg);
  delay(100);
  Serial.print(" : ");
  delay(200);
  Serial.println(i);
  //delay(1000);
  xSemaphoreGive(mutex);                                        //libera o semaforo

}

void setup() {

  Serial.begin(9600);               //inicia a comunicação serial
  pinMode(LED,OUTPUT);              //configura o pino do LED como saída
  mutex = xSemaphoreCreateMutex();  //cria o semaforo mutex
  xTaskCreate(vTask1,"Task1",configMINIMAL_STACK_SIZE + 1024,NULL,1,&task1Handle);  //cria a Task1 
  xTaskCreate(vTask2,"Task2",configMINIMAL_STACK_SIZE + 1024,NULL,4,&task2Handle);  //cria a Task2
}

void loop() {

  //pisca o LED
  digitalWrite(LED,HIGH);
  vTaskDelay(pdMS_TO_TICKS(100));
  digitalWrite(LED,LOW);
  vTaskDelay(pdMS_TO_TICKS(1000));
}



void vTask1(void * pvParameters)
{

  while(1)
  {
      enviaInformacao("Enviando informação da Task", 1);   //envia a informação
      vTaskDelay(pdMS_TO_TICKS(10)); //delay de 10ms 
  }
}

void vTask2(void * pvParameters)
{

  while(1)
  {
    enviaInformacao("Message form task",2);             //envia a informação
    vTaskDelay(pdMS_TO_TICKS(100));  //delay de 100ms       
  }
}
