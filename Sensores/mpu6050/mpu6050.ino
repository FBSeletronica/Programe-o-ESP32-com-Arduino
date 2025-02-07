/*
 * Exemplo para leitura do sensor MPU6050
 * 
 * descrição: Este código lê os valores do acelerômetro, giroscópio e temperatura do sensor MPU6050
 * objetivo: Exibir os valores do acelerômetro, giroscópio e temperatura do sensor MPU6050 no monitor serial.
 * 
 * Bibliotecaa: 
 *  Adafruit MPU6050
 *  Adafruit Sensor
 *  Wire 
 * 
 * Este código é parte do curso "Programe o ESP32 com Arduino" da Embarcados: https://cursos.embarcados.com.br/cursos/franzininho-wifi-arduino-ide/
  *  
 * Autor: Fábio Souza
 * data: 06/09/2023
 */
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#define USE_SERIAL_PLOTTER true

// Cria um objeto MPU6050
Adafruit_MPU6050 mpu;


 
void setup()
{
  // Inicializa a comunicação serial
  Serial.begin(115200);
  Serial.println("Inicializando o sensor MPU6050");
 
  if (!mpu.begin()) 
  {
    Serial.println("Não foi possível encontrar o sensor MPU6050");
    while (1);
  }

  // Configura o sensor
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
 
  Serial.println("Sensor MPU6050 inicializado com sucesso");  
}
 
void loop()
{
  sensors_event_t a, g, temp;           // Cria variáveis para armazenar os valores do acelerômetro, giroscópio e temperatura
  mpu.getEvent(&a, &g, &temp);          // Lê os valores do acelerômetro, giroscópio e temperatura
 
#if USE_SERIAL_PLOTTER                // Se estiver utilizando o Serial Plotter, exibe as leituras no Plotter Serial
  Serial.print("ax:");
  Serial.print(a.acceleration.x);
  Serial.print(",");
  Serial.print("ay:");
  Serial.print(a.acceleration.y);
  Serial.print(",");
  Serial.print("az:");
  Serial.print(a.acceleration.z);
  Serial.print(",");
  Serial.print("gx:");
  Serial.print(g.gyro.x);
  Serial.print(",");
  Serial.print("gy:");
  Serial.print(g.gyro.y);
  Serial.print(",");
  Serial.print("gz:");
  Serial.print(g.gyro.z);
  Serial.print(",");
  Serial.println(temp.temperature);
  
#else
  Serial.print("AccelX: "); Serial.print(a.acceleration.x); Serial.print(" m/s²\t");
  Serial.print("AccelY: "); Serial.print(a.acceleration.y); Serial.print(" m/s²\t");
  Serial.print("AccelZ: "); Serial.println(a.acceleration.z); Serial.print(" m/s²");

  Serial.print("GyroX: "); Serial.print(g.gyro.x); Serial.print(" rad/s\t");
  Serial.print("GyroY: "); Serial.print(g.gyro.y); Serial.print(" rad/s\t");
  Serial.print("GyroZ: "); Serial.println(g.gyro.z); Serial.print(" rad/s");

  Serial.print("Temp: "); Serial.print(temp.temperature); Serial.println(" °C");
  Serial.println("----------------------------------");
#endif
 
  delay(100);
}
