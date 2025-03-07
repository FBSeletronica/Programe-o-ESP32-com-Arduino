/*
 * Exemplo de MQTT Publisher e Subscriber com ESP32
 * Descrição: Esse exemplo mostra como publicar valores de temperatura e umidade,
 *            obtidos de um DHT11 em um broker MQTT e como controlar LEDs via MQTT em tópicos que foram assinados.
 *
 *  Este código é parte do curso "Programe o ESP32 com Arduino" do Embarcados: 
 *  https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
 * 
 * Autor: Fábio Souza
 * Data: 01/03/2025
*/

#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

// Configuração Wi-Fi
const char* ssid = "xxxxxxxxxxxxxxxxxxx";                          // SSID da rede Wi-Fi substituir pelo SSID da sua rede
const char* password = "xxxxxxxxxxxxxxxxxx";                       // Senha da rede Wi-Fi substituir pela senha da sua rede

// Configuração do Broker MQTT
const char* mqtt_server = "mqtt.eclipseprojects.io";              // Endereço do Broker MQTT - Se desejar utilizar outro, substitua este pelo endereço do seu broker
WiFiClient espClient;                                             // Cria um cliente Wi-Fi
PubSubClient client(espClient);                                   // Cria um cliente MQTT

// Definição dos Tópicos MQTT
const char* topic_temp = "esp32/sensor/temperatura";              // Tópico para publicação da temperatura - Substitua pelo tópico desejado se necessário
const char* topic_umid = "esp32/sensor/umidade";                  // Tópico para publicação da umidade - Substitua pelo tópico desejado se necessário
const char* topic_led_azul = "esp32/led/azul";                    // Tópico para controle do LED Azul - Substitua pelo tópico desejado se necessário
const char* topic_led_amarelo = "esp32/led/amarelo";              // Tópico para controle do LED Amarelo - Substitua pelo tópico desejado se necessário

// Configuração do sensor DHT11
#define DHTPIN 15                                                 // Pino de dados do sensor DHT11
#define DHTTYPE DHT11                                             // Tipo do sensor DHT
DHT dht(DHTPIN, DHTTYPE);                                         // Cria o objeto do sensor DHT

// Definição dos pinos dos LEDs
#define LED_AZUL 21
#define LED_AMARELO 33

// Protótipos das funções
void taskEnviarDHT(void *parameter);                              // Função da task que lê e envia os dados do DHT11 via MQTT
void callback(char* topic, byte* message, unsigned int length);   // Função de callback para processar mensagens recebidas do MQTT


// Função de configuração do Wi-Fi
void setupWiFi() 
{
  WiFi.begin(ssid, password);                       // Conecta-se à rede Wi-Fi  
  Serial.print("Conectando ao WiFi...");            // Aguarda a conexão com a rede
  while (WiFi.status() != WL_CONNECTED)             // Enquanto não estiver conectado, aguarda   
  {
    delay(500);                                     // Aguarda 500ms
    Serial.print(".");                              // Imprime um ponto no monitor serial
  }
  Serial.println("\nWiFi Conectado!");              // Após conectar, imprime a mensagem no monitor serial
  Serial.print("Endereço de IP: ");                 // Imprime a mensagem no monitor serial
  Serial.println(WiFi.localIP());                   // Imprime o endereço IP no monitor serial
}

// Função de reconexão ao Broker MQTT
void connectMQTT() 
{
  while (!client.connected())                       // Enquanto o cliente não estiver conectado ao Broker MQTT
  {
    Serial.println("Conectando ao MQTT...");        // Imprime a mensagem no monitor serial
    if (client.connect("ESP32_Client"))             // Tenta conectar ao Broker MQTT
    {
      Serial.println("Conectado!");                 // Imprime a mensagem no monitor serial

      // Assinar os tópicos dos LEDs
      client.subscribe(topic_led_azul);
      client.subscribe(topic_led_amarelo);
      
      // Imprime a mensagem no monitor serial
      Serial.println("Assinado nos tópicos: ");
      Serial.println(topic_led_azul);
      Serial.println(topic_led_amarelo);
    } 
    else 
    {
      Serial.print("Falha, código: ");            // Imprime a mensagem no monitor serial
      Serial.println(client.state());             // Imprime o código de erro no monitor serial
      delay(5000);                                // Aguarda 5 segundos e tenta novamente
    }
  }
}

// Callback para processar mensagens recebidas do MQTT
void callback(char* topic, byte* message, unsigned int length) 

  String msg = "";                    // Cria uma string para armazenar a mensagem
  for (int i = 0; i < length; i++)    // Percorre a mensagem recebida
  {
    msg += (char)message[i];          // Concatena os caracteres da mensagem
  }

  // Imprime a mensagem recebida no monitor serial - Log
  Serial.print("Mensagem recebida no tópico: ");
  Serial.println(topic);
  Serial.print("Conteúdo: ");
  Serial.println(msg);

  // Verifica se a mensagem é para o LED Azul
  if (strcmp(topic, topic_led_azul) == 0)   // Compara o tópico recebido com o tópico do LED Azul 
  {
    if (msg == "ON")                        // Se a mensagem for "ON"
    {
      digitalWrite(LED_AZUL, HIGH);         // Liga o LED Azul
      Serial.println("LED AZUL LIGADO!");   // Imprime a mensagem no monitor serial
    } else if (msg == "OFF")                // Se a mensagem for "OFF" 
    {
      digitalWrite(LED_AZUL, LOW);            // Desliga o LED Azul
      Serial.println("LED AZUL DESLIGADO!");  // Imprime a mensagem no monitor serial
    }
  }

  // Verifica se a mensagem é para o LED Amarelo
  if (strcmp(topic, topic_led_amarelo) == 0)           // Compara o tópico recebido com o tópico do LED Amarelo
  {
    if (msg == "ON")                                  // Se a mensagem for "ON"
    {
      digitalWrite(LED_AMARELO, HIGH);                // Liga o LED Amarelo       
      Serial.println("LED AMARELO LIGADO!");          // Imprime a mensagem no monitor serial
    } else if (msg == "OFF")                          // Se a mensagem for "OFF"           
    {
      digitalWrite(LED_AMARELO, LOW);                 // Desliga o LED Amarelo
      Serial.println("LED AMARELO DESLIGADO!");       // Imprime a mensagem no monitor serial
    }
  }
}

void setup() 
{
  Serial.begin(115200);       // Inicializa a comunicação serial
  setupWiFi();                // Configura a conexão Wi-Fi

  // Configuração dos LEDs como saída e desliga os LEDs
  pinMode(LED_AZUL, OUTPUT);      
  pinMode(LED_AMARELO, OUTPUT);
  digitalWrite(LED_AZUL, LOW);
  digitalWrite(LED_AMARELO, LOW);

  client.setServer(mqtt_server, 1883);    // Define o servidor MQTT 
  client.setCallback(callback);           // Define a função de callback
  connectMQTT();                          // Conecta ao Broker MQTT           

  // Criando a Task para leitura e envio do DHT11
  xTaskCreate(
    taskEnviarDHT,   // Nome da função da task
    "Task DHT11",    // Nome descritivo
    4096,            // Tamanho da stack
    NULL,            // Parâmetro (não usado)
    1,               // Prioridade da task
    NULL             // Handle (não necessário aqui)
  );
}

void loop() 
{
  if (!client.connected()) connectMQTT(); // Se a conexão com o Broker MQTT foi perdida, reconecta
  client.loop();                          // Mantém a conexão com o Broker MQTT
}

void taskEnviarDHT(void *parameter) 
{
  dht.begin();  // Inicializa o sensor DHT11

  while (1) 
  {
    float temperatura = dht.readTemperature();  // Lê a temperatura do sensor DHT11 em Celsius
    float umidade = dht.readHumidity();         // Lê a umidade do sensor DHT11 em %

    if (!isnan(temperatura) && !isnan(umidade))   // Verifica se a leitura do sensor foi bem sucedida
    {
      char tempStr[8], umidStr[8];                // Cria vetores de caracteres para armazenar os valores de temperatura e umidade
      dtostrf(temperatura, 6, 2, tempStr);        // Converte o valor de temperatura para string
      dtostrf(umidade, 6, 2, umidStr);            // Converte o valor de umidade para string

      client.publish(topic_temp, tempStr, false); // Publica o valor da temperatura no tópico correspondente
      client.publish(topic_umid, umidStr, false); // Publica o valor da umidade no tópico correspondente

      // Imprime os valores no monitor serial - log
      Serial.print("Temperatura enviada: ");
      Serial.println(tempStr);
      Serial.print("Umidade enviada: ");
      Serial.println(umidStr);
    } 
    else // Caso a leitura do sensor falhe
    {
      Serial.println("Falha de leitura do sensor DHT11!");  // Imprime a mensagem no monitor serial
    }

    vTaskDelay(60000 / portTICK_PERIOD_MS); // Aguarda 60 segundos
  }
}
