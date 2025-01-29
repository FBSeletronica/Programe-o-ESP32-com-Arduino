/*
 * Exemplo de I2C para leitura do Sensor de temperatura LM75B
 * 
 * descrição: Este código é um exemplo de comunicação I2C com o sensor de temperatura LM75B
 * objetivo: Demonstrar o uso da I2C no modo mestre para leitura de dados de um sensor de temperatura
 *  
 * Autor: Fábio Souza
 * data: 11/10/2024
 */

#include <Wire.h>

#define LM75B_I2C_ADDRESS 0x48  // Endereço I2C padrão do LM75B
#define TEMPERATURE_REGISTER 0x00  // Registro de temperatura

void setup() {
    Serial.begin(115200);
    Wire.begin(); // Inicializa I2C no ESP32

    Serial.println("Iniciando leitura do sensor LM75B...");
}

void loop() {
    float temperatura = lerTemperaturaLM75B();
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    delay(1000); // Aguarda 1 segundo antes da próxima leitura
}

float lerTemperaturaLM75B() {
    Wire.beginTransmission(LM75B_I2C_ADDRESS);
    Wire.write(TEMPERATURE_REGISTER);  // Seleciona o registro de temperatura
    Wire.endTransmission(false); // Mantém a comunicação ativa

    Wire.requestFrom(LM75B_I2C_ADDRESS, 2);  // Solicita 2 bytes

    if (Wire.available() == 2) {
        uint8_t msb = Wire.read(); // Byte mais significativo
        uint8_t lsb = Wire.read(); // Byte menos significativo

        int16_t rawTemperature = (msb << 8) | lsb; // Junta os bytes
        rawTemperature >>= 7; // Mantém apenas os 9 bits significativos

        return rawTemperature * 0.5; // Converte para Celsius
    }

    return NAN; // Retorna NaN se houver erro na leitura
}
