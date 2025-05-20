/*
 * Exemplo de uso do Bluetooth Low Energy (BLE) no ESP32
 * Descrição: Este exemplo demonstra como criar um servidor BLE com 
 * serviços personalizados e serviços padrão, como Bateria e Informações do Dispositivo.
 * 
 * Este exemplo foi desenvolvido para o curso Domine o ESP32 com Arduino: Da Base aos Projetos Avançados
 * LINK: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 * Autor: Fábio Souza
 * Data: 19/05/2025
*/

// bibliotecas necessárias
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

//Serviço Personalizado site https://www.uuidgenerator.net/
#define CUSTOM_SERVICE_UUID        "6e400001-b5a3-f393-e0a9-e50e24dcca9e" 
#define CUSTOM_CHARACTERISTIC_UUID "6e400002-b5a3-f393-e0a9-e50e24dcca9e"

//Serviços Padrão
#define BATTERY_SERVICE_UUID         BLEUUID((uint16_t)0x180F)
#define BATTERY_LEVEL_UUID           BLEUUID((uint16_t)0x2A19)

#define DEVICE_INFO_SERVICE_UUID     BLEUUID((uint16_t)0x180A)
#define MANUFACTURER_NAME_UUID       BLEUUID((uint16_t)0x2A29)
#define FIRMWARE_REVISION_UUID       BLEUUID((uint16_t)0x2A26)
#define MODEL_NUMBER_UUID            BLEUUID((uint16_t)0x2A24)

// Variáveis globais para armazenar as características
BLECharacteristic* customCharacteristic;        // Característica personalizada
BLECharacteristic* batteryLevelCharacteristic;  // Característica de nível de bateria

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando BLE com serviços padronizados...");

  // Inicializa o dispositivo BLE 
  BLEDevice::init("ESP32_BLE_01");  

  // Criação do servidor BLE
  BLEServer* pServer = BLEDevice::createServer(); 

  //Configuração do Serviço Personalizado
  BLEService* customService = pServer->createService(CUSTOM_SERVICE_UUID);
  customCharacteristic = customService->createCharacteristic(
    CUSTOM_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  customCharacteristic->setValue("TESTE BLE");
  customService->start();

  //Configuração do Serviço de Bateria
  BLEService* batteryService = pServer->createService(BATTERY_SERVICE_UUID);
  batteryLevelCharacteristic = batteryService->createCharacteristic(
    BATTERY_LEVEL_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  uint8_t batteryLevel = 100; // valor fixo para exemplo
  batteryLevelCharacteristic->setValue(&batteryLevel, 1);
  batteryLevelCharacteristic->addDescriptor(new BLE2902());
  batteryService->start();

  // Configuração do Serviço de Informações do Dispositivo 
  BLEService* deviceInfoService = pServer->createService(DEVICE_INFO_SERVICE_UUID);

  BLECharacteristic* manufacturerCharacteristic = deviceInfoService->createCharacteristic(
    MANUFACTURER_NAME_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  manufacturerCharacteristic->setValue("Embarcados");

  BLECharacteristic* firmwareCharacteristic = deviceInfoService->createCharacteristic(
    FIRMWARE_REVISION_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  firmwareCharacteristic->setValue("v1.0");

  BLECharacteristic* modelCharacteristic = deviceInfoService->createCharacteristic(
    MODEL_NUMBER_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  modelCharacteristic->setValue("ESP32 DevKit C");

  deviceInfoService->start();

  // Adiciona o serviço personalizado e os serviços padrão à propagação e inicia a propagação
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();   
  pAdvertising->addServiceUUID(CUSTOM_SERVICE_UUID);
  pAdvertising->addServiceUUID(BATTERY_SERVICE_UUID);
  pAdvertising->addServiceUUID(DEVICE_INFO_SERVICE_UUID);
  pAdvertising->start();

  // Exibe informações no console
  Serial.println("BLE pronto. Conecte com nRF Connect e explore os serviços.");

}

void loop() {
  // Exemplo: atualizar valor de bateria a cada 1s
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {                 // Atualiza a cada 1 segundo
    uint8_t level = map(analogRead(36),0,4096,0,100); // simulação de leitura de bateria
    batteryLevelCharacteristic->setValue(&level, 1);  // Atualiza o valor da característica
    batteryLevelCharacteristic->notify();             // Notifica os dispositivos conectados    

    // Exibe no console
    Serial.print("Nível de bateria atualizado: ");    
    Serial.print(level);
    Serial.println("%");
    lastUpdate = millis();
  }
}
