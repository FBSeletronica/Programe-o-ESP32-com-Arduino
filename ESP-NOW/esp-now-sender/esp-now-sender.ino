/*
 * Exemplo ESP-NOW Sender
 * Descrição: Esse código envia dados via ESP-NOW para um ESP32 que 
 * está configurado como Receiver. A mensagem é enviada após pressionar
 * um botão e deve controlar um LED no receptor
 * 
 * Autor: Fábio Souza
 * Data: 14/12/2023
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


#include <esp_now.h>
#include <WiFi.h>

const int buttonPin = BOTAO_6;      // Pino do Botão

unsigned long debounceDelay = 50;   // Atraso para evitar o efeito de bounce
unsigned long lastDebounceTime = 0; // Último tempo de debounce

int buttonState = HIGH;             // Estado atual do botão
int lastButtonState = HIGH;         // Último estado do botão

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x34,0xB4,0x72,0x6B,0xD4,0xB2};

//variável para status do LED
uint8_t ledStatus = false;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  
  Serial.begin(115200);             //Inicia a comunicação serial
  pinMode(buttonPin, INPUT_PULLUP);        //Configura o pino do botão como entrada
 
  WiFi.mode(WIFI_STA);              //Configura o ESP32 como Station

  // Inicia o ESP-NOW
  if (esp_now_init() != ESP_OK)                   //Verifica se o ESP-NOW foi iniciado com sucesso
  {
    Serial.println("Erro ao iniciar o ESP-NOW");  //Exibe uma mensagem de erro
    return;                                       //Retorna ao loop
  }

  esp_now_register_send_cb(OnDataSent);           //Registra a função de callback
                                                  //para o envio de dados
  //Registra o receptor
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);  //Copia o endereço MAC do receptor
  peerInfo.channel = 0;                             //Configura o canal do ESP-NOW
  peerInfo.encrypt = false;                         //Configura a criptografia
  
  // Adiciona o receptor     
  if (esp_now_add_peer(&peerInfo) != ESP_OK)          //Verifica se o receptor foi adicionado
  {
    Serial.println("Falha ao adicionar o Receptor");  //Exibe uma mensagem de erro
    return;                                           //Retorna
  }
}
 
void loop() 
{
  int reading = digitalRead(buttonPin);                           // Lê o estado do botão

  if (reading != lastButtonState) {                               // Verifica se o estado do botão mudou
    lastDebounceTime = millis();                                  // Atualiza o tempo de debounce
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {            // Verifica se o tempo de debounce expirou
    if (reading != buttonState) {                                 // Verifica se o estado atual da leitura é diferente do estado do botão
      buttonState = reading;                                      // Atualiza o estado do botão

      if (buttonState == LOW) {                                   // Verifica se o botão foi pressionado
      
        ledStatus^=1;                                             // Inverte o estado do LED

        String msg = String(ledStatus);                           // Cria uma string com o estado do LED
        
        esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)msg.c_str(), msg.length()); // Envia a mensagem via ESP-NOW
        Serial.print("Enviando dados: ");                         // Exibe uma mensagem
        Serial.println(ledStatus); // Exibe o estado do LED
        Serial.print("Resultado do envio: ");                     // Exibe uma mensagem
        Serial.println(result == ESP_OK ? "Sucesso" : "Falha");   // Exibe o resultado do envio
      }
    }
  }
  lastButtonState = reading;                                      // Atualiza o último estado do



  
  

}
