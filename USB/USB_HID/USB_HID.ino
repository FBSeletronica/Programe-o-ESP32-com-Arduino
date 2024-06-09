/*
 * Exemplo de USB HID
 * 
 * descrição: Este exemplo mostra como usar a biblioteca USB HID 
 * objetivo: Demonstrar o funcionamento do USB HID como teclado
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

#include "USB.h"            // Inclui a biblioteca USB
#include "USBHIDKeyboard.h" // Inclui a biblioteca USB HID para teclado
USBHIDKeyboard Keyboard;    // Cria um objeto de teclado HID

// Define os pinos dos botões
const int button6Pin = BOTAO_6;
int previousButton6State = HIGH; // Estado anterior do botão 6

const int button5Pin = BOTAO_5;
int previousButton5State = HIGH; // Estado anterior do botão 5

const int button4Pin = BOTAO_4;
int previousButton4State = HIGH; // Estado anterior do botão 4

const int button3Pin = BOTAO_3;
int previousButton3State = HIGH; // Estado anterior do botão 3

void setup() {
  // Configura os pinos dos botões como entradas com pull-up interno
  pinMode(button6Pin, INPUT_PULLUP);
  pinMode(button5Pin, INPUT_PULLUP);
  pinMode(button4Pin, INPUT_PULLUP);
  pinMode(button3Pin, INPUT_PULLUP);

  Keyboard.begin(); // Inicializa o teclado HID
  USB.begin();      // Inicializa a comunicação USB
  delay(2000);      // Aguarda 2 segundos para estabilização
}

void loop() {
  // Lê o estado do botão 6
  int button6State = digitalRead(button6Pin);
  // Verifica se o estado do botão 6 mudou e se está pressionado
  if ((button6State != previousButton6State) && (button6State == LOW)) {
    // Envia a combinação de teclas Windows + R
    Keyboard.press(KEY_LEFT_GUI);
    Keyboard.press('r');
    delay(1000); // Aguarda 1 segundo
    Keyboard.releaseAll(); // Solta todas as teclas
    Keyboard.println("notepad"); // Digita "notepad" e pressiona Enter
    delay(1000); // Aguarda 1 segundo
  }
  previousButton6State = button6State; // Atualiza o estado anterior do botão 6

  // Lê o estado do botão 5
  int button5State = digitalRead(button5Pin);
  // Verifica se o estado do botão 5 mudou e se está pressionado
  if ((button5State != previousButton5State) && (button5State == LOW)) {
    // Digita "Teste..." e pressiona Enter
    Keyboard.println("Teste...");
    delay(3000); // Aguarda 1 segundo
  }
  previousButton5State = button5State; // Atualiza o estado anterior do botão 5

  // Lê o estado do botão 4
  int button4State = digitalRead(button4Pin);
  // Verifica se o estado do botão 4 mudou e se está pressionado
  if ((button4State != previousButton4State) && (button4State == LOW)) {
    // Envia a combinação de teclas Ctrl + C (copiar)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('c');
    Keyboard.releaseAll(); // Solta todas as teclas
    delay(1000); // Aguarda 1 segundo
  }
  previousButton4State = button4State; // Atualiza o estado anterior do botão 4

  // Lê o estado do botão 3
  int button3State = digitalRead(button3Pin);
  // Verifica se o estado do botão 3 mudou e se está pressionado
  if ((button3State != previousButton3State) && (button3State == LOW)) {
    // Envia a combinação de teclas Ctrl + V (colar)
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press('v');
    Keyboard.press(KEY_RETURN); //ENTER
    Keyboard.releaseAll(); // Solta todas as teclas
    delay(1000); // Aguarda 1 segundo

  }
  previousButton3State = button3State; // Atualiza o estado anterior do botão 3
}
