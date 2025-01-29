/*
 * Exemplo com de I2C scanner para verificar endereços no barramento I2C
 * 
 * descrição: Este exemplo verifica os endereços dos dispositivos conectados ao barramento I2C
 * objetivo: Demonstrar o uso básico da comunicação I2C
 *  
 * Autor: Fábio Souza
 * data: 11/10/2024
 */

#include <Wire.h> // Inclui a biblioteca Wire.h

void setup() {
  
  Serial.begin(115200);                   // Inicializa a comunicação serial
  Wire.begin();                           // Inicializa a comunicação I2C
  Wire.setClock(400000);                  // Configura a velocidade de comunicação I2C    
  Serial.println("ESP32 - I2C Scanner");  // Imprime na serial

  delay(5000);                            // Aguarda 5 segundos

}

void loop() {
  
  // Inicia a varredura dos endereços
  for (byte i = 1; i < 127; i++) {              // Varre os endereços de 1 a 127
    Wire.beginTransmission(i);                  // Inicia a transmissão
    if (Wire.endTransmission() == 0)            // Se o endereço for encontrado
    {
      Serial.print("Endereço encontrado: 0x");  // Imprime na serial
      Serial.println(i, HEX);                   // Imprime na serial
    }
  }
  Serial.println("Fim da varredura");           // Imprime na serial

  delay(5000);                                  // Aguarda 5 segundos

}
