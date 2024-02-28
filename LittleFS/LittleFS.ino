/*
 * Exemplo LittleFS
 * Descrição: Esse código exibe como usar o LittleFS para armazenar dados em um arquivo
 * O exemplo exibe como usar um botão para escrever dados em um arquivo e outro botão 
 * para ler os dados do arquivo
 * 
 * Autor: Fábio Souza
 * Data: 14/12/2023
*/

//mapa de pinos da Franzininho WiFi LAB01
#define LED_VERMELHO 14   // Pino do LED Vermelho
#define LED_VERDE 13      // Pino do LED Verde
#define LED_AZUL 12       // Pino do LED Azul
#define BOTAO_1 7         // Pino do Botão 1
#define BOTAO_2 6         // Pino do Botão 2
#define BOTAO_3 5         // Pino do Botão 3
#define BOTAO_4 4         // Pino do Botão 4
#define BOTAO_5 3         // Pino do Botão 5
#define BOTAO_6 2         // Pino do Botão 6
#define BUZZER  17        // Pino Do Buzzer

#include <Arduino.h>
#include "FS.h"
#include <LittleFS.h>

const int buttonPin[] = {BOTAO_6, BOTAO_5};     // Pinos dos Botões

unsigned long debounceDelay = 50;               // Atraso para evitar o efeito de debounce
unsigned long lastDebounceTime[] = {0, 0};      // Último tempo de debounce para cada botão

int buttonState[] = {HIGH, HIGH};       // Estado atual de cada botão
int lastButtonState[] = {HIGH, HIGH};   // Último estado de cada botão

int dado;

/* You only need to format LittleFS the first time you run a
   test or else use the LITTLEFS plugin to create a partition
   https://github.com/lorol/arduino-esp32littlefs-plugin
 */
#define FORMAT_LITTLEFS_IF_FAILED true

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\r\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("- failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println(" - not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.path(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("\tSIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}

void createDir(fs::FS &fs, const char * path){
    Serial.printf("Creating Dir: %s\n", path);
    if(fs.mkdir(path)){
        Serial.println("Dir created");
    } else {
        Serial.println("mkdir failed");
    }
}

void removeDir(fs::FS &fs, const char * path){
    Serial.printf("Removing Dir: %s\n", path);
    if(fs.rmdir(path)){
        Serial.println("Dir removed");
    } else {
        Serial.println("rmdir failed");
    }
}

void readFile(fs::FS &fs, const char * path){
    Serial.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        Serial.println("- failed to open file for reading");
        return;
    }

    Serial.println("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        Serial.println("- failed to open file for writing");
        return;
    }
    if(file.print(message)){
        Serial.println("- file written");
    } else {
        Serial.println("- write failed");
    }
    file.close();
}

void appendFile(fs::FS &fs, const char * path, const char * message){
    Serial.printf("Appending to file: %s\r\n", path);

    File file = fs.open(path, FILE_APPEND);
    if(!file){
        Serial.println("- failed to open file for appending");
        return;
    }
    if(file.print(message)){
        Serial.println("- message appended");
    } else {
        Serial.println("- append failed");
    }
    file.close();
}

void renameFile(fs::FS &fs, const char * path1, const char * path2){
    Serial.printf("Renaming file %s to %s\r\n", path1, path2);
    if (fs.rename(path1, path2)) {
        Serial.println("- file renamed");
    } else {
        Serial.println("- rename failed");
    }
}

void deleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\r\n", path);
    if(fs.remove(path)){
        Serial.println("- file deleted");
    } else {
        Serial.println("- delete failed");
    }
}


void setup() {
  Serial.begin(115200);             // Inicializa a comunicação serial

  delay(5000);                      // Aguarda 5 segundos para a inicialização da comunicação serial

  for (int i = 0; i < sizeof(buttonPin) / sizeof(buttonPin[0]); i++) {
    pinMode(buttonPin[i], INPUT_PULLUP); // Configura os pinos dos botões como entrada
  }
  pinMode(BUZZER,OUTPUT);
  
  if(!LittleFS.begin(FORMAT_LITTLEFS_IF_FAILED))
  {
      Serial.println("Falha na montagem do LittleFS");              
      return;
   }
   else{
       Serial.println("Little FS montado com sucesso!");
   }

   // verifica se o arquivo existe e cria se não existir
   bool fileexists = LittleFS.exists("/data.txt");
   Serial.print(fileexists);

   if(!fileexists)                                              // Se o arquivo não existir
   {
       Serial.println("Arquivo não existe");                    // Exibe mensagem
       Serial.println("Criando o arquivo...");                      // Exibe mensagem
       writeFile(LittleFS, "/data.txt", "Dados Salvos \r\n");  // Cria o arquivo e adiciona um texto
   }
   else {
       Serial.println("Arquivo já existe!");
   }
}

void loop() 
{

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

          switch (i)
          {
            case 0:
                dado = random (0, 5000);            
                appendFile(LittleFS, "/data.txt", (String(dado)+ "\r\n").c_str()); //Adiciona um dado ao arquivo
            break;
            case 1:
                readFile(LittleFS, "/data.txt");  //le o arquivo                      
            break;
          }

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
