/*
 * Exemplo de uso de PWM para controlar um LED RGB
   Descricao: Este codigo demonstra como usar PWM para controlar um LED RGB
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

// Define os números dos pinos GPIO para os LEDs RGB
const int redPin = LED_VERMELHO;   // pino do LED vermelho - Trocar pelo pino correto se necessário
const int greenPin = LED_VERDE;    // pin do LED verde - Trocar pelo pino correto se necessário
const int bluePin = LED_AZUL;      // pino do LED azul - Trocar pelo pino correto se necessário

void setup() {
  pinMode(redPin, OUTPUT);   // Configura o pino do LED vermelho como saída
  pinMode(greenPin, OUTPUT); // Configura o pino do LED verde como saída
  pinMode(bluePin, OUTPUT);  // Configura o pino do LED azul como saída
}
void loop() {
  // Define valores PWM para criar diferentes cores
  setColor(255, 0, 0);   // Vermelho
  delay(1000);           // Aguarda 1 segundo

  setColor(0, 255, 0);   // Verde
  delay(1000);           // Aguarda 1 segundo

  setColor(0, 0, 255);   // Azul
  delay(1000);           // Aguarda 1 segundo

  setColor(255, 255, 0); // Amarelo
  delay(1000);           // Aguarda 1 segundo

  setColor(0, 255, 255); // Ciano
  delay(1000);           // Aguarda 1 segundo

  setColor(255, 0, 255); // Magenta
  delay(1000);           // Aguarda 1 segundo
}

/*
  Função para definir os valores PWM para os LEDs RGB
  Recebe como parâmetros os valores PWM para o LED vermelho, verde e azul
  O valor deve estar entre 0 e 255
  retorna nada
*/
void setColor(int redValue, int greenValue, int blueValue) {
  analogWrite(redPin, redValue);     // Define o valor PWM para o LED vermelho
  analogWrite(greenPin, greenValue); // Define o valor PWM para o LED verde
  analogWrite(bluePin, blueValue);   // Define o valor PWM para o LED azul
}