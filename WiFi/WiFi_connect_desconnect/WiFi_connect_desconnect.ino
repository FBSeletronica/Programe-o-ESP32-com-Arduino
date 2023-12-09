/*
 * Exemplo WiFi STA Conectando e Desconectando
 * Descrição: Esse código demonstra como conectar e desconectar de uma rede WiFi
 * 
 * Autor: Fábio Souza
 * Data: 09/10/2023
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
const int BUTTON_DISCONNECT = BOTAO_6;    // Pino do Botão de Conexão

#include <WiFi.h>

const char* ssid = "Franzininho";   // SSID da rede WiFi
const char* password = "88891711";  // Senha da rede WiFi

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

void setup() {
  Serial.begin(115200);             // Inicializa a comunicação serial

  pinMode(BUTTON_DISCONNECT, INPUT_PULLUP);                 // Define o pino do botão desconect como entrada
  pinMode(WIFI_CONNECTED, OUTPUT);           // Define o pino do LED de Conectado como saída
  pinMode(WIFI_DISCONNECTED, OUTPUT);        // Define o pino do LED de Desconectado como saída

  update_led_status(DISCONNECTED);            // Sinaliza que está desconectado
  delay(5000);  

  // Conecta na rede WiFi
  Serial.println();                                                  
  Serial.print("[WiFi] Conectando na Rede: ");
  Serial.println(ssid);
  Serial.println();

  WiFi.mode(WIFI_STA);                      // Configura o WiFi para o modo Station (conecta na rede WiFi)
  WiFi.begin(ssid, password);               // Conecta na rede WiFi

  // Aguarda a conexão com a rede WiFi
  Serial.print("Conectando no WiFi ..");    
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print('.');
    delay(500);
  }


  // Mostra os dados da conexão
  Serial.println("");
  Serial.println("WiFi Conectado");
  Serial.println("IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("RSSI: ");
  Serial.println(WiFi.RSSI());

  update_led_status(CONNECTED);            // Sinaliza que está conectado
  
}

void loop() 
{
  
 if (WiFi.status() != WL_CONNECTED) 
 {
    update_led_status(DISCONNECTED);            // Sinaliza que está desconectado
    delay(5000);
    Serial.println("Reconectando ao WiFi...");
    WiFi.disconnect();
    WiFi.reconnect();

    // Aguarda a conexão com a rede WiFi
    Serial.print("Conectando no WiFi ..");    
    while (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print('.');
      delay(500);
    }


    // Mostra os dados da conexão
    Serial.println("");
    Serial.println("WiFi Conectado");
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("RSSI: ");
    Serial.println(WiFi.RSSI());
  }
  else
  {
    update_led_status(CONNECTED);            // Sinaliza que está conectado
  }

  if(digitalRead(BUTTON_DISCONNECT) == LOW)
  {
    Serial.println("Desconectando do WiFi...");
    update_led_status(DISCONNECTED);            // Sinaliza que está desconectado
    WiFi.disconnect(true, true);
    Serial.println("Desconectado!");
    delay(5000);
    //reset esp32
    //ESP.restart();
  }
}