/*
 * Exemplo de entrada digital com interrupçãO externa
 * 
 * descrição: Este exemplo faz a leitura de 1 entrada digital conectada a um botão
 * e inverte o estado do LED correspondente. Também é feita a contagem de pulsos através de uma interrupção externa.
 * No loop principal é feito o blink de um LED para demonstrar que a interrupção não trava o programa.
 * objetivo: Demonstrar o uso de interrupção externa
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
#define BUZZER  17        // Pino do Buzzer

// Variáveis globais
const int buttonPin = BOTAO_1;        // Pino do botão
const int pulseInputPin = BOTAO_2;    // Pino de entrada de pulsos
const int ledPin = LED_VERMELHO;      // Pino do LED
unsigned long lastDebounceTime = 0;   // Último tempo de debounce do botão
int ledState = 0;                     // Estado do LED
int pulseCount = 0;                   // Contador de pulsos

/*
* Função: buttonInterrupt
* Descrição: Função de interrupção externa do botão
* Parâmetros: nenhum
* Retorno: nenhum
*/
void buttonInterrupt(){
  if(millis()-lastDebounceTime >=100){      // Debounce do botão
    ledState = !ledState;                   // Inverte o estado do LED
    digitalWrite(ledPin,ledState);          // Escreve o estado do LED
  }
  lastDebounceTime = millis();              // Atualiza o tempo do debounce
}

/*
* Função: pulseInterrupt
* Descrição: Função de interrupção externa do contador de pulsos
* Parâmetros: nenhum
* Retorno: nenhum
*/
void pulseInterrupt(){
  pulseCount++;                 // Incrementa o contador de pulsos
  Serial.println(pulseCount);   // Imprime o valor do contador de pulsos
}

void setup() {
  Serial.begin(9600);                       // Inicializa a comunicação serial
  pinMode(buttonPin, INPUT_PULLUP);         // Configura o pino do botão como entrada com pullup
  pinMode(pulseInputPin, INPUT_PULLUP);     // Configura o pino de entrada de pulsos como entrada com pullup
  pinMode(ledPin, OUTPUT);                  // Configura o pino do LED como saída
  pinMode(21, OUTPUT);                      // Configura o pino 21 como saída
  attachInterrupt(buttonPin, buttonInterrupt, FALLING);     // Configura a interrupção externa do botão
  attachInterrupt(pulseInputPin, pulseInterrupt, FALLING);  // Configura a interrupção externa do contador de pulsos

}

void loop() {
  digitalWrite(21, HIGH);     // Pisca o LED para demonstrar que a interrupção não trava o programa
  delay(500);                 
  digitalWrite(21, LOW);
  delay(500);
}
