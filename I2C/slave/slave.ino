/*
 * Exemplo de comunicação I2C no modo Slave
 * 
 * descrição: Este exemplo demonstra o uso da comunicação I2C no modo Slave com o ESP32.
 *            O ESP32 é configurado como Slave e pode receber comandos do Master para controlar LEDs,
 *            ler o valor do ADC e enviar um valor aleatório.
 *            O Master pode enviar comandos para controlar os LEDs, ler o valor do ADC e solicitar um valor aleatório.
 * objetivo: Receber comandos do Master para controlar LEDs, ler o valor do ADC e enviar um valor aleatório.
 * 
 * 
 * Registros:
 * 0x00 - Controle dos LEDs
 * 7 6 5 4 3 2 1 0
 * | | | | | | | |
 * | | | | | | | +--- LED3
 * | | | | | | +----- LED2
 * | | | | | +------- LED1
 * | | | | +--------- Reservado
 * | | | +----------- Reservado
 * | | +------------- Reservado
 * | +--------------- Reservado
 * +----------------- Reservado
 * 0x01 - Leitura do ADC
 * 0x02 - Envio de valor aleatório
 * 
 * 
 * Este código é parte do curso "Programe o ESP32 com Arduino" da Embarcados: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 * 
 * 
 * Autor: Fábio Souza
 * data: 11/10/2024
 */

#include <Wire.h>


#define SDA_PIN 15
#define SCL_PIN 16

#define SLAVE_ADDRESS 0x10      // Endereço I2C do ESP32 Slave
#define REG_LEDS      0x00      // Registrador para controle dos LEDs
#define REG_ADC       0x01      // Registrador para leitura do ADC
#define REG_RANDOM    0x02      // Registrador para envio de valor aleatório

#define LED1 12  // Pino do LED 1
#define LED2 13  // Pino do LED 2
#define LED3 14  // Pino do LED 3
#define ADC_PIN 1 // Pino do ADC

uint8_t registradorLEDs = 0x00;  // Registrador para armazenar o estado dos LEDs
uint8_t registradorSelecionado = 0x00; // Armazena qual registrador será lido

// Protótipos das funções de callback
void receberDados(int quantidade);
void enviarDados();

void setup() {
    Serial.begin(115200);         // Inicializa comunicação serial
    Wire.setPins(SDA_PIN, SCL_PIN);    // Configura pinos SDA e SCL
    Wire.begin(SLAVE_ADDRESS);    // Inicializa comunicação I2C no modo Slave
    Wire.onReceive(receberDados); // Callback para receber dados do Master
    Wire.onRequest(enviarDados);  // Callback para enviar dados ao Master

    // Configura pinos dos LEDs como saída
    pinMode(LED1, OUTPUT);
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);

    Serial.println("ESP32 Slave pronto.");  
}

void loop() {
    
}

/*
* Função para receber dados do Master
*/
void receberDados(int quantidade) 
{
    if (Wire.available()) {
        registradorSelecionado = Wire.read();                     // Armazena o registrador solicitado

        if (Wire.available())                                     // Se houver um segundo byte, é uma escrita no registrador
        { 
            uint8_t valor = Wire.read();                          // Lê o valor do registrador
            
            if (registradorSelecionado == REG_LEDS)               // Controle de LEDs
            { 
                registradorLEDs = valor;                          // Atualiza o registrador de LEDs
                Serial.print("Registrador LEDs atualizado: 0x");  // Exibe o valor do registrador
                Serial.println(registradorLEDs, HEX);             // Exibe o valor do registrador

                // Atualiza LEDs conforme o registrador
                digitalWrite(LED1, registradorLEDs & 0b00000001 ? HIGH : LOW);
                digitalWrite(LED2, registradorLEDs & 0b00000010 ? HIGH : LOW);
                digitalWrite(LED3, registradorLEDs & 0b00000100 ? HIGH : LOW);

                // Exibe o estado dos LEDs
                Serial.print("LED1: "); Serial.println((registradorLEDs & 0b00000001) ? "ON" : "OFF");
                Serial.print("LED2: "); Serial.println((registradorLEDs & 0b00000010) ? "ON" : "OFF");
                Serial.print("LED3: "); Serial.println((registradorLEDs & 0b00000100) ? "ON" : "OFF");
            }
        }
    }
}

/*
* Função para enviar dados ao Master
*/
void enviarDados() {
    if (registradorSelecionado == REG_ADC)            // Solicitação do ADC
    { 
        int valorADC = analogRead(ADC_PIN);           // Lê o valor do ADC
        Serial.print("Enviando valor ADC: ");         // Exibe o valor do ADC
        Serial.println(valorADC);                     // Exibe o valor do ADC

        Wire.write(highByte(valorADC));                // Envia MSB
        Wire.write(lowByte(valorADC));                 // Envia LSB
    } 
    else if (registradorSelecionado == REG_RANDOM)    // Solicitação de valor aleatório
    { 
        int valorAleatorio = random(0, 65535);        // Gera número aleatório de 0 a 65535
        Serial.print("Enviando valor aleatório: ");   // Exibe o valor aleatório
        Serial.println(valorAleatorio);               // Exibe o valor aleatório

        Wire.write(highByte(valorAleatorio));         // Envia MSB
        Wire.write(lowByte(valorAleatorio));          // Envia LSB
    }
}