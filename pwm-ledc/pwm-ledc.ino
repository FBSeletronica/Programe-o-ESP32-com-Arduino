/*
 * Exemplo de uso do PWM com LEDC
 * Descrição: Este exemplo demonstra o uso do PWM com LEDC através do brilho de um LED
 * 
 * Autor: Fábio Souza
 * data: 06/09/2023
 * update:12/07/2025
 */

// Definições de pinos da placa Franzininho WiFi LAB01
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

// PWM com LEDC definições
#define LEDC_CHANNEL_0     0          // Número do canal LEDC
#define LEDC_TIMER_BITS    8          // Resolução do temporizador LEDC
#define PWM_MAX_VALUE      4095        // Valor máximo do PWM - 2^8 - 1
#define LEDC_BASE_FREQ     5000       // Frequência base do PWM

int brightness = 0;    // Variável para armazenar o brilho do LED
int fadeAmount = 5;    // Quantidade de aumento/diminuição de brilho a cada ciclo
const int ledPin = LED_VERMELHO; // Define o número do pino GPIO para o LED

void setup() {
  // Configura o pino para usar PWM com frequência e resolução definidas
  bool ok = ledcAttach(ledPin, LEDC_BASE_FREQ, LEDC_TIMER_BITS);
  if (!ok) {
    Serial.begin(115200);
    Serial.println("Erro ao configurar o LEDC!");
  }
}

void loop() {

  ledcWrite(ledPin, brightness);                                // define o brilho do LED

  brightness = brightness + fadeAmount;                         // altera o brilho para a próxima vez através do loop

  if (brightness <= 0 || brightness >= PWM_MAX_VALUE) {         // inverte a direção do fade no final do fade se brightness chegou nos limites 
    fadeAmount = -fadeAmount;                                   // inverte a direção do fade
  }
  delay(30);                                                    // aguarda 30 milissegundos para ver o efeito
}
