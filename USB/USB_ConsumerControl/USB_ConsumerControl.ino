/*
 * Exemplo de entrada digital com debounce de tecla
 * 
 * descrição: Este exemplo faz a leitura de uma entrada digital conectada a um botão
 * e inverte o estado do LED
 * objetivo: Demonstrar o uso de entrada digital com rotina de debounce
 * 
 * Autor: Fábio Souza
 * data: 06/09/2023
 */

#define LED_VERMELHO 14   // Pino do LED Vermelho
#define LED_VERDE 13      // Pino do LED Verde
#define LED_AZUL 12       // Pino do LED Azul
#define BOTAO_1 7         // Pino do Botão 1
#define BOTAO_2 6         // Pino do Botão 2
#define BOTAO_3 5         // Pino do Botão 3
#define BOTAO_4 4         // Pino do Botão 4
#define BOTAO_5 3         // Pino do Botão 5
#define BOTAO_6 2         // Pino do Botão 6


#include "USB.h"
#include "USBHIDConsumerControl.h"
USBHIDConsumerControl ConsumerControl;

const int buttonVolIncPin = BOTAO_6;
int previousButtonVolIncState = HIGH;

const int buttonVolDecPin = BOTAO_5;
int previousButtonVolDecState = HIGH;

const int buttonPlayPausePin = BOTAO_4;
int previousButtonPlayPauseState = HIGH;

const int buttonNextPin = BOTAO_3;
int previousButtonNextState = HIGH;

const int buttonPrevPin = BOTAO_2;
int previousButtonPrevState = HIGH;

void setup() {
  pinMode(buttonVolIncPin, INPUT_PULLUP);
  pinMode(buttonVolDecPin, INPUT_PULLUP);
  pinMode(buttonPlayPausePin, INPUT_PULLUP);
  pinMode(buttonNextPin, INPUT_PULLUP);
  pinMode(buttonPrevPin, INPUT_PULLUP);
  ConsumerControl.begin();
  USB.begin();
}

void loop() {
  int buttonVolIncState = digitalRead(buttonVolIncPin);
  if ((buttonVolIncState != previousButtonVolIncState) && (buttonVolIncState == LOW)) {
    ConsumerControl.press(CONSUMER_CONTROL_VOLUME_INCREMENT);
    ConsumerControl.release();
  }
  previousButtonVolIncState = buttonVolIncState;


  int buttonVolDecState = digitalRead(buttonVolDecPin);
  if ((buttonVolDecState != previousButtonVolDecState) && (buttonVolDecState == LOW)) {
    ConsumerControl.press(CONSUMER_CONTROL_VOLUME_DECREMENT);
    ConsumerControl.release();
  }
  previousButtonVolDecState = buttonVolDecState;

  int buttonPlayPauseState = digitalRead(buttonPlayPausePin);
  if ((buttonPlayPauseState != previousButtonPlayPauseState) && (buttonPlayPauseState == LOW)) {
    ConsumerControl.press(CONSUMER_CONTROL_PLAY_PAUSE);
    ConsumerControl.release();
  }
  previousButtonPlayPauseState = buttonPlayPauseState;


  int buttonNextState = digitalRead(buttonNextPin);
  if ((buttonNextState != previousButtonNextState) && (buttonNextState == LOW)) {
    ConsumerControl.press(CONSUMER_CONTROL_SCAN_NEXT);
    ConsumerControl.release();
  }
  previousButtonNextState = buttonNextState;


  int buttonPrevState = digitalRead(buttonPrevPin);
  if ((buttonPrevState != previousButtonPrevState) && (buttonPrevState == LOW)) {
    ConsumerControl.press(CONSUMER_CONTROL_SCAN_PREVIOUS);
    ConsumerControl.release();
  }
  previousButtonPrevState = buttonPrevState;
}

