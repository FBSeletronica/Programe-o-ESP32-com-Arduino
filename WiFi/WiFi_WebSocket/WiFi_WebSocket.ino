/*
 * Exemplo Webserver websocket com ESP32
 * Descrição: Esse código demonstra como criar um websocket com ESP32
 * para controlar um LED e ler o estado de um botão e um LDR (sensor de luz).
 * Através de uma interface web, é possível ligar e desligar o LED,
 * verificar o estado do botão e ler o valor do LDR em tempo real.
 * A interface é atualizada automaticamente via WebSocket,
 * sem a necessidade de recarregar a página.
 * 
 * 
 * Autor: Fábio Souza
 * Data: 02/06/2025
*/

//Mapa de pinos da Franzininho WiFi LAB01
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
#define LDR 1


// Pinos para LEDs de conexão WiFi
const int WIFI_CONNECTED =    21;  // WIFI_CONNECTED LED
const int WIFI_DISCONNECTED = 33;  // WIFI_DISCONNECTED LED

// Mapeamento dos pinos
const byte BT = BOTAO_6;            // Pino do Botão
const byte LED = LED_VERMELHO;      // Pino do LED

//Bibliotecas necessárias
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Definição das credenciais WiFi
const char* ssid     = "Franzininho";
const char* password = "88891711";

// Criação do servidor web e WebSocket
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

// Variável para armazenar o estado do LED
bool statusLED = false;

// HTML da interface WebSocket
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-BR">
<head>
  <meta charset="UTF-8" />
  <title>Controle WebSocket</title>
  <style>
    body { font-family: sans-serif; text-align: center; padding-top: 40px; background: #f4f4f4; }
    h1 { color: #333; }
    #status, #ldr, #botao_estado {
      font-size: 1.2em; margin: 10px;
    }
    #botao {
      padding: 12px 24px;
      font-size: 1em;
      border: none;
      border-radius: 5px;
      cursor: pointer;
    }
    #botao.on { background: #c0392b; color: white; }
    #botao.off { background: #27ae60; color: white; }
  </style>
</head>
<body>
  <h1>Controle via WebSocket</h1>
  <p id="status">Conectando...</p>
  <button id="botao" onclick="toggleLED()">Carregando...</button>
  <p id="botao_estado">Botão: --</p>
  <p id="ldr">LDR: --</p>

<script>
  let estadoLED = "OFF";
  let ws = new WebSocket(`ws://${location.host}/ws`);
  ws.onmessage = e => {
    const partes = e.data.split(";");
    partes.forEach(p => {
      const [key, val] = p.split("=");
      if (key === "LED") {
        estadoLED = val;
        atualizarBotao();
      }
      if (key === "BT") {
        document.getElementById("botao_estado").innerText = "Botão: " + val;
      }
      if (key === "LDR") {
        document.getElementById("ldr").innerText = "LDR: " + val;
      }
    });
  };

  function toggleLED() {
    ws.send(estadoLED === "ON" ? "OFF" : "ON");
  }

  function atualizarBotao() {
    const btn = document.getElementById("botao");
    const status = document.getElementById("status");
    btn.textContent = estadoLED === "ON" ? "Desligar" : "Ligar";
    btn.className = estadoLED === "ON" ? "on" : "off";
    status.textContent = "LED está " + estadoLED;
  }
</script>
</body>
</html>
)rawliteral";


// Função para lidar com eventos do WebSocket
void onWebSocketMessage(AsyncWebSocket *server, AsyncWebSocketClient *client,
                        AwsEventType type, void *arg, uint8_t *data, size_t len) {
  if (type == WS_EVT_DATA) {
    String msg = "";
    for (size_t i = 0; i < len; i++) msg += (char)data[i];
    if (msg == "ON") setLed(true);
    if (msg == "OFF") setLed(false);
  }
  if (type == WS_EVT_CONNECT) {
    client->text("LED=" + String(statusLED ? "ON" : "OFF") +
                 ";BT=" + String(digitalRead(BT) == LOW ? "PRESSIONADO" : "SOLTO") +
                 ";LDR=" + String(analogRead(LDR)));
  }
}

// Função para notificar todos os clientes conectados com o estado atual
void notifyClients() {
  String msg = "LED=" + String(statusLED ? "ON" : "OFF");
  msg += ";BT=" + String(digitalRead(BT) == LOW ? "PRESSIONADO" : "SOLTO");
  msg += ";LDR=" + String(analogRead(LDR));
  ws.textAll(msg);
}

// Função para definir o estado do LED e notificar os clientes
void setLed(bool on) {
  statusLED = on;
  digitalWrite(LED, on ? HIGH : LOW);
  notifyClients();
}

//
void setup() {
  Serial.begin(115200);

  pinMode(LED, OUTPUT);
  pinMode(BT, INPUT_PULLUP);
  pinMode(LDR, INPUT);
  pinMode(WIFI_CONNECTED, OUTPUT);
  pinMode(WIFI_DISCONNECTED, OUTPUT);
  setLed(false);

  // Conexão com a rede WiFi
  WiFi.begin(ssid, password);
  Serial.println("Conectando no WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(WIFI_CONNECTED, LOW);
    digitalWrite(WIFI_DISCONNECTED, HIGH);
    delay(500);
    Serial.print(".");
  }

  // Conexão estabelecida
  digitalWrite(WIFI_CONNECTED, HIGH);
  digitalWrite(WIFI_DISCONNECTED, LOW);
  Serial.println("\nWiFi conectado! IP: " + WiFi.localIP().toString());

  // Configuração do servidor web e WebSocket
  ws.onEvent(onWebSocketMessage);                     // Define o evento de mensagem do WebSocket
  server.addHandler(&ws);                             // Adiciona o WebSocket ao servidor   
  // Rota para servir a página HTML
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", htmlPage);
  });

  server.begin(); // Inicia o servidor

}

unsigned long lastUpdate = 0;
void loop() {

  //notifica os clientes a cada 100ms com o estado atual do LED, botão e LDR
  if (millis() - lastUpdate > 100) {
    lastUpdate = millis();
    notifyClients(); // envia estado atualizado do LED, botão e LDR
  }

}
