/*
 * Exemplo de uso do Touch 
 * Descrição: Esse código demonstra o uso do touch do ESP32
 * 
 * Autor: Fábio Souza
 * Data: 09/10/2023
*/

int threshold = 40;           // Limiar de sensibilidade do toque
bool touch1detected = false;  // Flag de toque 1
bool touch2detected = false;  // Flag de toque 2

// Função de callback para o toque 1
void gotTouch1(){
 touch1detected = true;
}

// Função de callback para o toque 2
void gotTouch2(){
 touch2detected = true;
}

// Função de setup
void setup() {
  Serial.begin(115200);                           // Inicializa a comunicação serial
  delay(1000);                                    // Aguarda 1 segundo
  Serial.println("ESP32 Touch Interrupt Test");   // Imprime na serial
  touchAttachInterrupt(T2, gotTouch1, threshold); // Configura o pino 2 para o toque 1  
  touchAttachInterrupt(T3, gotTouch2, threshold); // Configura o pino 3 para o toque 2
}

// Função de loop
void loop(){

  if(touch1detected){                     // Se o toque 1 foi detectado
    touch1detected = false;               // Limpa a flag
    Serial.println("Touch 1 detected");   // Imprime na serial
  }

  if(touch2detected){                     // Se o toque 2 foi detectado
    touch2detected = false;               // Limpa a flag
    Serial.println("Touch 2 detected");   // Imprime na serial
  }
}
