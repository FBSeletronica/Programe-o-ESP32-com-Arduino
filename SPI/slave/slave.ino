/*
 * Exemplo SPI Slave - Bit-Banging
 * 
 * descrição: Este exemplo demonstra como implementar um escravo SPI utilizando bit-banging, ou seja, 
 *            sem o uso da biblioteca SPI.h.
 *            O escravo aguarda a seleção do mestre e, em seguida, recebe dados bit a bit,
 *            respondendo com um valor predefinido.
 *            O LED no pino 12 acende quando a comunicação é bem sucedida, ou seja, quando o mestre envia
 *           um valor específico (0x19) e o escravo responde com um valor predefinido (0x25).
 * objetivo: Demonstrar o funcionamento do protocolo SPI sem o uso da biblioteca SPI.h.
 *  
 * Autor: Fábio Souza
 * data: 15/08/2025
 */


#define LED_PIN 12 // Pino do LED

// #define SS 10
// #define MOSI 11
// #define MISO 12
// #define SCK 13

//Comando predefinidos
const uint8_t DATA_SEND = 0x25;   // Valor para enviar ao mestre
const uint8_t DATA_EXPECT = 0x19; // Valor esperado do mestre para acender o led

void setup() {
  Serial.begin(115200);
  pinMode(MOSI, INPUT);   / / Entrada para receber a saida do mestre
  pinMode(SCK, INPUT);      // Clock do mestre
  pinMode(SS, INPUT);       // Slave Select saida do mestre
  pinMode(MISO, OUTPUT);    // Saida  do escravo para entrada no mestre
  digitalWrite(MISO, LOW);  // Inicializa MISO como LOW

  pinMode(LED_PIN, OUTPUT);

}

void loop() {
  if(digitalRead(SS)==LOW){       // Verifica se o escravo foi selecionado pelo mestre
    uint8_t recebido = 0;         // Variável para armazenar o dado recebido

    //recebe 8 bits do mestre - bit-banging - MSB First
    for(int i = 7;i>=0;i--){      
      // Espera subir o clock - MODE 0 - CPOL=0, CPHA=0
      while (digitalRead(SCK) == LOW);
      // Le bit enviado pelo mestre
      recebido |= ((digitalRead(MOSI)) << i);
      // Define bit para resposta
      digitalWrite(MISO, (DATA_SEND >> i) & 0x01);
      // Espera descer o clock
      while (digitalRead(SCK) == HIGH);
    }

    Serial.print("Recebido do mestre: 0x");
    Serial.println(recebido, HEX);  // Exibe o valor recebido do mestre

    // Verifica se o valor recebido é o esperado
    if(recebido == DATA_EXPECT){  
      Serial.println("Comunicação bem sucedida!");
      digitalWrite(LED_PIN,HIGH);
      delay(1000);
      digitalWrite(LED_PIN,LOW);
    }
  }
}
