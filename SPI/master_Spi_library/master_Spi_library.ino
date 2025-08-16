/*
 * Exemplo SPI Master - biblioteca SPI.h
 * 
 * descrição: Exemplo de comunicação SPI entre um dispositivo mestre e um escravo.
 * O mestre envia um comando ao escravo e espera uma resposta.
 * O exemplo utiliza a biblioteca SPI.h para facilitar a implementação.
 * objetivo: Demonstrar oi uso da biblioteca SPI.h para comunicação SPI entre um mestre e um escravo.
 *  
 * Autor: Fábio Souza
 * data: 15/08/2025
 */

#include <SPI.h> // Inclui a biblioteca SPI.h

const uint8_t BUTTON_PIN = 2;      
const uint8_t DATA_SEND = 0x19;    // Valor para acender led no escravo
const uint8_t DATA_EXPECT = 0x25;  // Valor esperado do retorno do escravo

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN,INPUT_PULLUP);

 //SPI.begin(SCK, MISO, MOSI, SS);    // Inicializa o barramento SPI com pinos específicos (opcional)
  SPI.begin();                        // Inicializa o barramento SPI com os pinos padrão
  pinMode(SS, OUTPUT);                // Configura o pino de seleção do escravo como saída
  digitalWrite(SS, HIGH);             // Define o pino de seleção do escravo como alto (não selecionado)

}

void loop() {
  if(digitalRead(BUTTON_PIN)==LOW){ // Verifica se o botão está pressionado
    //debounce
    delay(100);
    int startTime = millis();
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(50);
    }
    int endTime = millis();
    if ((endTime - startTime) > 1000) {
      Serial.println("Botão pressionado. Enviando comando 0x19...");

      // Inicia a transação SPI
      // Inicia a transação SPI com configurações específicas
      // SPISettings(velocidade, ordem dos bits, modo SPI)
      SPI.beginTransaction(SPISettings(100000,MSBFIRST,SPI_MODE0));

      digitalWrite(SS,LOW); // Seleciona o escravo (ativa o pino SS)
      uint8_t resposta = SPI.transfer(DATA_SEND); // Envia o comando ao escravo e recebe a resposta
      digitalWrite(SS,HIGH);  // Desseleciona o escravo (desativa o pino SS)

      // Finaliza a transação SPI
      SPI.endTransaction();

      // Exibe a resposta recebida do escravo
      Serial.print("Resposta do escravo: 0x");
      Serial.println(resposta, HEX);

      // Verifica se a resposta é a esperada
      if(resposta == DATA_EXPECT){
        Serial.println("Comunicação bem sucedida!");
      }
      else {
        Serial.println("Resposta inesperada. Verifique a comunicação ou o dispositivo escravo.");
      }
    }
  }
}

