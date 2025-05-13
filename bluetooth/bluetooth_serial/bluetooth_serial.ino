/*
 * Exemplo de uso do Bluetooth clássico como Serial
 * Descrição: Esse código demonstra o uso do bluetooth clássico
 * como seril. Controlamos o estado de um LED e recebemos mensagens
 * periodicas do device.
 * 
 * Autor: Fábio Souza
 * Data: 12/05/2025
*/

#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

#define LED_PIN 14
#define ADC_PIN 36

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32_BT");
  Serial.println("Bluetooth pronto. Emparelhe com ESP32_BT");
  pinMode(LED_PIN,OUTPUT);
}

void loop() {
  if (SerialBT.available()) {
    String comando = SerialBT.readStringUntil('\n');
    comando.trim();
    Serial.println("Recebido: " + comando);

    if (comando == "LED_ON") {
      digitalWrite(LED_PIN, HIGH);
      SerialBT.println("LED ligado!");
    } else if (comando == "LED_OFF") {
      digitalWrite(LED_PIN, LOW);
      SerialBT.println("LED desligado!");
    }
  }

  // Enviar mensagem periódica
  static unsigned long lastTime = 0;
  if (millis() - lastTime > 5000) {
    SerialBT.println("ESP32 online");
    SerialBT.println(analogRead(ADC_PIN));
    lastTime = millis();
  }
}
