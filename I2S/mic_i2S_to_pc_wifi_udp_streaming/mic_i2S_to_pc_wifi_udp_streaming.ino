/*
 * Exemplo para streaming de audio via UDP
 * 
 * descrição: Este código é um exemplo de utilização do microfone INMP441 com o ESP32 
 * utilizando a bilbioteca ESP_I2S com streaming de audio via UDP
 * objetivo: Entender como aplicar o uso do mic em um projeto real
 *
 *  Este código é parte do curso "Programe o ESP32 com Arduino" do Embarcados: 
 *  https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 *  
 * Autor: Fábio Souza
 * data: 26/02/2025
 */


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP_I2S.h>
#include "esp_task_wdt.h"

// Configuração da Rede WiFi
const char *SSID = "XXXXXXXXX";      // Nome da rede WiFi
const char *PASSWORD = "XXXXXXXXX"; //Senha do WiFi

// Configuração do UDP
WiFiUDP udp;
const char* SERVER_IP = "192.168.0.73";  // IP do PC (ajuste conforme necessário)
const int UDP_PORT = 12345;

// Configuração do I2S (Microfone)
#define I2S_WS   35  // LRCLK (Word Select)
#define I2S_BCK  34  // BCLK (Bit Clock)
#define I2S_SD   33  // DIN (Data In)

#define SAMPLE_RATE 16000  // Taxa de amostragem
#define BUFFER_SIZE 1024   // Tamanho do buffer
#define BYTE_BUFFER_SIZE BUFFER_SIZE * sizeof(int16_t) // Buffer em bytes

// Buffer de áudio
int16_t audioBuffer[BUFFER_SIZE];

// Objeto I2S
I2SClass i2s;

// Handle da Task UDP
TaskHandle_t xHandleUDP = NULL;

// Configuração do I2S
void setupI2S() {
    i2s.setPins(I2S_BCK, I2S_WS, -1, I2S_SD, -1);
    if (!i2s.begin(I2S_MODE_STD, SAMPLE_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO)) {
        Serial.println("Erro ao inicializar I2S!");
        while (1);
    }
    Serial.println("I2S iniciado!");
}

// Configuração do WiFi
void setupWiFi() {
    Serial.print("Conectando ao WiFi...");
    WiFi.begin(SSID, PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nWiFi conectado!");
    Serial.print("Endereço IP: ");
    Serial.println(WiFi.localIP());

    udp.begin(UDP_PORT);
    Serial.println("UDP iniciado!");
}

// Task para transmissão UDP
void udpStreamTask(void *pvParameters) {
    while (1) {
        size_t bytesRead = 0;
        while (bytesRead < BYTE_BUFFER_SIZE) {
            bytesRead += i2s.readBytes(((char*)audioBuffer) + bytesRead, BYTE_BUFFER_SIZE - bytesRead);
        }

        // Verifica conexão WiFi antes de enviar
        if (WiFi.status() == WL_CONNECTED) {
            udp.beginPacket(SERVER_IP, UDP_PORT);
            int bytesSent = udp.write((uint8_t*)audioBuffer, bytesRead);
            int result = udp.endPacket();
            
            // Verifica se o pacote foi enviado com sucesso
            if (result == 1) {
                Serial.println("Pacote UDP enviado!");
            } else {
                Serial.printf("[ERRO] Falha ao enviar UDP! Código: %d\n", result);
                Serial.printf("[INFO] Bytes lidos: %d, Bytes enviados: %d\n", bytesRead, bytesSent);
            }
        } else {
            Serial.println("[ERRO] WiFi desconectado, tentando reconectar...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
        
        // Alimenta o watchdog
        esp_task_wdt_reset();

        // **Ajusta a taxa de envio** (evita sobrecarga no UDP)
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void setup() {
    Serial.begin(115200);
    setupWiFi();
    setupI2S();

    // Adiciona a tarefa principal ao Watchdog
    esp_task_wdt_add(NULL);

    // Criar a task para envio UDP em tempo real
    xTaskCreatePinnedToCore(
        udpStreamTask,    // Nome da função
        "UDP Stream Task", // Nome da task
        4096,             // Tamanho da stack
        NULL,             // Parâmetros
        1,                // Prioridade
        &xHandleUDP,      // Handle da task
        0                 // Rodar no Core 0
    );

    // Adiciona a task UDP ao watchdog
    esp_task_wdt_add(xHandleUDP);
}

void loop() {
    esp_task_wdt_reset(); // Mantém o watchdog ativo
    vTaskDelay(pdMS_TO_TICKS(1000)); // O loop principal não faz nada
}
