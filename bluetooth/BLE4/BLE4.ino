/*
 * Exemplo de uso do Bluetooth Low Energy (BLE) no ESP32
 * Descrição: Este exemplo demonstra como monitorar a temperatura e umidade usando o DHT11 e controlar um LED via BLE.
 * 
 * Este exemplo foi desenvolvido para o curso Domine o ESP32 com Arduino: Da Base aos Projetos Avançados
 * LINK: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 * Autor: Fábio Souza
 * Data: 19/05/2025
*/

// Bibliotecas necessárias
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <DHT.h>

//Configurações do DHT11
#define DHTPIN 12
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Configurações do LED
#define LED_PIN 14

// UUIDs do serviço e características BLE
#define SERVICE_UUID            "12345678-1234-1234-1234-1234567890ab"
#define CHAR_SENSOR_UUID        "12345678-1234-1234-1234-1234567890ac"
#define CHAR_LED_CONTROL_UUID   "12345678-1234-1234-1234-1234567890ad"

// Ponteiros para as Características BLE 
BLECharacteristic* sensorCharacteristic;
BLECharacteristic* ledCharacteristic;

// Estado da conexão
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Callback para eventos de conexão e desconexão
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Dispositivo conectado.");
  }
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Dispositivo desconectado.");
  }
};

// Callback para controle do LED
class LEDControlCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override {
    String value = pCharacteristic->getValue();
    value.trim();
    if (value == "ON") {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("LED ligado via BLE.");
    } else if (value == "OFF") {
      digitalWrite(LED_PIN, LOW);
      Serial.println("LED desligado via BLE.");
    } else {
      Serial.print("Comando inválido: ");
      Serial.println(value);
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando BLE + DHT11 + LED...");

  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  // Inicializa o BLE
  BLEDevice::init("ESP32_BLE_Sensor_LED");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Configura Serviço
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Característica do sensor (notificação)
  sensorCharacteristic = pService->createCharacteristic(
    CHAR_SENSOR_UUID,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  sensorCharacteristic->addDescriptor(new BLE2902());

  // Característica para controle do LED
  ledCharacteristic = pService->createCharacteristic(
    CHAR_LED_CONTROL_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  ledCharacteristic->setCallbacks(new LEDControlCallback());
  ledCharacteristic->addDescriptor(new BLE2902());

  // Inicia Serviço
  pService->start();
  pServer->getAdvertising()->addServiceUUID(SERVICE_UUID);
  pServer->getAdvertising()->start();

  Serial.println("BLE pronto. Conecte via app para visualizar e controlar.");
}

void loop() {

  // Atualiza os valores do sensor a cada 5 segundos
  static unsigned long lastUpdate = 0;
  if (deviceConnected && millis() - lastUpdate > 5000) {
    float temp = dht.readTemperature();
    float hum = dht.readHumidity();

    if (!isnan(temp) && !isnan(hum)) {
      char msg[50];
      snprintf(msg, sizeof(msg), "T: %.1f C | U: %.1f %%", temp, hum);
      sensorCharacteristic->setValue(msg);
      sensorCharacteristic->notify();

      Serial.print("Sensor: ");
      Serial.println(msg);
    } else {
      Serial.println("Falha na leitura do DHT11");
    }

    lastUpdate = millis();
  }

  // Gerencia reconexão BLE
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    BLEDevice::getAdvertising()->start();
    Serial.println("Advertising reiniciado.");
    oldDeviceConnected = false;
  }

  if (deviceConnected && !oldDeviceConnected) {
    Serial.println("Novo dispositivo conectado.");
    oldDeviceConnected = true;
  }
}
