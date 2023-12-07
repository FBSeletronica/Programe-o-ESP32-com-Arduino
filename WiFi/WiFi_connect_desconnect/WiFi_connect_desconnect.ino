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

const int WIFI_CONNECTING =   LED_AZUL;  // WIFI_CONNECTING LED
const int WIFI_CONNECTED =    LED_VERDE;  // WIFI_CONNECTED LED
const int WIFI_DISCONNECTED = LED_VERMELHO;  // WIFI_DISCONNECTED LED
const int BUTTON_DISCONNECT = BOTAO_6;    // Pino do Botão de Conexão

#include <WiFi.h>

const char* ssid     = "Franzininho";      // SSID da rede WiFi
const char* password = "88891711";  // Senha da rede WiFi  

int tryDelay = 1000;            // Delay entre as tentativas de conexão
int numberOfTries = 10;         // Número de tentativas de conexão
bool wifi_connected = false;    // Variável para controlar o estado da conexão WiFi

enum WiFiStatus { DISCONNECTED, CONNECTING, CONNECTED }; // Enumeração para os status da conexão WiFi

void update_led_status(WiFiStatus status) {
  switch (status) {
    case DISCONNECTED:
      digitalWrite(WIFI_DISCONNECTED, HIGH);              // Desliga o LED de Conectando
      digitalWrite(WIFI_CONNECTING, LOW);                 // Liga o LED de Conectando
      digitalWrite(WIFI_CONNECTED, LOW);                  // Desliga o LED de Conectado
      break;
    case CONNECTING:
      digitalWrite(WIFI_DISCONNECTED, LOW);               // Liga o LED de Conectando
      digitalWrite(WIFI_CONNECTING, HIGH);                // Desliga o LED de Conectando
      digitalWrite(WIFI_CONNECTED, LOW);                  // Desliga o LED de Conectado
      break;
    case CONNECTED:
      digitalWrite(WIFI_DISCONNECTED, LOW);               // Desliga o LED de Conectando
      digitalWrite(WIFI_CONNECTING, LOW);                 // Liga o LED de Conectando
      digitalWrite(WIFI_CONNECTED, HIGH);                 // Desliga o LED de Conectado
      break;
  }
}

void connectWifi(int tryDelay, int numberOfTries)
{
  
  // Aguarda a conexão com a rede WiFi e verifica os eventos
    while (true) {
        
        // Verifica se a conexão foi estabelecida
        switch(WiFi.status()) 
        {
          case WL_NO_SSID_AVAIL:  // SSID não encontrado                  
            Serial.println("[WiFi] SSID não encontrado");
            break;
          case WL_CONNECT_FAILED: // Falha na conexão
            Serial.print("[WiFi] Falha - WiFi não Conectador! Razão: "); 
            update_led_status(DISCONNECTED);            // Sinazliza que está desconectado
            return; // Sai da função
            break;
          case WL_CONNECTION_LOST: // Conexão perdida
            Serial.println("[WiFi] Conexão perdida");
            break;
          case WL_SCAN_COMPLETED: // Scan finalizado
            Serial.println("[WiFi] Scan finalizado");
            break;
          case WL_DISCONNECTED:  // Desconectado
            Serial.println("[WiFi] WiFi está desconectado");
            break;
          case WL_CONNECTED:    // Conectado
            Serial.println("[WiFi] WiFi está conectado");  
            Serial.print("[WiFi] IP: "); 
            Serial.println(WiFi.localIP());               // Imprime o endereço IP
            update_led_status(CONNECTED);                 // sinaliza que está conectado
            wifi_connected = true;                        // Atualiza a variável de controle
            return;                                       // Sai da função
            break;                                        
          default:
            Serial.print("[WiFi] WiFi Status: ");         // Imprime o status da conexão    
            Serial.println(WiFi.status());                // Imprime o status da conexão
            break;
        }
        delay(tryDelay);                                        // Aguarda o tempo de reconexão
        
        if(numberOfTries <= 0)
        {
          Serial.print("[WiFi] Conexão do WiFi Falhou");        // Informa que falhou a conexão
          WiFi.disconnect();                                    // Usa a função para desconectar e desabilitar as tentativas de reconexão
          update_led_status(DISCONNECTED);                      // Sinaliza que está desconectado
          return;                                               // Sai da função       
        } 
        else 
        {
          update_led_status(CONNECTING);                      // Inicializa o LED de status
          numberOfTries--;                                      // Decrementa o número de tentativas                   
        }
    }
}

void setup()
{
    Serial.begin(115200);                       // Inicializa a comunicação serial
    pinMode(BUTTON_DISCONNECT, INPUT_PULLUP);                 // Define o pino do botão desconect como entrada
    pinMode(WIFI_CONNECTING, OUTPUT);          // Define o pino do LED de Conectando como saída
    pinMode(WIFI_CONNECTED, OUTPUT);           // Define o pino do LED de Conectado como saída
    pinMode(WIFI_DISCONNECTED, OUTPUT);        // Define o pino do LED de Desconectado como saída
    delay(2000);                                // Aguarda 2 segundos para abrir o monitor serial
    update_led_status(DISCONNECTED);            // Sinaliza que está desconectado
    Serial.println("Pressione o Botão para conectar na Rede WiFI");  
    delay(2000);                                                      // Aguarda 2 segundos     
}

void loop()
{
    int btnState = digitalRead(BUTTON_DISCONNECT);          // Lê o estado do botão desconectar
    if(btnState == LOW)                                     // Verifica se o botão foi pressionado
    {
      if(wifi_connected)
      {
        Serial.println("[WiFi] Desconectando da rede WiFi");  // Informa que vai desconectar  
        delay(1000);                                          // Aguarda 1 segundo               
        WiFi.disconnect(true, true);                          // Desconecta da rede WiFi e desabilita as tentativas de reconexão
        Serial.println("[WiFi] Desconectado da rede WiFi");   // Informa que desconectou
        update_led_status(DISCONNECTED);                      // Sinaliza que está desconectado
        wifi_connected = false;                               // Atualiza a variável de controle 
        delay(1000);                                          // Aguarda 1 segundo
      }
      else
      {
        // Conecta na rede WiFi
        Serial.println();                                                  
        Serial.print("[WiFi] Conectando na Rede: ");
        Serial.println(ssid);
        Serial.println();
        WiFi.begin(ssid, password);                           // Conecta na rede WiFi
        connectWifi(tryDelay, numberOfTries);                 // Aguarda a conexão com a rede WiFi
        delay(1000);                                          // Aguarda 1 segundo
      }
    }
}
