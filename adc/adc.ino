/*
 * Exemplo de leitura do ADC do ESP32
 * Descricao: Leitura do ADC do ESP32 e impressao do valor lido no monitor serial
 * 
 * Autor: Fábio Souza
 * Data: 21/09/2023
*/

void setup() {
  
  Serial.begin(115200);   // inicia a serial para impressao dos valores lidos
  
  //configura o ADC
  analogReadResolution(12);                           // resolucao de 12 bits
  analogSetAttenuation(adc_attenuation_t::ADC_11db);  //configura a atenuacao para 11db
}

void loop() {

  int analogValue = analogRead(32);  // le o valor do ADC da porta 1
  int analogVolts = analogReadMilliVolts(32);
  
  Serial.printf("ADC analog value = %d\n",analogValue);           // imprime o valor lido no monitor serial
  Serial.printf("ADC millivolts value = %d\n",analogVolts);
  delay(100);                                                     // aguarda 100ms para realizar nova leitura
}