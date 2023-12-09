
/*
 * Exemplo WiFi web Server
 * Descrição: Esse código demonstra como criar um servidor web com o ESP32
 * 
 * Autor: Fábio Souza
 * Data: 09/11/2023
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

const byte BT = BOTAO_6;            // Pino do Botão
const byte LED = LED_VERMELHO;      // Pino do LED

boolean statusLED = LOW;           // Status do LED  (LOW = Desligado / HIGH = Ligado) 

#include <WiFi.h>                 //Biblioteca para conexão WiFi

//Credenciais de rede WiFi
const char* ssid     = "Franzininho";
const char* password = "88891711";
const char* hostname = "FranzininhoLAB01";    // Nome do Hostname
enum WiFiStatus { DISCONNECTED, CONNECTED }; // Enumeração para os status da conexão WiFi

WiFiServer server(80);

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

void setup()
{
    Serial.begin(115200);
    pinMode(BT,INPUT_PULLUP);
    pinMode(LED,OUTPUT);
    pinMode(WIFI_CONNECTED, OUTPUT);           // Define o pino do LED de Conectado como saída
    pinMode(WIFI_DISCONNECTED, OUTPUT);        // Define o pino do LED de Desconectado como saída


    delay(5000);
    
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

    server.begin(); //Inicia o servidor web

}

void loop()
{
 WiFiClient client = server.available();        //Verifica se há clientes conectados ao servidor
  if(!client){                                  //Se não houver clientes conectados, retorna ao loop
    return;
  }

  while(!client.available()){                   //Aguarda a disponibilidade do cliente     
    delay(1);
  }

  String req = client.readStringUntil('\r');    //Lê a requisição do cliente
  Serial.println(req);                          //Imprime a requisição no monitor serial          
  client.flush();                               //Limpa o cliente

  if(req.indexOf("LEDon") != -1){               //Se a requisição contiver "LEDon" liga o LED
    digitalWrite(LED,HIGH);                     //Liga o LED
    statusLED = HIGH;                           //Atualiza o status do LED
  }
  else if(req.indexOf("LEDoff")!=-1){           //Se a requisição contiver "LEDoff" desliga o LED
          digitalWrite(LED,LOW);                //Desliga o LED
          statusLED = LOW;                      //Atualiza o status do LED
  }

  //Envia a página HTML para o cliente
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<meta http-equiv='refresh' content='2'>");
  client.println("<h1>Web Server ESP32</h1>");
  client.println("<br></br>");

  if(digitalRead(BT) == LOW){                   //Se o botão for pressionado
   client.println("<p>BOTAO PRESSIONADO</p>");  //Imprime no cliente "BOTAO PRESSIONADO"
  }
  else{
    client.println("<p>BOTAO SOLTO</p>");       //Imprime no cliente "BOTAO SOLTO"
  }

  if(!statusLED)
  client.println("<p>LED <a href=\"LEDon\"><button>LIGAR</button></a></p>");      //Imprime no cliente "LED LIGADO"
  else
  client.println("<p>LED <a href=\"LEDoff\"><button>DESLIGAR</button></a></p>");  //Imprime no cliente "LED DESLIGADO"

  client.println("</html>");                                                      //Fim da página HTML
}