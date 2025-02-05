/*
 * Exemplo para leitura do sensor BMP180
 * 
 * descrição: Este código lê a temperatura, pressão e altitude do sensor BMP180
 * objetivo: Exibir as informações de temperatura, pressão e altitude no monitor serial
 * 
 * Bibliotecas:
 * - Adafruit BMP085
 * - Wire
 * 
 * Este código é parte do curso "Programe o ESP32 com Arduino" da Embarcados: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
  *  
 * Autor: Fábio Souza
 * data: 04/02/2025
 */
#include <Wire.h>  // Biblioteca para comunicação I2C
#include <Adafruit_BMP085.h>  // Biblioteca para o sensor BMP180

Adafruit_BMP085 bmp;  // Criando o objeto BMP180

void setup() 
{
    Serial.begin(115200);  // Inicializa a comunicação serial
    Wire.begin();  // Inicia o barramento I2C

    if (!bmp.begin()) {  // Verifica se o sensor BMP180 foi inicializado corretamente
        Serial.println("Erro ao inicializar o BMP180. Verifique a conexão!");
        while (1);  // Se houver erro, entra em loop infinito
    }
    Serial.println("BMP180 detectado com sucesso!");  // Mensagem de sucesso
}

void loop() 
{
    float temperatura = bmp.readTemperature();  // Lê a temperatura do sensor
    int32_t pressao = bmp.readPressure();  // Lê a pressão do sensor
    float altitude = bmp.readAltitude();  // Calcula a altitude com base na pressão

    Serial.print("Temperatura: ");  // Imprime a temperatura no serial
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Pressão: ");  // Imprime a pressão no serial
    Serial.print(pressao / 100.0);  // Converte a pressão para hPa
    Serial.println(" hPa");

    Serial.print("Altitude estimada: ");  // Imprime a altitude estimada no serial
    Serial.print(altitude);
    Serial.println(" m");

    Serial.println("------------------------------");  // Linha separadora
    delay(2000); // Aguarda 2 segundos antes de fazer a próxima leitura
}
