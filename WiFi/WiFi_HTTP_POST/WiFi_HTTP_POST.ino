/*
 * Exemplo WiFi HTTP POST
 * Descrição: Esse código demonstra como enviar valores para o ThingSpeak usando 
 * o protocolo HTTP POST
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

const int WIFI_CONNECTED =    21;  // WIFI_CONNECTED LED
const int WIFI_DISCONNECTED = 33;  // WIFI_DISCONNECTED LED

// Inclui as bibliotecas necessárias
#include <WiFi.h>
#include <HTTPClient.h>

// credenciais da rede WiFi
const char* ssid     = "Franzininho";
const char* password = "88891711";
const char* hostname = "FranzininhoLAB01";    // Nome do Hostname
enum WiFiStatus { DISCONNECTED, CONNECTED }; // Enumeração para os status da conexão WiFi


// credenciais do ThingSpeak
const char* serverName = "http://api.thingspeak.com/update";
String apiKey = "XHF3QTMI01HM41R6";   //troque pelo sua Write API Key

//variáveis para controle do tempo
unsigned long lastTime = 0;
unsigned long updateTime = 30000;

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

void setup() {
  Serial.begin(115200);

  pinMode(WIFI_CONNECTED, OUTPUT);           // Define o pino do LED de Conectado como saída
  pinMode(WIFI_DISCONNECTED, OUTPUT);        // Define o pino do LED de Desconectado como saída

  delay(5000);                               // Aguarda 5 segundos para inicializar o WiFi

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

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  randomSeed(analogRead(1));  //inicializa o gerador de números aleatórios para 
                              //o valor do pino analógico 1
}

void loop() {
  
  //Envia dados para o ThingSpeak a cada X segundos (con figuração no updateTime)
  if ((millis() - lastTime) > updateTime) 
  {
    //envia apenas se estiver conectado
    if(WiFi.status()== WL_CONNECTED)  
    {
      WiFiClient client;        //cria um cliente WiFi
      HTTPClient http;          //cria um cliente HTTP

      int temp = random(40);    //gera um valor aleatório para a temperatura
      int humid = random(100);  //gera um valor aleatório para a umidade
    
      // Conecta ao servidor HTTP
      http.begin(client, serverName);
    
      //Monta a requisição HTTP
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "api_key=" + apiKey + "&field1=" + String(temp) 
                                + "&field2=" + String(humid) + "";           
      
      //imprime a requisição HTTP na serial
      Serial.print("httpRequestData: ");  
      Serial.println(httpRequestData);

      // Envia a requisição HTTP POST
      int httpResponseCode = http.POST(httpRequestData);
      
      // libera os recursos
      http.end();
    }
    lastTime = millis();  //atualiza o lastTime
  }
}
