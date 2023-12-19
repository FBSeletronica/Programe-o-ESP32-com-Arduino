/*
 * Exemplo ESP-NOW Receiver
 * Descrição: Esse código é um exemplo de como receber dados via ESP-NOW.
 * O valor recebido controla o estado de um LED
 * 
 * Autor: Fábio Souza
 * Data: 14/12/2023
*/
#define LED_VERMELHO 14   // Pino do LED Vermelho
#define LED_VERDE 13      // Pino do LED Verde
#define LED_AZUL 12       // Pino do LED Azul
#define BOTAO_1 7         // Pino do Botão 1
#define BOTAO_2 6         // Pino do Botão 2
#define BOTAO_3 5         // Pino do Botão 3
#define BOTAO_4 4         // Pino do Botão 4
#define BOTAO_5 3         // Pino do Botão 5
#define BOTAO_6 2         // Pino do Botão 6
#define BUZZER  17        // Pino Do Buzzer

#include <esp_now.h>
#include <WiFi.h>

const int ledVerdePin = LED_VERDE;

// Função de callback para receber dados
void esp_now_recv_callback(const uint8_t * mac, const uint8_t *incomingData, int len)
{
  //imprime mensagem recebida
  Serial.print("Mensagem recebida de: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.print(" | Dados: ");
  Serial.println((char*)incomingData);

  // Converte o valor recebido para inteiro
  int value = atoi((char*)incomingData);
  Serial.print("Valor inteiro: ");                
  Serial.println(value);

  // Atualiza o estado do LED
  digitalWrite(ledVerdePin, value ? HIGH : LOW); //Acende ou apaga o LED de acordo com o valor recebido
}
 
void setup() {
  
  Serial.begin(115200);             //Inicializa a comunicação serial
  pinMode(ledVerdePin, OUTPUT);     //Configura o pino do LED como saída

  WiFi.mode(WIFI_STA);              //Configura o WiFi como Station

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK)                   //Verifica se o ESP-NOW foi iniciado com sucesso
  {
    Serial.println("Erro ao iniciar o ESP-NOW");
    return;
  }
  
  // Configura a função de callback para receber dados
  esp_now_register_recv_cb(esp_now_recv_callback);
}
 
void loop() {

}
