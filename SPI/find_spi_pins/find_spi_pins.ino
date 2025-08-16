/*
 * Exemplo para descobrir os pinos SPI
 * 
 * descrição: Exibe no monitor serial os pinos utilizados para a comunicação SPI padrão no Arduino.
 * objetivo: Auxiliar na identificação dos pinos SPI em diferentes placas Arduino.
 *  
 * Autor: Fábio Souza
 * data: 15/08/2025
 */


void setup() {
  delay(10000);               // Aguarda 10 segundos para permitir a abertura do monitor serial
  Serial.begin(115200);       // Inicia a comunicação serial na taxa de 115200 bps
  Serial.print("MOSI: ");     // Exibe os pinos SPI no monitor serial
  Serial.println(MOSI);       // MOSI: Master Out Slave In
  Serial.print("MISO: ");     // MISO: Master In Slave Out
  Serial.println(MISO);       // Exibe o pino MISO
  Serial.print("SCK: ");      // SCK: Serial Clock
  Serial.println(SCK);        // Exibe o pino SCK
  Serial.print("SS: ");       // SS: Slave Select
  Serial.println(SS);         // Exibe o pino SS

}

void loop() {
  // put your main code here, to run repeatedly:

}
