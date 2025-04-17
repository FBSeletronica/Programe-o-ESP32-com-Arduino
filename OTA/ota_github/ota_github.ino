/*
 * Exemplo OTA Avançado via HTTPS com firmware no GitHub
 *
 * Autor: Fábio Souza
 * Data: 16/04/2025
 */
#include "WiFi.h"
#include "HttpsOTAUpdate.h"
#include "DHT.h"
#include "credentials.h"

// Configurações de Wi-Fi
static const char *ssid = WIFI_SSID;
static const char *password = WIFI_PASSWORD;

// Configurações dos pinos
#define UPDATE_BUTTON_PIN 2
#define UPDATE_LED_PIN 21
#define LED_PIN 13
#define DHTPIN 15
#define DHTTYPE DHT11

// Debounce para o botão
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;
bool lastButtonState = HIGH;

// Controle do LED
unsigned long previousMillis_LED = 0;
const long interval_LED = 250; // intervalo para piscar LED (ms)

// Controle do DHT11
unsigned long previousMillis_DHT11 = 0;
const long interval_DHT11 = 30000; // intervalo de leitura DHT11 (ms)

DHT dht(DHTPIN, DHTTYPE);

// Controle OTA
static HttpsOTAStatus_t otastatus;
bool isOtaRunning = false; // Flag para saber se o OTA está em andamento

// Prototipação
void piscarLed();
void lerDHT11();
bool buttonPressed();
void iniciarOTA();
void connectToWiFi();
void HttpEvent(HttpEvent_t *event);

// Certificado e URL devem ser definidos aqui:
extern const char *server_certificate;
extern const char *url;


void setup() {
  delay(5000); // Tempo para abrir o serial monitor se precisar
  Serial.begin(115200);
  Serial.println("\nIniciando ESP32...");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(UPDATE_LED_PIN, OUTPUT);
  digitalWrite(UPDATE_LED_PIN, LOW);

  pinMode(UPDATE_BUTTON_PIN, INPUT_PULLUP);

  dht.begin();

  connectToWiFi();
  HttpsOTA.onHttpEvent(HttpEvent);

}

void loop() {
  // Mantém o LED piscando
  piscarLed();

  // Lê o sensor DHT11 periodicamente
  lerDHT11();

  // Se botão pressionado e OTA não estiver em andamento
  if(buttonPressed() && !isOtaRunning){
    Serial.println("Botão pressionado! Iniciando OTA...");
    iniciarOTA();
  }

  // Se OTA estiver em andamento, checa o status
  if(isOtaRunning){
    otastatus = HttpsOTA.status();
    if (otastatus == HTTPS_OTA_SUCCESS){
      Serial.println("Firmware atualizado com sucesso. Reiniciando dispositivo...");
      delay(1000);
      ESP.restart();
    }else if (otastatus == HTTPS_OTA_FAIL) {
      Serial.println("Falha na atualização do firmware!");
      isOtaRunning = false; // Permitir tentar novamente depois
      digitalWrite(UPDATE_LED_PIN,LOW);
    }
  }
}

void piscarLed() {
  static bool estado = false;
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_LED >= interval_LED) {
    previousMillis_LED = currentMillis;
    estado = !estado;
    digitalWrite(LED_PIN, estado);
  }
}

void lerDHT11() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis_DHT11 >= interval_DHT11) {
    previousMillis_DHT11 = currentMillis;

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();

    if (isnan(temperatura) || isnan(umidade)) {
      Serial.println("Falha na leitura do sensor DHT11!");
      return;
    }

    Serial.print("Temperatura: ");
    Serial.print(temperatura);
    Serial.println(" °C");

    Serial.print("Umidade: ");
    Serial.print(umidade);
    Serial.println(" %");
  }
}
bool buttonPressed() {
  bool reading = digitalRead(UPDATE_BUTTON_PIN);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == LOW) {
      lastButtonState = reading;
      return true;
    }
  }
  lastButtonState = reading;
  return false;
}

void iniciarOTA() {
  digitalWrite(UPDATE_LED_PIN,HIGH);
  Serial.println("Iniciando processo OTA...");
  isOtaRunning = true;
  HttpsOTA.begin(url, server_certificate);
}

void connectToWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Conectando ao Wi-Fi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi conectado!");
  Serial.print("IP local: ");
  Serial.println(WiFi.localIP());
}

void HttpEvent(HttpEvent_t *event) {
  switch (event->event_id) {
    case HTTP_EVENT_ERROR:        Serial.println("Http Event Error"); break;
    case HTTP_EVENT_ON_CONNECTED: Serial.println("Http Event On Connected"); break;
    case HTTP_EVENT_HEADER_SENT:  Serial.println("Http Event Header Sent"); break;
    case HTTP_EVENT_ON_HEADER:    
      Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
      break;
    case HTTP_EVENT_ON_DATA:      
      break;
    case HTTP_EVENT_ON_FINISH:    
      Serial.println("Http Event On Finish"); 
      break;
    case HTTP_EVENT_DISCONNECTED: 
      Serial.println("Http Event Disconnected"); 
      break;
    case HTTP_EVENT_REDIRECT:     
      Serial.println("Http Event Redirect"); 
      break;
  }
}
