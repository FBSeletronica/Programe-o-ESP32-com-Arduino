/*
 * Exemplo de I2C no modo Master com ESP32 para controle de outro ESP32 no modo Slave
 * 
 * descrição: Este código é para o ESP32 no modo Master, que envia comandos para um ESP32 no modo Slave.
 *            O Slave possui 3 LEDs, um ADC e um gerador de números aleatórios.
 *            O Master envia comandos para ligar e desligar os LEDs, e solicita leituras do ADC e do gerador de números aleatórios.
 *            O Slave responde com os valores lidos.
 * objetivo: Demonstrar a comunicação I2C entre dois ESP32.
 * 
 * Este código é parte do curso "Programe o ESP32 com Arduino" da Embarcados: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 *  
 * Autor: Fábio Souza
 * data: 11/10/2024
 */

#include <Wire.h>

#define SLAVE_ADDRESS 0x10      // Endereço do Slave
#define REG_LEDS      0x00      // Registrador para controle dos LEDs
#define REG_ADC       0x01      // Registrador para leitura do ADC
#define REG_RANDOM    0x02      // Registrador para leitura do valor aleatório

void setup() {
    Serial.begin(115200);
    Wire.begin(); // Inicializa o ESP32 como Master

    Serial.println("ESP32 Master pronto.");
}

void loop() {
    Serial.println("Enviando controle dos LEDs...");
    
    escreverRegistro(REG_LEDS, 0b00000001); // Liga apenas LED1
    delay(2000);

    escreverRegistro(REG_LEDS, 0b00000010); // Liga apenas LED2
    delay(2000);

    escreverRegistro(REG_LEDS, 0b00000100); // Liga apenas LED3
    delay(2000);

    escreverRegistro(REG_LEDS, 0b00000111); // Liga todos os LEDs
    delay(2000);

    escreverRegistro(REG_LEDS, 0b00000000); // Apaga todos os LEDs
    delay(2000);

    // Solicita leitura do ADC
    Serial.println("Solicitando leitura do ADC...");
    int valorADC = lerRegistro16(REG_ADC);
    Serial.print("Valor ADC recebido: ");
    Serial.println(valorADC);

    // Solicita leitura do valor aleatório
    Serial.println("Solicitando leitura do valor aleatório...");
    int valorAleatorio = lerRegistro16(REG_RANDOM);
    Serial.print("Valor aleatório recebido: ");
    Serial.println(valorAleatorio);

    delay(2000);
}

void escreverRegistro(uint8_t registrador, uint8_t valor) {
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(registrador); // Especifica qual registrador será escrito
    Wire.write(valor);       // Escreve o valor no registrador
    Wire.endTransmission();

    Serial.print("Registro 0x");
    Serial.print(registrador, HEX);
    Serial.print(" atualizado com valor: 0x");
    Serial.println(valor, HEX);
}

int lerRegistro16(uint8_t registrador) {
    // Primeiro, escreve o registrador que será lido
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(registrador);
    Wire.endTransmission();  // Agora o Slave sabe que deve responder com esse registrador

    // Agora solicita 2 bytes do Slave
    Wire.requestFrom(SLAVE_ADDRESS, 2);

    if (Wire.available() == 2) {
        uint8_t msb = Wire.read();
        uint8_t lsb = Wire.read();
        return (msb << 8) | lsb; // Combina os dois bytes em um valor de 16 bits
    }

    return -1; // Retorna -1 em caso de erro
}




