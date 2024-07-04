/*
 * Exemplo com sensor de temperatura ds18b20
 * 
 * descrição: Este exemplo exibe como ler o sensor ds18b20
 * objetivo: Demonstrar o uso do sensor ds18b20
 *  
 * Autor: Fábio Souza
 * data: 06/09/2023
 */

#include <OneWire.h>              // Inclui a biblioteca OneWire para comunicação com dispositivos 1-Wire
#include <DallasTemperature.h>    // Inclui a biblioteca DallasTemperature para leitura de sensores de temperatura DS18B20

const int oneWireBus = 16;        // Define o pino digital onde o barramento 1-Wire está conectado
OneWire oneWire(oneWireBus);      // Cria uma instância da biblioteca OneWire usando o pino definido
DallasTemperature sensors(&oneWire); // Cria uma instância da biblioteca DallasTemperature, passando o objeto OneWire

void setup() {
  Serial.begin(115200);          // Inicializa a comunicação serial com uma taxa de 115200 bps (bits por segundo)
  sensors.begin();               // Inicializa a biblioteca DallasTemperature, preparando-a para uso
}

void loop() {
  sensors.requestTemperatures();                    // Solicita a leitura das temperaturas de todos os sensores no barramento 1-Wire
  float temperatureC = sensors.getTempCByIndex(0); // Obtém a temperatura do primeiro sensor em graus Celsius
  float temperatureF = sensors.getTempFByIndex(0); // Obtém a temperatura do primeiro sensor em graus Fahrenheit
  Serial.print(temperatureC);                       // Imprime a temperatura em Celsius na porta serial
  Serial.println("ºC");                             // Imprime "ºC" na porta serial para indicar que o valor anterior é em Celsius
  Serial.print(temperatureF);                       // Imprime a temperatura em Fahrenheit na porta serial
  Serial.println("ºF");                             // Imprime "ºF" na porta serial para indicar que o valor anterior é em Fahrenheit
  delay(5000);                                      // Aguarda 5 segundos antes de repetir o loop
}
