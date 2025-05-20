/*
 * Exemplo de uso do Bluetooth Low Energy (BLE) no ESP32
 * Descrição: Este exemplo demonstra como criar um serviço BLE UART no ESP32.
 * Ele permite a comunicação entre o ESP32 e um smartphone usando o aplicativo nRF Connect ou Serial Bluetooth Terminal.
 * O ESP32 atua como servidor BLE e o smartphone como cliente.
 * O ESP32 pode enviar dados para o smartphone e receber dados do smartphone.
 * O serviço BLE UART é baseado no perfil GATT (Generic Attribute Profile).
 * O UUID do serviço e das características são definidos como constantes.
 * O ESP32 envia dados para o smartphone quando o usuário digita no monitor serial e pressiona Enter.
 * 
 * Este exemplo foi desenvolvido para o curso Domine o ESP32 com Arduino: Da Base aos Projetos Avançados
 * LINK: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 * Autor: Fábio Souza
 * Data: 19/05/2025
*/

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// UUIDs do serviço UART BLE
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Ponteiro para o servidor e característica
BLEServer* pServer = nullptr;
BLECharacteristic* pTxCharacteristic = nullptr;

// Variáveis de controle de conexão
bool deviceConnected = false;
bool oldDeviceConnected = false;

// Callback de conexão
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
    Serial.println("Cliente conectado.");
  }

  // Callback de desconexão
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
    Serial.println("Cliente desconectado.");
  }
};

// Callback de escrita (smartphone → ESP32)
class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override {
    String rx = pCharacteristic->getValue();

    // Verifica se a string recebida não está vazia
    if (rx.length() > 0) {
      Serial.println("Recebido do smartphone:");
      Serial.println(rx);

      // (Opcional) ecoa o texto de volta ao app
      pTxCharacteristic->setValue(rx);
      pTxCharacteristic->notify();
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando BLE UART...");

  // Inicializa o BLE
  BLEDevice::init("ESP32_UART_BLE");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Cria o serviço BLE
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Característica TX (ESP32 → Smartphone)
  pTxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );
  pTxCharacteristic->addDescriptor(new BLE2902());

  // Característica RX (Smartphone → ESP32)
  BLECharacteristic* pRxCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Inicia o serviço
  pService->start();  
  pServer->getAdvertising()->start();

  Serial.println("BLE UART pronto. Use nRF Connect ou Serial Bluetooth Terminal.");
}

void loop() {
  // Enviar texto do monitor serial para o smartphone(ESP32 → Smartphone)
  static String inputSerial = "";

  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      if (inputSerial.length() > 0 && deviceConnected) {
        Serial.print("Enviando para smartphone: ");
        Serial.println(inputSerial);

        pTxCharacteristic->setValue(inputSerial);
        pTxCharacteristic->notify();
        inputSerial = "";
      }
    } else {
      inputSerial += c;
    }
  }

  // Reconectar advertising após desconexão
  if (!deviceConnected && oldDeviceConnected) {
    delay(500);
    pServer->startAdvertising();
    Serial.println("Reiniciando advertising...");
    oldDeviceConnected = deviceConnected;
  }

  // Novo cliente conectado
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
    Serial.println("Novo cliente conectado.");
  }
}
