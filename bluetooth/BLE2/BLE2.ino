/*
 * Exemplo de uso do Bluetooth Low Energy (BLE) no ESP32
 * Descrição: Este exemplo demonstra como criar um servidor BLE no ESP32 com serviços personalizados e serviços padrão.
 * Ele inclui um serviço personalizado com leitura e escrita, além de serviços padrão como Bateria e Informações do Dispositivo.
 * O exemplo também inclui um LED que é acionado com base no valor recebido via BLE. O LED acende quando o valor recebido é 
 * maior ou igual a 50.
 * O valor recebido é enviado para o cliente BLE e atualizado a cada segundo.
 * O nível de bateria é simulado com a leitura do ADC.
 * O cliente pode se conectar ao servidor BLE usando um aplicativo como o nRF Connect.
 *  * 
 * Este exemplo foi desenvolvido para o curso Domine o ESP32 com Arduino: Da Base aos Projetos Avançados
 * LINK: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 * Autor: Fábio Souza
 * Data: 19/05/2025
*/
// Blibliotecas necessárias
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define LED_PIN 14  // Definição do pino do LED

// Configuração do Serviço Personalizado
#define CUSTOM_SERVICE_UUID        "6e400001-b5a3-f393-e0a9-e50e24dcca9e"
#define CUSTOM_CHARACTERISTIC_UUID "2ccdb908-3513-11f0-9cd2-0242ac120002"

// Novo serviço personalizado para leitura/escrita
#define RW_SERVICE_UUID            "31f788aa-3513-11f0-9cd2-0242ac120002"
#define READ_CHARACTERISTIC_UUID   "35e83bd0-3513-11f0-9cd2-0242ac120002"
#define WRITE_CHARACTERISTIC_UUID  "39583d6a-3513-11f0-9cd2-0242ac120002"

// Configuração do Serviços Padrão
#define BATTERY_SERVICE_UUID         BLEUUID((uint16_t)0x180F)
#define BATTERY_LEVEL_UUID           BLEUUID((uint16_t)0x2A19)

#define DEVICE_INFO_SERVICE_UUID     BLEUUID((uint16_t)0x180A)
#define MANUFACTURER_NAME_UUID       BLEUUID((uint16_t)0x2A29)
#define FIRMWARE_REVISION_UUID       BLEUUID((uint16_t)0x2A26)
#define MODEL_NUMBER_UUID            BLEUUID((uint16_t)0x2A24)

// Características do BLE
BLECharacteristic* customCharacteristic;
BLECharacteristic* batteryLevelCharacteristic;
BLECharacteristic* readCharacteristic;

int valorRecebido = 0;  // Variável para armazenar o valor recebido

// Callback para escrita
// Esta classe é chamada quando o cliente BLE escreve um valor na característica
// Ela atualiza o valor recebido e acende ou apaga o LED com base no valor recebido
class WriteCallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pCharacteristic) override {
    String value = pCharacteristic->getValue();         // Obtém o valor escrito pelo cliente
    if (value.length() > 0) {                           // Verifica se o valor não está vazio
      valorRecebido = value.toInt();                    // Converte o valor recebido para inteiro         
      Serial.print("Valor recebido via BLE: ");         // Imprime o valor recebido
      Serial.println(valorRecebido);                    // Imprime o valor recebido   
      if(valorRecebido >= 50)               
      {
        digitalWrite(LED_PIN,HIGH);        // Acende o LED se o valor recebido for maior ou igual a 50
      }
      else
      {
        digitalWrite(LED_PIN,LOW);       // Apaga o LED se o valor recebido for menor que 50  
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando BLE com leitura e escrita...");
  pinMode(LED_PIN,OUTPUT);

  BLEDevice::init("ESP32_BLE_02");
  BLEServer* pServer = BLEDevice::createServer();

  // Configuração do Serviço Personalizado
  BLEService* customService = pServer->createService(CUSTOM_SERVICE_UUID);
  customCharacteristic = customService->createCharacteristic(
    CUSTOM_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  customCharacteristic->setValue("TESTE BLE");
  customService->start();

  // Configuração do Serviço de Leitura e Escrita
  BLEService* rwService = pServer->createService(RW_SERVICE_UUID);

  readCharacteristic = rwService->createCharacteristic(
    READ_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ
  );
  readCharacteristic->setValue(valorRecebido);

  BLECharacteristic* writeCharacteristic = rwService->createCharacteristic(
    WRITE_CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_WRITE
  );
  writeCharacteristic->setCallbacks(new WriteCallback());

  rwService->start();

  // Configuração do Serviço de Bateria
  BLEService* batteryService = pServer->createService(BATTERY_SERVICE_UUID);
  batteryLevelCharacteristic = batteryService->createCharacteristic(
    BATTERY_LEVEL_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  batteryLevelCharacteristic->addDescriptor(new BLE2902());
  batteryService->start();

  // Configuração do Serviço de Informações do Dispositivo
  BLEService* deviceInfoService = pServer->createService(DEVICE_INFO_SERVICE_UUID);

  deviceInfoService->createCharacteristic(MANUFACTURER_NAME_UUID, BLECharacteristic::PROPERTY_READ)
    ->setValue("Embarcados");

  deviceInfoService->createCharacteristic(FIRMWARE_REVISION_UUID, BLECharacteristic::PROPERTY_READ)
    ->setValue("v1.0");

  deviceInfoService->createCharacteristic(MODEL_NUMBER_UUID, BLECharacteristic::PROPERTY_READ)
    ->setValue("ESP32 DevKit C");

  deviceInfoService->start();

  // Advertising 
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(CUSTOM_SERVICE_UUID);
  pAdvertising->addServiceUUID(RW_SERVICE_UUID);
  pAdvertising->addServiceUUID(BATTERY_SERVICE_UUID);
  pAdvertising->addServiceUUID(DEVICE_INFO_SERVICE_UUID);
  pAdvertising->start();

  Serial.println("BLE pronto. Conecte com nRF Connect.");
}

void loop() {
  // Atualiza leitura do ADC como nível de bateria simulado
  static unsigned long lastUpdate = 0;
  if (millis() - lastUpdate > 1000) {
    uint8_t level = map(analogRead(36), 0, 4096, 0, 100);
    batteryLevelCharacteristic->setValue(&level, 1);
    batteryLevelCharacteristic->notify();

    // Atualiza valor de leitura com o último valor recebido
    readCharacteristic->setValue(String(valorRecebido).c_str());

    Serial.print("Nível de bateria: ");
    Serial.print(level);
    Serial.print("% | Valor recebido: ");
    Serial.println(valorRecebido);

    lastUpdate = millis();
  }
}
