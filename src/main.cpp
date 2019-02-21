#include <Arduino.h>
#include <Servo.h>


const int tamanho = 50;
int lat = 90;
int longe = 90;

Servo servo;
Servo otoservo;

float media (float *vetor) {
  float acc = 0;
  for (int i = 0; i < tamanho; i++) {
    acc += vetor[i];
  }
  acc = acc / tamanho;
  return acc;
}

float mapear(int porta, int resistencia) {
  float valorLDR = 1023;
  while (valorLDR == 1023){
    valorLDR = analogRead(porta);
    delay(3);
  }
  valorLDR = map(valorLDR, 0, 1023, 0, 5000) / 1000.0;
  valorLDR = ((resistencia * valorLDR) / (5 - valorLDR));

  return valorLDR;
}

#include <light_intensity.h>
#include <axis.h>
//posiciona o motor de acordo com a media dos dois primeiros LDR  comparada aos dois outros.

/*LDR topright(390, A0, 1805.10, );
LDR topleft(390, A1, 1463.45);
LDR botleft(390, A2, 1152.68);
LDR botright(390, A3, 1351.18);*/
LDR topright(390, A0, 0, 1);
LDR topleft(390, A1, 0, 1);
LDR botleft(390, A2, 0, 1);
LDR botright(390, A3, 0, 1);
Eixo eixoY(&topright, &topleft, &botleft, &botright, &otoservo);
Eixo eixoX(&topleft, &botleft, &botright, &topright, &servo);



void setup() {
  Serial.begin(9600);
  servo.attach(9); //must have pwm
  otoservo.attach(10); //must have pwm
}

void loop() {

  topright.atualizar();
  topleft.atualizar();
  botright.atualizar();
  botleft.atualizar();

  Serial.print(topright.getIntensity());
  Serial.print(",");
  Serial.print(topleft.getIntensity());
  Serial.print(",");
  Serial.print(botleft.getIntensity());
  Serial.print(",");
  Serial.print(botright.getIntensity());
  Serial.println("");

  eixoX.atualizar();
  eixoY.atualizar();

  delay(50);

}
