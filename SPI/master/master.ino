#define SPI_FREQ_US 5  // 5µs
// #define SS 10
// #define MOSI 11
// #define MISO 12
// #define SCK 13


const uint8_t BUTTON_PIN = 2;
const uint8_t DATA_SEND = 0x19;    // Valor para acender led no escravo
const uint8_t DATA_EXPECT = 0x25;  // Valor esperado do retorno do escravo

void spiShowPinout(){
  delay(3000);
  Serial.print("MOSI: ");
  Serial.println(MOSI);
  Serial.print("MISO: ");
  Serial.println(MISO);
  Serial.print("SCK: ");
  Serial.println(SCK);
  Serial.print("SS: ");
  Serial.println(SS);
}

void spiInit()
{
  pinMode(MOSI, OUTPUT);  // MOSI é saída no mestre
  pinMode(MISO, INPUT);   // MISO é entrada no mestre
  pinMode(SCK, OUTPUT);   // SCK é saída no mestre
  pinMode(SS, OUTPUT);    // SS é saída no mestre

  digitalWrite(SCK, LOW);  // Inicializa o clock em LOW
  digitalWrite(SS, HIGH);  // Desativa o escravo SPI
  digitalWrite(MOSI, LOW);
}

// Funcao controle de clock 100kHz
void spiPulseClock(){
  // 1 ciclo = ( 1 / 100 kHz = 10 µs)
  // HIGH = 5 µs e LOW = 5 µs
  digitalWrite(SCK, HIGH);
  delayMicroseconds(SPI_FREQ_US);
  digitalWrite(SCK, LOW);
  delayMicroseconds(SPI_FREQ_US);

}

uint8_t spiSendByte(uint8_t data){
  uint8_t data_in = 0;
  digitalWrite(SS,LOW);
  delayMicroseconds(SPI_FREQ_US);

  // envia bits 7..0
  for(int i = 7;i>=0;i--){
    // Coloca bit a bit na linha MOSI
    digitalWrite(MOSI, (data >> i) & 0x01);
    // Pulso de clock
    spiPulseClock();
    // Lê bit do escravo no MISO
    data_in |= ((digitalRead(MISO)) << i);
  }
  digitalWrite(SS, HIGH);  // Desativa slave e finaliza comunicação
  return data_in; // dado recebido
}

void setup() {
  Serial.begin(115200);
  spiShowPinout();
  spiInit(); 

  pinMode(BUTTON_PIN,INPUT_PULLUP);

}

void loop() {
  if(digitalRead(BUTTON_PIN)==LOW){
    //debounce
    delay(100);
    int startTime = millis();
    while (digitalRead(BUTTON_PIN) == LOW) {
      delay(50);
    }
    int endTime = millis();
    if ((endTime - startTime) > 1000) {
      Serial.println("Botão pressionado. Enviando comando 0x19...");
      uint8_t resposta = spiSendByte(DATA_SEND);
      Serial.print("Resposta do escrava: 0x");
      Serial.println(resposta,HEX);
      if(resposta == DATA_EXPECT){
        Serial.println("Comunicação bem sucedida!");
      }
    }
  }
}
