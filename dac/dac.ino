/*
 * Exemplo de geração de senoide com DAC
 * Descrição: Esse exemplo exibe como usar o DAC para gerar uma onda senoidal
 * 
 * Autor: Fábio Souza
 * Data: 21/09/2023
*/

int Sin_Array[256];                 // vetor para os valores da senoide
float Period = (2*PI)/256;          // período da onda
float Rad_Angle;                    // variável para o ângulo em radianos

void setup(){

 //gera o vetor com os valores da senoide
 for(int Angle=0; Angle<256; Angle++) {
   Rad_Angle = Angle*Period;                     // calcula o ângulo em radianos
   Sin_Array[Angle] = (sin(Rad_Angle)*127)+128;  // calcula o valor da senoide e armazena no vetor faz um offset para que os valores fiquem entre 0 e 255
 }

}

void loop(){
 for(int i=0;i<256;i++)             // Loop para percorrer o vetor
   dacWrite(17,Sin_Array[i]);       // Escreve o valor da senoide no DAC
}
