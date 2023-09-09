/*
 * Exemplo de entradas digitais com debounce de tecla
 * 
 * descrição: Este exemplo faz a leitura de 3 entrada digital conectada a botões
 * e inverte o estado do LED correspondente
 * objetivo: Demonstrar o uso de entrada digital com rotina de debounce
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
#define BUZZER  17

const int buttonPin[] = {BOTAO_1, BOTAO_2, BOTAO_3};  // Pinos dos Botões 1, 2 e 3
const int ledPin[] = {LED_VERMELHO, LED_VERDE, LED_AZUL};  // Pinos dos LEDs Vermelho, Verde e Azul

unsigned long debounceDelay = 50; // Atraso para evitar o efeito de debounce
unsigned long lastDebounceTime[] = {0, 0, 0}; // Último tempo de debounce para cada botão

int buttonState[] = {HIGH, HIGH, HIGH};   // Estado atual de cada botão
int lastButtonState[] = {HIGH, HIGH, HIGH}; // Último estado de cada botão
int ledState[] = {LOW, LOW, LOW};       // Estado atual de cada LED

// Configuração inicial
void setup() {
  for (int i = 0; i < sizeof(buttonPin) / sizeof(buttonPin[0]); i++) {
    pinMode(buttonPin[i], INPUT_PULLUP); // Configura os pinos dos botões como entrada
    pinMode(ledPin[i], OUTPUT);   // Configura os pinos dos LEDs como saída
  }
  pinMode(BUZZER,OUTPUT);
}

// Loop principal
void loop() {
  for (int i = 0; i < sizeof(buttonPin) / sizeof(buttonPin[0]); i++) {  // Para cada botão
    int reading = digitalRead(buttonPin[i]);                            // Lê o estado do botão

    if (reading != lastButtonState[i]) {                                // Se o estado do botão mudou
      lastDebounceTime[i] = millis();                                   // Atualiza o tempo de debounce
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {             // Se o tempo de debounce passou
      if (reading != buttonState[i]) {                                  // Se o estado do botão mudou
        buttonState[i] = reading;                                       // Atualiza o estado do botão

        if (buttonState[i] == LOW) {                                    // Se o botão foi pressionado
          beep();
          ledState[i] = !ledState[i];                                   // Inverte o estado do LED
          digitalWrite(ledPin[i], ledState[i]);                         // Atualiza o estado do LED
        }
      }
    }
    lastButtonState[i] = reading;                                       // Atualiza o último estado do botão
  }
}

void beep(){
  digitalWrite(BUZZER, HIGH);
  delay(100);
  digitalWrite(BUZZER,LOW);
}
