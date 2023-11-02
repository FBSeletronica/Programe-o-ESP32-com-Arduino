/*********************************************************
* Exemplo que demonstra o uso de filas para comunicação
* entre tarefas. Neste exemplo, uma tarefa é responsável
* por ler o estado de um botão e enviar o estado para
* uma fila. A outra tarefa é responsável por ler o estado
* da fila e acender ou apagar um LED.
* 
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

// LED is active high
#define LED    LED_VERMELHO
#define BUTTON BOTAO_6

static QueueHandle_t queue;

/*
  Tarefa para ler o estado do botão e enviar para a fila
*/
void le_botao_task(void *pvParameters) {
  
  unsigned long debounceDelay = 50; // Atraso para evitar o efeito de bounce
  unsigned long lastDebounceTime = 0; // Último tempo de debounce
  bool buttonState = HIGH;   // Estado atual do botão
  int lastButtonState = HIGH; // Último estado do botão

  pinMode(BUTTON,INPUT_PULLUP);
  
 while(1) 
 {
    int reading = digitalRead(BUTTON);                           // Lê o estado do botão

    if (reading != lastButtonState) {                               // Verifica se o estado do botão mudou
      lastDebounceTime = millis();                                  // Atualiza o tempo de debounce
    }

    if ((millis() - lastDebounceTime) > debounceDelay) {            // Verifica se o tempo de debounce expirou
      if (reading != buttonState) {                                 // Verifica se o estado atual da leitura é diferente do estado do botão
        buttonState = reading;                                      // Atualiza o estado do botão

        if (buttonState == LOW) {                                   // Verifica se o botão foi pressionado
          Serial.println("Botão pressionado");                      // Imprime no monitor serial
          xQueueSend(queue,&buttonState,1);                   // Envia o evento para a fila
        }
      }
    }
    lastButtonState = reading;                                      // Atualiza o último estado do botão
  }
}

/*
  Tarefa para acender ou apagar o LED
  conforme o evento recebido pela fila
*/
void led_task(void *pvParameters) 
{
  bool event, led = false;    // Variáveis para armazenar o evento e o estado do LED

  pinMode(LED,OUTPUT);        // Configura o pino do LED como saída
  digitalWrite(LED,led);      // Apaga o LED

  while(1) 
  {
    if(xQueueReceive(queue, &event,portMAX_DELAY) == pdTRUE){   // Aguarda um evento da fila
      if(event == LOW)                               // Verifica se o botão foi pressionado
      {
        Serial.println("Inverte o Estado do LED");                
        led ^= true;                     // Inverte o estado do LED
        digitalWrite(LED,led);           // Acende ou apaga o LED
      }
    }
  }
}

void setup() {
  Serial.begin(115200);  
  queue = xQueueCreate(1,sizeof(bool));

  xTaskCreate(
    le_botao_task,    // Função da tarefa
    "Task Le Botão",  // Nome da tarefa
    2048,             // Tamaho da pilha
    NULL,             // Não usa parâmetros
    1,                // Prioridade 1
    NULL              // Não retorna o handle
  );

  xTaskCreate(
    led_task,         // Função da tarefa
    "task led",       // Nome da tarefa            
    2048,             // Tamaho da pilha
    NULL,             // Não usa parâmetros
    1,                // Prioridade 1
    NULL              // Não retorna o handle
  );

}

void loop() {
  // Nada aqui
}
