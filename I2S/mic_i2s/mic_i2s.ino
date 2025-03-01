/*
 * Exemplo com de I2s para leitura do microfone INMP441 usando a biblioteca ESP_I2S
 * 
 * descrição: Este código é um exemplo de utilização do microfone INMP441 com o ESP32 
 * utilizando a bilbioteca ESP_I2S
 * objetivo: Entender o funcionamento bãsico da interface I2S
 *
 *  Este código é parte do curso "Programe o ESP32 com Arduino" do Embarcados: 
 *  https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 *  
 * Autor: Fábio Souza
 * data: 26/02/2025
 */

#include <ESP_I2S.h>

// Definição dos pinos (ajuste conforme sua conexão)
#define I2S_WS  35  // LRCLK (Word Select)
#define I2S_SCK 34  // BCLK (Bit Clock)
#define I2S_SD  33  // DIN (Data In)

// Configurações do áudio
#define SAMPLE_RATE 16000    // 16 kHz de taxa de amostragem
#define BUFFER_SIZE 1024     // 1024 amostras de 16 bits
#define BYTE_BUFFER_SIZE BUFFER_SIZE * sizeof(int16_t) // Tamanho em bytes do buffer de áudio (1024 * 2)

I2SClass i2s; // Objeto I2S

void setup() {
    Serial.begin(115200);
  
    // Configurar pinos I2S
    i2s.setPins(
    I2S_SCK,    // BCK
    I2S_WS,     // WS
    -1,         // DOUT (não usado)
    I2S_SD,     // DIN
    -1          // MCLK (opcional)
  );
  
  // Inicializar I2S no modo RX
  if (!i2s.begin(I2S_MODE_STD, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
    Serial.println("Erro ao inicializar I2S!");
    while(1); // Loop infinito em caso de falha
  }

}

void loop() {
  static int16_t audio_buffer[BUFFER_SIZE]; // Buffer para 1024 amostras de 16 bits

  // Leitura bloqueante (similar ao i2s_read)
  size_t bytes_read = 0;
  while(bytes_read < BYTE_BUFFER_SIZE) 
  {
    // Lê os dados do microfone e armazena no buffer de áudio (audio_buffer)
    bytes_read += i2s.readBytes(((char*)audio_buffer) + bytes_read,BYTE_BUFFER_SIZE - bytes_read );
  }

  // Envia para o Serial Plotter (downsampling 1:8 para facilitar a visualização)
  for(int i = 0; i < BUFFER_SIZE; i+=8) 
  {
    Serial.println(audio_buffer[i]);
  }
}