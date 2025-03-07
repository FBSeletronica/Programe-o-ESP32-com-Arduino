/*
 * Exemplo de MQTT Publisher
 * Descrição: Esse exemplo mostra como publicar valores de temperatura e umidade,
 *            obtidos de um DHT11 em um broker MQTT
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
const char* ssid = "xxxxxxxxxxxxxxxx";              // SSID da rede Wi-Fi substituir pelo SSID da sua rede
const char* password =  "xxxxxxxxxxxxxxxxxxxxxx";   // Senha da rede Wi-Fi substituir pela senha da sua rede

// Configuração do Broker MQTT
const char* mqtt_server = "mqtt.eclipseprojects.io";  // Endereço do Broker MQTT - Se desejar utilizar outro, substitua este pelo endereço do seu broker
WiFiClient espClient;                                 // Cria o objeto espClient        
PubSubClient client(espClient);                       // Cria o objeto client com o objeto espClient 

// Definição dos tópicos MQTT
const char* topic_temp = "esp32/sensor/temperatura";  // Tópico para publicação da temperatura - Substitua pelo tópico desejado se necessário
const char* topic_umid = "esp32/sensor/umidade";      // Tópico para publicação da umidade - Substitua pelo tópico desejado se necessário

// Configuração do sensor DHT11
#define DHTPIN 15         // Pino de dados do DHT11
#define DHTTYPE DHT11     // Tipo do sensor
DHT dht(DHTPIN, DHTTYPE); // Cria o sensor DHT11

// Protótipo da task
void taskEnviarDHT(void *parameter);  // Função da task que lê e envia os dados do DHT11 via MQTT

// Função de configuração do Wi-Fi
void setupWiFi() 
{
  WiFi.begin(ssid, password);             // Conecta na rede Wi-Fi configurada 
  Serial.print("Conectando ao WiFi");     // Aguarda a conexão com a rede
  while (WiFi.status() != WL_CONNECTED)   // Verifica o status da conexão e aguarda até que a conexão seja estabelecida  
  {
    delay(500);                           // Aguarda 500ms
    Serial.print(".");                    // Imprime "." na tela
  }
  Serial.println("\nWiFi Conectado!");    // Informa que a conexão foi estabelecida
}

// Função de reconexão ao Broker MQTT
void connectMQTT() 
{
  while (!client.connected())               // Enquanto o cliente não estiver conectado ao Broker MQTT
  {
    Serial.println("Conectando ao MQTT...");  // Informa que está tentando conectar ao Broker MQTT
    if (client.connect("ESP32_Client"))     // Tenta se conectar ao Broker MQTT
    {
      Serial.println("Conectado!");         // Informa que a conexão foi estabelecida
    } 
    else                                  // Caso não consiga se conectar ao Broker MQTT
    {
      Serial.print("Falha, código: ");    // Informa que houve uma falha
      Serial.println(client.state());     // Informa o código do erro
      delay(5000);                        // Aguarda 5 segundos e tenta novamente
    }
  }
}

void setup() 
{
  Serial.begin(115200);                   // Inicializa a comunicação serial
  setupWiFi();                            // Configura a conexão Wi-Fi
  client.setServer(mqtt_server, 1883);    // Configura o Broker MQTT e a porta de comunicação
  connectMQTT();                           // Conecta ao Broker MQTT

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

//
void loop() 
{
  if (!client.connected()) connectMQTT();   // Se a conexão com o Broker MQTT foi perdida, reconecta

  client.loop();                          // Mantém a conexão com o Broker MQTT
}

// Task para leitura e publicação dos dados do DHT11
void taskEnviarDHT(void *parameter) 
{
   dht.begin(); // Inicializa o sensor DHT11

  while(1) 
  { 
    float temperatura = dht.readTemperature();  // Celsius
    float umidade = dht.readHumidity();         // Percentual

    if (!isnan(temperatura) && !isnan(umidade))   // Verifica se a leitura do sensor foi bem sucedida
    {
      char tempStr[8], umidStr[8];                // Strings para armazenar os valores de temperatura e umidade
      dtostrf(temperatura, 6, 2, tempStr);        // Converte o valor da temperatura para string
      dtostrf(umidade, 6, 2, umidStr);            // Converte o valor da umidade para string

      // Publica os valores via MQTT 
      client.publish(topic_temp, tempStr, false); // Publica a temperatura no tópico definido
      client.publish(topic_umid, umidStr, false); // Publica a umidade no tópico definido

      // Imprime os valores no monitor serial - log
      Serial.print("Temperatura enviada: ");
      Serial.println(tempStr);
      Serial.print("Umidade enviada: ");
      Serial.println(umidStr);
    } 
    else // Caso a leitura do sensor falhe
    {
      Serial.println("Falha na leitura do sensor DHT11!");  // Informa que houve uma falha na leitura do sensor
    }

    // Aguardar 60 segundos antes da próxima leitura - ajuste conforme necessário
    vTaskDelay(60000 / portTICK_PERIOD_MS);
  }
}
