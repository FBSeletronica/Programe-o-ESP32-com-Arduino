/*
 * Exemplo WiFi HTTP GET
 * Descrição: Esse código demonstra como fazer uma requisição HTTP GET
 * para uma API externa e obter os dados em formato JSON.
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

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

// credenciais da rede WiFi
const char* ssid     = "Franzininho";
const char* password = "88891711";
const char* hostname = "FranzininhoLAB01";    // Nome do Hostname
enum WiFiStatus { DISCONNECTED, CONNECTED }; // Enumeração para os status da conexão WiFi

//openweathermap.org API key
String openWeatherMapApiKey = "2631b64b1f9374c56bd15a3c87c19574"; //troque pela sua chave de API

//Informe a cidade e o código do país que queira obter os dados
String city = "Campinas";
String countryCode = "BR";

//variáveis para controle do tempo
unsigned long lastTime = 0;
unsigned long updateTime = 30000;

String jsonBuffer;  //variável para armazenar o JSON

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
}

void loop() {
  // Envia requisição a cada x segundos (definido em updateTime)
  if ((millis() - lastTime) > updateTime) 
  {
    //envia apenas se estiver conectado
    if(WiFi.status()== WL_CONNECTED)
    {
      WiFiClient client;
      HTTPClient http;
      
      //URL para obter os dados do clima
      String serverPath = "http://api.openweathermap.org/data/2.5/weather?q=" + city + "," + countryCode + "&APPID=" + openWeatherMapApiKey;
      
      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP POST request
      int httpResponseCode = http.GET();
      
      String payload = ""; 
      
      //se o código de resposta for maior que zero, significa que a requisição foi bem sucedida
      if (httpResponseCode>0) 
      {
        Serial.print("Código de Resposta HTTP: ");
        Serial.println(httpResponseCode);
        payload = http.getString();
        Serial.println(payload);
      }
      else  //se o código de resposta for menor ou igual a zero, significa que a requisição falhou
      {
        Serial.print("Código de Erro: ");
        Serial.println(httpResponseCode);
      }
      // Libera os recursos
      http.end();

      //Realiza a requisição HTTP GET
      jsonBuffer = payload;
      //Impressão do JSON no monitor serial
      Serial.println(jsonBuffer);
      //Realiza o parse do JSON
      JSONVar myObject = JSON.parse(jsonBuffer);
  
      // Se o JSON não for válido, retorna
      if (JSON.typeof(myObject) == "undefined") 
      {
        Serial.println("Falha ao analisar JSON");
        return;
      }
    
      //Imprime os dados do clima no monitor serial
      Serial.print("JSON object = ");
      Serial.println(myObject);
      Serial.print("Temperatura: ");
      Serial.println(myObject["main"]["temp"]);
      Serial.print("Pressão: ");
      Serial.println(myObject["main"]["pressure"]);
      Serial.print("Umidade: ");
      Serial.println(myObject["main"]["humidity"]);
      Serial.print("Velocidade do vento: ");
      Serial.println(myObject["wind"]["speed"]);
    }
    //atualiza o tempo
    lastTime = millis();
  }
}
