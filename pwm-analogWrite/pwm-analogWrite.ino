/*
 * Exemplo de uso do PWM com a função analogWrite
 * Descrição: Este exemplo demonstra o uso da função analogWrite para controlar o brilho de um LED.
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
#define BUZZER  17        // Pino do Buzzer

//definições para o PWM
#define PWM_RESOLUTION 8    // Define a resolução do PWM
#define PWM_MAX_VALUE 255   // Define o valor máximo do PWM
#define PWM_FREQ  1000      // Define a frequência do PWM

// Define o número do pino GPIO para o LED
const int ledPin = LED_VERMELHO; // Troque para o número do seu pino GPIO para o LED

int brightness = 0;    // Variável para armazenar o brilho do LED
int fadeAmount = 5;    // Quantidade de aumento/diminuição de brilho a cada ciclo

void setup() {
  pinMode(ledPin, OUTPUT);                  // Configura o pino do LED como saída
  analogWriteResolution(PWM_RESOLUTION);    // Define a resolução do PWM  
  analogWriteFrequency(PWM_FREQ);           // Define a frequência do PWM    
}

void loop() {
  analogWrite(ledPin, brightness); // Define o brilho do LED usando PWM

  brightness = brightness + fadeAmount; // Incrementa o brilho

  // Inverte a direção do aumento/diminuição de brilho
  if (brightness <= 0 || brightness >= PWM_MAX_VALUE) {   //se atingiu o valor máximo ou mínimo
    fadeAmount = -fadeAmount;                             //inverte o valor do fadeAmount
  }

  delay(30); // Aguarda um curto período de tempo antes de alterar o brilho
}