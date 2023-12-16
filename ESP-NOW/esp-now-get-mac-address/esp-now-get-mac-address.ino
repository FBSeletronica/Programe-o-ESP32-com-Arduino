/*
 * Exemplo Get MAC Address
 * Descrição: Esse código demonstra como pegar o MAC Address do ESP32
 * 
 * Autor: Fábio Souza
 * Data: 14/12/2023
*/

#include "WiFi.h"
 
void setup(){
  Serial.begin(115200);
  delay(2000);
}
 
void loop(){
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
  delay(10000);
}
