/*
 * Exemplo de USB HID Mouse
 * 
 * descrição: Este exemplo mostra como usar a biblioteca USB HID Mouser
 * objetivo: Demonstrar o funcionamento do mouse USB HID com ESP32 em modo OTG
 * 
 * Autor: Fábio Souza
 * data: 07/04/2025
 */

#define LED_VERMELHO 14
#define LED_VERDE 13
#define LED_AZUL 12
#define BOTAO_1 7     // UP
#define BOTAO_2 6     // LEFT
#define BOTAO_3 5     // RIGHT
#define BOTAO_4 4     // DOWN
#define BOTAO_5 3     // Left Click
#define BOTAO_6 2     // Right Click
#define BUZZER  17

// incluindo as bibliotecas necessárias
#include "USB.h"
#include "USBHIDMouse.h"

USBHIDMouse Mouse;  // Instancia o mouse USB HID

// variaveis auxiliares para movimentação do mouse
int range = 5;
int scrollSpeed = 1;
int responseDelay = 10;


void setup() 
{
  //configura os pinos dos botões como entrada com pull-up interno
  pinMode(BOTAO_1, INPUT_PULLUP);
  pinMode(BOTAO_2, INPUT_PULLUP);
  pinMode(BOTAO_3, INPUT_PULLUP);
  pinMode(BOTAO_4, INPUT_PULLUP);
  pinMode(BOTAO_5, INPUT_PULLUP);
  pinMode(BOTAO_6, INPUT_PULLUP);

  // Configura os pino do LED Vermelho como saída
  // e inicializa o LED vermelho como desligado
  pinMode(LED_VERMELHO, OUTPUT);
  digitalWrite(LED_VERMELHO, LOW);

  // Configura o pino do buzzer como saída  
  pinMode(BUZZER, OUTPUT); 
  
  // Inicializa a comunicação USB e o mouse
  USB.begin();
  Mouse.begin();
}

void loop() 
{
  // Variáveis para armazenar os valores de movimento do mouse
  int x = 0;
  int y = 0;
  int wheel = 0;
  int pan = 0;

  // Verifica o estado dos botões e ajusta os valores de movimento do mouse
  if (digitalRead(BOTAO_2) == LOW) x -= range;
  if (digitalRead(BOTAO_3) == LOW) x += range;
  if (digitalRead(BOTAO_1) == LOW) y -= range;
  if (digitalRead(BOTAO_4) == LOW) y += range;

  // Se botões 1 e 4 forem pressionados faz Scroll
  if (digitalRead(BOTAO_1) == LOW && digitalRead(BOTAO_4) == LOW) {
    wheel = scrollSpeed;
    feedback();
  }

  // Se botões 2 e 3 forem pressionados faz Pan
  if (digitalRead(BOTAO_2) == LOW && digitalRead(BOTAO_3) == LOW) {
    pan = scrollSpeed;
    feedback();
  }

  // Se qualquer movimento for detectado, move o mouse
  if (x != 0 || y != 0 || wheel != 0 || pan != 0) {
    Mouse.move(x, y, wheel, pan);
  }

  // Verifica o estado dos botões de clique left e executa a ação correspondente
  if (digitalRead(BOTAO_5) == LOW) {
    Mouse.click(MOUSE_LEFT);
    feedback();
    delay(200);
  }

  // Verifica o estado do botão 6 de clique right e executa a ação correspondente
  if (digitalRead(BOTAO_6) == LOW) {
    Mouse.click(MOUSE_RIGHT);
    feedback();
    delay(200);
  }

  // delay para evitar leituras muito rápidas (ajustar conforme necessário)
  delay(responseDelay);
}

// Função para emitir um beep no buzzer
void beep() 
{
  tone(BUZZER, 2000);
  delay(100); // Mantém o buzzer ligado por 100 ms
  noTone(BUZZER);
}

// Função para feedback visual e sonoro
void feedback() 
{
  digitalWrite(LED_VERMELHO, HIGH);
  delay(50);
  digitalWrite(LED_VERMELHO, LOW);
  beep();
}
