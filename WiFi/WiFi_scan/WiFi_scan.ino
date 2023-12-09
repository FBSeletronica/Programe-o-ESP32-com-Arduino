/*
 * Exemplo de WiFi Scan
 * Descrição: Esse exemplo mostra como fazer um scan das redes WiFi disponíveis
 * 
 * Autor: Fábio Souza
 * Data: 09/10/2023
*/
#include "WiFi.h"   // Biblioteca WiFi

void setup()
{
    Serial.begin(115200);           // Inicializa a comunicação serial

    WiFi.mode(WIFI_STA);            // Configura o WiFi para o modo Station (cliente)
    WiFi.disconnect();              // Desconecta do WiFi, caso esteja conectado
    delay(100);                     // Aguarda 100ms

    Serial.println("Setup Finalizado");   // Imprime na serial que o setup foi concluído
}

void loop()
{
    Serial.println("Iniciando Scanner");               // Imprime na serial que o scan foi iniciado

    int n = WiFi.scanNetworks();                // Inicia o scan das redes WiFi disponíveis
    Serial.println("Scanner Finalizado");                // Imprime na serial que o scan foi concluído

    if (n == 0)                                 // Se nenhuma rede for encontrada
    {
        Serial.println("Não foi encontrada nenhuma rede");    // Imprime na serial que nenhuma rede foi encontrada
    } else 
    {
        //formatação da tabela de redes encontradas
        Serial.print(n);                        
        Serial.println(" redes encontradas");      
        Serial.println("Nr | SSID                             | RSSI | CH | Encryption");

        // Loop para imprimir as redes encontradas
        for (int i = 0; i < n; ++i) 
        {
            // Imprime na serial o número da rede, o nome da rede, o RSSI, o canal e o tipo de criptografia
            Serial.printf("%2d",i + 1);
            Serial.print(" | ");
            Serial.printf("%-32.32s", WiFi.SSID(i).c_str());
            Serial.print(" | ");
            Serial.printf("%4ld", WiFi.RSSI(i));
            Serial.print(" | ");
            Serial.printf("%2ld", WiFi.channel(i));
            Serial.print(" | ");
            switch (WiFi.encryptionType(i))
            {
            case WIFI_AUTH_OPEN:
                Serial.print("WIFI_AUTH_OPEN");
                break;
            case WIFI_AUTH_WEP:
                Serial.print("WIFI_AUTH_WEP");
                break;
            case WIFI_AUTH_WPA_PSK:
                Serial.print("WIFI_AUTH_WPA_PSK");
                break;
            case WIFI_AUTH_WPA2_PSK:
                Serial.print("WIFI_AUTH_WPA2_PSK");
                break;
            case WIFI_AUTH_WPA_WPA2_PSK:
                Serial.print("WIFI_AUTH_WPA_WPA2_PSK");
                break;
            case WIFI_AUTH_WPA2_ENTERPRISE:
                Serial.print("WIFI_AUTH_WPA2_ENTERPRISE");
                break;
            case WIFI_AUTH_WPA3_PSK:
                Serial.print("WIFI_AUTH_WPA3_PSK");
                break;
            case WIFI_AUTH_WPA2_WPA3_PSK:
                Serial.print("WIFI_AUTH_WPA2_WPA3_PSK");
                break;
            case WIFI_AUTH_WAPI_PSK:
                Serial.print("WIFI_AUTH_WAPI_PSK");
                break;
            default:
                Serial.print("UNKNOWN");
            }
            Serial.println();   // Imprime uma quebra de linha na serial antes de imprimir a próxima rede
            delay(10);          // Aguarda 10ms
        }
    }
    Serial.println("");         // Imprime uma quebra de linha    
    WiFi.scanDelete();          // Deleta a lista de redes encontradas
    delay(10000);               //aguarda 10s antes de fazer um novo scan    
}
