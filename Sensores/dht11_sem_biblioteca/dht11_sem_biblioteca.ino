/*
 * Exemplo para leitura do sensor DHT11 sem usar biblioteca
 * 
 * descrição: Este código lê o sensor DHT11 sem usar biblioteca. Ele envia um pulso de início, aguarda a resposta do sensor, 
 *              confirma a presença do sensor, lê os 40 bits de dados e verifica o checksum.
 * objetivo: Ler a temperatura e umidade do ambiente com o sensor DHT11.
 * 
* Pulso de início: O ESP32 coloca o pino LOW por 18ms e depois libera.
* Resposta do DHT11: O sensor responde com um pulso LOW de 80µs e HIGH de 80µs.
* Leitura dos 40 bits: Para cada bit:
*   Aguarda o início do pulso HIGH.
*   Mede o tempo HIGH:
*       Se for menor que 50µs → Bit 0
*       Se for maior que 50µs → Bit 1
* Checksum: O último byte recebido deve ser igual à soma dos quatro anteriores.
  
 * Este código é parte do curso "Programe o ESP32 com Arduino" da Embarcados: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
  *  
 * Autor: Fábio Souza
 * data: 06/09/2023
 */

#define DHTPIN 15  // Defina o pino onde o DHT11 está conectado

void setup() {
    Serial.begin(115200);                                               // Inicializa a comunicação serial
    Serial.println("Iniciando leitura do DHT11 sem biblioteca...");     // Mensagem de início
}

void loop() {
    int temperatura, umidade;                                           // Variáveis para armazenar os valores lidos
    if (lerDHT11(DHTPIN, &umidade, &temperatura))                       // Chama a função para ler o DHT11 e verifica se a leitura foi bem-sucedida
    {
        // Exibe os valores lidos
        Serial.print("Umidade: ");                                    
        Serial.print(umidade);                                      
        Serial.println("%");

        Serial.print("Temperatura: ");
        Serial.print(temperatura);
        Serial.println("°C");
    } 
    else                                                  // Se a leitura falhar, exibe uma mensagem de erro                            
    {
        Serial.println("Falha na leitura do DHT11.");
    }

    delay(2000);  // Aguarde 2 segundos antes de nova leitura
}

/*
* Função para ler o sensor DHT11
* Parâmetros:
* pin: pino onde o sensor está conectado
* umidade: ponteiro para armazenar a umidade lida
* temperatura: ponteiro para armazenar a temperatura lida
* Retorno: true se a leitura foi bem-sucedida, false caso contrário
*/

bool lerDHT11(int pin, int *umidade, int *temperatura)  // Função para ler o sensor DHT11 
{
    uint8_t dados[5] = {0, 0, 0, 0, 0};                 // Array para armazenar os dados lidos

    // 1. Enviar pulso de início
    pinMode(pin, OUTPUT);       // Define o pino como saída
    digitalWrite(pin, LOW);     // Inicia a comunicação
    delay(18);                  // Manter LOW por 18ms
    digitalWrite(pin, HIGH);    // Libera o barramento
    delayMicroseconds(40);      // Libera o barramento

    // 2. Esperar resposta do DHT11
    pinMode(pin, INPUT_PULLUP);                 // Define o pino como entrada
    unsigned long timeout = micros() + 100;     // Timeout de 100µs
    while (digitalRead(pin) == HIGH)            // Aguarda resposta do sensor (LOW)
    {  
        if (micros() > timeout) return false;  // Se o tempo de espera for excedido, retorna falso
    }

    // 3. Confirmação do sensor (80µs LOW e 80µs HIGH)
    timeout = micros() + 100;                       // Timeout de 100µs
    while (digitalRead(pin) == LOW)                 // Aguarda resposta do sensor (HIGH)
    {
        if (micros() > timeout) return false;    // Se o tempo de espera for excedido, retorna falso
    }
    timeout = micros() + 100;                   // Timeout de 100µs
    while (digitalRead(pin) == HIGH)            // Aguarda resposta do sensor (LOW)
    {
        if (micros() > timeout) return false;   // Se o tempo de espera for excedido, retorna falso
    }

    // 4. Leitura dos 40 bits (5 bytes)
    for (int i = 0; i < 40; i++) 
    {
        // Espera pelo início do bit (LOW)
        timeout = micros() + 100;                   // Timeout de 100µs
        while (digitalRead(pin) == LOW)             // Aguarda resposta do sensor (HIGH)
        {
            if (micros() > timeout) return false;   // Se o tempo de espera for excedido, retorna falso
        }

        // Mede a largura do pulso HIGH
        unsigned long tStart = micros();        // Marca o início do pulso
        timeout = micros() + 100;               // Timeout de 100µs    
        while (digitalRead(pin) == HIGH)        // Aguarda resposta do sensor (LOW)
        {   
            if (micros() > timeout) return false;   // Se o tempo de espera for excedido, retorna falso
        }

        // Se HIGH durar mais de 50µs, é um "1", caso contrário, é "0"
        if ((micros() - tStart) > 50) 
        {
            dados[i / 8] |= (1 << (7 - (i % 8)));   // Armazena o bit
        }
    }

    // 5. Verifica checksum
    // O checksum é a soma dos 4 primeiros bytes
    // Se o checksum for igual ao último byte, a leitura foi bem-sucedida
    // Caso contrário, retorna falso
    if (dados[4] != ((dados[0] + dados[1] + dados[2] + dados[3]) & 0xFF))   // Verifica se o checksum está correto  
    {
        return false;
    }

    // 6. Atualiza os valores
    *umidade = dados[0];      // Parte inteira da umidade
    *temperatura = dados[2];  // Parte inteira da temperatura

    return true;
}
