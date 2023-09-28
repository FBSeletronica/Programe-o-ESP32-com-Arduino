/*
 * Exemplo de geração de senoide com DAC
 * Descrição: Esse exemplo exibe como usar o DAC para gerar uma onda senoidal
 * 
 * Autor: Fábio Souza
 * Data: 21/09/2023
*/

int sine_wave[256];                 // vetor para os valores da senoide
float period = (2*PI)/256;          // período da onda
float angle_rad;                    // variável para o ângulo em radianos

void setup(){

 //gera o vetor com os valores da senoide
 for(int i=0; i<255; i++) {
   angle_rad = i*period;                     // calcula o ângulo em radianos
   sine_wave[i] = (sin(angle_rad)*127)+128;  // calcula o valor da senoide e armazena no vetor faz um offset para que os valores fiquem entre 0 e 255
 }

}

void loop(){
 for(int i=0;i<255;i++)             // Loop para percorrer o vetor
   dacWrite(17,sine_wave[i]);       // Escreve o valor da senoide no DAC
}
