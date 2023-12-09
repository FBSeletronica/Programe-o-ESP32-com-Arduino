/*
 * Exemplo WiFi Reconnect events
 * Descrição: Esse código demonstra como usar os eventos do WiFi para fazer a
 * reconexão automática com o ponto de acesso WiFi.
 * 
 * Autor: Fábio Souza
 * Data: 08/11/2023
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

const int WIFI_CONNECTED =    LED_VERDE;  // WIFI_CONNECTED LED
const int WIFI_DISCONNECTED = LED_VERMELHO;  // WIFI_DISCONNECTED LED

#include <WiFi.h>
 
const char* ssid = "Franzininho";             // Nome da rede WiFi
const char* password = "88891711";            // Senha do WiFi

const char* hostname = "FranzininhoLAB01";    // Nome do Hostname

enum WiFiStatus { DISCONNECTED, CONNECTED }; // Enumeração para os status da conexão WiFi

void update_led_status(WiFiStatus status) {
  switch (status) {
    case DISCONNECTED:
      digitalWrite(WIFI_DISCONNECTED, HIGH);              // Desliga o LED de Conectando
      digitalWrite(WIFI_CONNECTED, LOW);                  // Desliga o LED de Conectado
      break;
    case CONNECTED:
      digitalWrite(WIFI_DISCONNECTED, LOW);               // Desliga o LED de Conectando
      digitalWrite(WIFI_CONNECTED, HIGH);                 // Desliga o LED de Conectado
      break;
  }
}

//Evento WiFi Conectado
void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  update_led_status(CONNECTED);
  Serial.println("Conectato com sucesso ao WiFi!");
}

//Evento WiFi IP
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
  Serial.println("WiFi Conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
}

//Evento WiFi Desconectado
void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info)
{
  update_led_status(DISCONNECTED);
  Serial.println("Desconectado do WiFi");
  Serial.print("WiFi perdeu a conexão. Razão: ");
  Serial.println(info.wifi_sta_disconnected.reason);
  Serial.println("Tentando reconectar");
  WiFi.begin(ssid, password);
}

void setup(){

  Serial.begin(115200);     //Inicializa a comunicação serial

  pinMode(WIFI_CONNECTED, OUTPUT);           // Define o pino do LED de Conectado como saída
  pinMode(WIFI_DISCONNECTED, OUTPUT);        // Define o pino do LED de Desconectado como saída

  WiFi.mode(WIFI_STA);
  WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  WiFi.setHostname(hostname); //define hostname
  WiFi.disconnect(true);    //Desconecta de qualquer rede WiFi
  delay(1000);              //Delay de 1s    

  //Registra os eventos do WiFi
  WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
  WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
  WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

  //Conecta na rede WiFi
  WiFi.begin(ssid, password);
  //mensagem no monitor serial  
  Serial.println();
  Serial.println();
  Serial.println("Conectando no WiFi ..");
}

void loop(){
  delay(1000);    //não faz nada no loop
}