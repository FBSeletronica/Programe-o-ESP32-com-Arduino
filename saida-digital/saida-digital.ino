/*
 * Exemplo de saída digital
 * 
 * descrição: Este exemplo liga e desliga um LED a cada segundo
 * objetivo: Demonstrar o uso de saída digital
 * 
 * Autor: Fábio Souza
 * data: 06/09/2023
 */

// Define o número do pino GPIO que está conectado ao LED
const int ledPin = 13; // Troque para o número do seu pino GPIO

void setup() {
  pinMode(ledPin, OUTPUT);        // Configura o pino como saída
}

void loop() {
  digitalWrite(ledPin, HIGH);   // Escreve nível lógico alto no pino - Liga o LED
  delay(1000);                  // Aguarda um segundo    
  digitalWrite(ledPin, LOW);    // Escreve nível lógico baixo no pino - Desliga o LED
  delay(1000);                  // Aguarda um segundo
}
