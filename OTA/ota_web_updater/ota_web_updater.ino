/*
 * Exemplo HTTP Web updater
 * 
 * descrição: Este exemplo mostra como fazer ataulizações OTA localmente
 * usando a biblioteca HTTPUpdateServer.h
 * 
 * Autor: Fábio Souza
 * data: 16/04/2025
 */

//Bibliotecas necessárias
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <HTTPUpdateServer.h>
#include <DHT.h>
#include <Preferences.h>
#include "FS.h"
#include "SPIFFS.h"
#include "credentials.h"

// ==== VERSÃO DO FIRMWARE ====
#define FIRMWARE_VERSION "v1.0.0"

// ==== CONFIGURAÇÕES Wi-Fi e OTA ====
// Defina as credenciais de Wi-Fi e OTA no arquivo credentials.h
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

const char* host_name = "esp32-ota";
const char* ota_user = OTA_USERNAME;
const char* ota_pass = OTA_PASSWORD;

// ==== SENSOR DHT11 ====
#define DHTPIN 15                       // Pino onde o DHT11 está conectado
#define DHTTYPE DHT11                   // DHT 11
DHT dht(DHTPIN, DHTTYPE);               // Inicializa o sensor DHT11
unsigned long previousMillis_DHT11 = 0; // armazena o tempo atual
const long interval_DHT11 = 30000;      // intervalo do piscar (ms)

// ==== LED ====
#define LED_PIN 33                      // Pino onde o LED está conectado
unsigned long previousMillis_LED = 0;   // armazena o tempo atual
const long interval_LED = 250;          // intervalo do piscar (ms)


WebServer server(80);             // Cria um servidor web na porta 80
HTTPUpdateServer httpUpdater;     // Cria um objeto HTTPUpdateServer
Preferences prefs;                // Cria um objeto Preferences para armazenar a versão do firmware

// Função para piscar o LED
void piscarLed() {
  static bool estado = false;               // Estado do LED (ligado/desligado)
  unsigned long currentMillis = millis();   // Armazena o tempo atual

  if (currentMillis - previousMillis_LED >= interval_LED) {   // Verifica se o intervalo de piscar foi atingido
    previousMillis_LED = currentMillis;                       // Atualiza o tempo atual
    estado = !estado;                                         // Inverte o estado do LED                      
    digitalWrite(LED_PIN, estado);                            // Atualiza o estado do LED      
  }
}

// Função para ler o sensor DHT11
// e exibir os valores de temperatura e umidade no monitor serial
void lerDHT11(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_DHT11 >= interval_DHT11) {
    previousMillis_DHT11 = currentMillis;
    // Leitura da temperatura e umidade
    float temperatura = dht.readTemperature();  // Celsius por padrão
    float umidade = dht.readHumidity();

    // Verifica se houve erro na leitura
    if (isnan(temperatura) || isnan(umidade)) {
        Serial.println("Falha na leitura do sensor DHT11!");    // Exibe mensagem de erro
        return;
    }

    // Exibe os valores lidos no monitor serial
    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
  }
}

// Função para inicializar o SPIFFS
void initSPIFFS(){
  if(!SPIFFS.begin(true)){
    Serial.println("Erro ao montar SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS montado com sucesso");
  }
}

// Função para conectar ao Wi-Fi
void connectToWiFi(){
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Conectando ao Wi-Fi");

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

// Função para configurar o mDNS
void setupMDNS() {
  if (MDNS.begin(host_name)) {
    Serial.println("mDNS iniciado: http://" + String(host_name) + ".local");
    MDNS.addService("http", "tcp", 80);
  } else {
    Serial.println("Erro ao iniciar mDNS");
  }
}

// Função para salvar a versão do firmware na NVS
void saveFirmwareVersion(const char* version) {
  prefs.begin("fw-info", false);  // modo escrita
  prefs.putString("version", version);
  prefs.end();
}

// Função para ler a versão do firmware da NVS
String readFirmwareVersion() {
  prefs.begin("fw-info", true);  // modo leitura
  String version = prefs.getString("version", "indefinida");
  prefs.end();
  return version;
}

void setupWebServer(){
  //Página principal
  server.on("/", []() {
      
    String html = "<!DOCTYPE html><html><head><meta charset='UTF-8'><title>ESP32</title></head><body>";
    html += "<h2>ESP32 OTA</h2>";
    html += "<link rel='icon' href='/favicon.ico' type='image/x-icon'>";
    html += "<p>Versão atual do firmware: " + String(FIRMWARE_VERSION) + "</p>";
    html += "<p>Versão armazenada na NVS: " + readFirmwareVersion() + "</p>";
        
    html += "<p><a href='/update'>Atualizar firmware</a></p>";
    html += "<p><a href='/mensagem'>Ver mensagem da SPIFFS</a></p>";
    html += "</body></html>";
  
    server.sendHeader("Content-Type", "text/html; charset=utf-8");
    server.send(200, "text/html", html);
  });

  //Página OTA com autenticação
  httpUpdater.setup(&server, "/update", ota_user, ota_pass);

  //Página mensagem
  server.on("/mensagem", HTTP_GET, []() {
    File file = SPIFFS.open("/mensagem.txt", "r");
    if (!file || file.isDirectory()) {
      server.send(404, "text/plain", "Arquivo /mensagem.txt não encontrado");
      return;
    }

    String conteudo = file.readString();
    file.close();
    server.send(200, "text/plain", conteudo);
  });

  //Página favicon
  server.on("/favicon.ico", HTTP_GET, []() {
    File iconFile = SPIFFS.open("/favicon.ico", "r");
    if (!iconFile || iconFile.isDirectory()) {
      server.send(404, "text/plain", "Favicon não encontrado");
      return;
    }
    server.streamFile(iconFile, "image/x-icon");
    iconFile.close();
  });

  server.begin();
  Serial.println("Servidor Web iniciado.");
}
  

void setup() {
  delay(5000);
  Serial.begin(115200);
  Serial.println("\nIniciando ESP32...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN,LOW);

  dht.begin();  
  initSPIFFS();
  connectToWiFi();
  setupMDNS();
  setupWebServer();
  saveFirmwareVersion(FIRMWARE_VERSION);
}

void loop() {
  server.handleClient();
  piscarLed();
  lerDHT11();
}
