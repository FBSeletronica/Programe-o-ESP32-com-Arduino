/*
 * Exemplo para leitura do sensor DHT11
 * 
 * descrição: Este exemplo exibe como ler o sensor DHT11 e exibir os valores de temperatura e umidade no monitor serial
 * objetivo: Demonstrar como ler o sensor DHT11
 * 
 * 
 * Este código é parte do curso "Programe o ESP32 com Arduino" da Embarcados: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
  *  
 * Autor: Fábio Souza
 * data: 06/09/2023
 */

#include <DHT.h>    // Inclusão da biblioteca DHT

// Definição do pino onde o DHT11 está conectado
#define DHTPIN 15  // Altere conforme seu projeto

// Definição do tipo do sensor (podemos escolher entre DHT11, DHT22 ou DHT21)
#define DHTTYPE DHT11

// Inicializa o sensor DHT
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);                               // Inicializa o monitor serial
    Serial.println("Inicializando o DHT11...");         // Exibe mensagem de inicialização
    
    dht.begin();                                        // Inicia o sensor                    
}

void loop() {
    // Aguarda um intervalo entre as leituras
    delay(2000); 

    // Leitura da temperatura e umidade
    float temperatura = dht.readTemperature();  // Celsius por padrão
    float umidade = dht.readHumidity();

    // Verifica se houve erro na leitura
    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha na leitura do sensor DHT11!");    // Exibe mensagem de erro
        return;
    }

    // Exibe os valores lidos no monitor serial
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
}
