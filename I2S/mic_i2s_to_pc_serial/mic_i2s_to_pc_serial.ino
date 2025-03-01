/*
 * Exemplo com de I2s para leitura do microfone INMP441 e envio para o PC
 * 
 * descrição: Este código é um exemplo de utilização do microfone INMP441 com o ESP32 
 * utilizando a bilbioteca ESP_I2S e envio do áudio para o PC
 * objetivo: Fazer a leitura de audio e transmitir via serial
 *
 *  Este código é parte do curso "Programe o ESP32 com Arduino" do Embarcados: 
 *  https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 *  
 * Autor: Fábio Souza
 * data: 26/02/2025
 */

#include <ESP_I2S.h>

// Definição dos pinos do I2S
#define I2S_WS  35  // LRCLK (Word Select)
#define I2S_SCK 34  // BCLK (Bit Clock)
#define I2S_SD  33  // DIN (Data In)

// Configurações de áudio
#define SAMPLE_RATE 16000   // 16 kHz de taxa de amostragem
#define BUFFER_SIZE 1024    // 1024 amostras de 16 bits
#define BYTE_BUFFER_SIZE (BUFFER_SIZE * sizeof(int16_t)) // Tamanho do buffer em bytes

I2SClass i2s; // Objeto I2S

void setup() {
    Serial.begin(115200);

    // Configuração dos pinos do I2S
    i2s.setPins(I2S_SCK, I2S_WS, -1, I2S_SD, -1);

    // Inicializa o I2S no modo padrão, 16 bits, mono
    if (!i2s.begin(I2S_MODE_STD, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
        Serial.println("Erro ao inicializar I2S!");
        while (1);
    }

    Serial.println("I2S Inicializado! Enviando áudio para o PC via Serial...");
}

void loop() {
    static int16_t audio_buffer[BUFFER_SIZE];  // Buffer para armazenar áudio
    size_t bytes_read = 0;

    // Captura os dados do microfone
    while (bytes_read < BYTE_BUFFER_SIZE) {
        bytes_read += i2s.readBytes(((char*)audio_buffer) + bytes_read, BYTE_BUFFER_SIZE - bytes_read);
    }

    // Envia os dados de áudio via Serial (para captura no PC)
    Serial.write((uint8_t*)audio_buffer, bytes_read);
}
