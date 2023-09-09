/*
 * Exemplo de entrada digital
 * 
 * descrição: Este exemplo faz a leitura de uma entrada digital conectada a um botão
 * e acende o LED quando o botão for pressionado
 * objetivo: Demonstrar o uso de entrada digital
 * 
 * Autor: Fábio Souza
 * data: 06/09/2023
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

void setup() {
  Serial.begin(9600);
  pinMode(BOTAO_1, INPUT_PULLUP);
  pinMode(LED_VERMELHO,OUTPUT);
}

void loop() {
  if(digitalRead(BOTAO_1)== LOW){
    digitalWrite(LED_VERMELHO,HIGH);
    Serial.println("PRESSIONADO");
  }else{
    digitalWrite(LED_VERMELHO,LOW);
    Serial.println("SOLTO");
  }
  delay(200);
}
