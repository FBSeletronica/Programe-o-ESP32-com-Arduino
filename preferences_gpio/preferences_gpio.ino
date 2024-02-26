/*
 * Exemplo Preferences
 * Descrição: Esse código exibe como usar a biblioteca Preferences para salvar 
 * dados permanentes na memoria flash do ESP32.
 * O exemplo exibe como salvar o estado de um LED na memória flash e recuperar o
 * valor quando o ESP32 é reiniciado. * 
 * 
 * Autor: Fábio Souza
 * Data: 14/12/2023
*/

//mapa de pinos da Franzininho WiFi LAB01
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


#include <Preferences.h>

Preferences dados;            //cria objeto para tratar os dados permanentes

const int buttonPin = BOTAO_6;      // Pino do Botào
const int ledPin = LED_VERMELHO;        // Pino do LED

//variáveis auxiliares
bool ledState;                    //estado do LED
bool buttonState;                 //estado do botão
int lastButtonState = LOW;        //estado do anterior do botão

//variáveis para debounce
unsigned long lastDebounceTime = 0;  // ultimo tempo de debounce
unsigned long debounceDelay = 50;    // tempo de debounce

void setup() 
{ 
  Serial.begin(115200);                                       // Inicializa a comunicação serial
  pinMode(buttonPin, INPUT_PULLUP);                           //configura o pino do botão como entrada
  pinMode(ledPin, OUTPUT);                                    //configura o pino do LED como saída

  dados.begin("GPIO", false);                                       //inicializa a memória flash
  ledState = dados.getBool("state", false);                         //recupera o estado do LED da memória flash
  Serial.printf("Estado do LED antes do Reset: %d \n", ledState);   //imprime valor lido na serial
  digitalWrite(ledPin, ledState);                                   //atualiza o estado do LED com o valor lido
}

void loop() 
{
  int reading = digitalRead(buttonPin);                         //lê o estado do botão

  if (reading != lastButtonState)                               //verifica se o estado do botão mudou
  {
    lastDebounceTime = millis();                                //atualiza o tempo do debounce
  }
  if ((millis() - lastDebounceTime) > debounceDelay)            //verifica se o tempo de debounce foi atingido
  {
    if (reading != buttonState)                                 //verifica se o estado do botão é diferente do estado atual
    {
      buttonState = reading;                                    //atualiza o estado do botão
      if (buttonState == LOW)                                   //verifica se o botão foi pressionado
      {
        ledState = !ledState;                                   //inverte o estado do LED
      }
    }
  }
  lastButtonState = reading;                                    //atualiza o estado do botão

  if (digitalRead(ledPin)!= ledState)                           //verifica se o estado do LED foi alterado
  {  
    Serial.println("Estado alterado");                          //imprime mensagem na serial
    digitalWrite(ledPin, ledState);                             //atualiza o estado do LED
    dados.putBool("state", ledState);                           //salva o estado atual do LED na memória flash
    Serial.printf("Estado salvo: %d \n", ledState);              //imprime valor salvo na serial 
  }
}
