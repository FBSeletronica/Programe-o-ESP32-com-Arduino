/*
 * Exemplo de saída digital 2
 * 
 * descrição: Este exemplo pisca 3 LED de forma sequencial
 * objetivo: Demonstrar o uso de saída digital
 * 
 * Autor: Fábio Souza
 * data: 06/09/2023
 */

#define LED_VERMELHO 14   // Pino do LED Vermelho
#define LED_VERDE 13      // Pino do LED Verde
#define LED_AZUL 12       // Pino do LED Azul

#define TEMPO 1000        //tempo para o delay

const int ledPin[] = {LED_VERMELHO,LED_VERDE,LED_AZUL}; // vetor para armazenar os pinos dos LED

void setup() {
  for(int i =0; i < sizeof(ledPin)/sizeof(ledPin[0]);i++){  // percorre o vetor de pinos
    pinMode(ledPin[i], OUTPUT);                             // Configura o pino como saída
  }
}

void loop() {
  for(int i =0; i < sizeof(ledPin)/sizeof(ledPin[0]);i++){  // percorre o vetor de pinos
   digitalWrite(ledPin[i], HIGH);                           // Escreve nível lógico alto no pino - Acende o LED
   delay(TEMPO);                                            // Aguarda tempo definido
   digitalWrite(ledPin[i], LOW);                            // Escreve nível lógico baixo no pino - Desliga o LED
  }
}
