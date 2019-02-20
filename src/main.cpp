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

class LDR {
    float resistor;
    int pin;
    float valores[tamanho];
    float correcao_claro;
    float correcao_escuro;
  public:
    LDR(float resistor, int pin, float claro, float escuro) {
      this->resistor = resistor;
      this->pin = pin;
      correcao_claro = claro;
      correcao_escuro = escuro;
    }

    void atualizar(){
      valores[tamanho - 1] = mapear(pin, resistor);
      for (int i = 0; i <= tamanho - 2; i++) {
        valores[i] = valores[i + 1];
      }
    }

    float getIntensity() {
      return ((media(valores) - correcao_claro)/correcao_escuro);
    }
};
//posiciona o motor de acordo com a media dos dois primeiros LDR  comparada aos dois outros.
class Eixo {

    LDR *topright;
    LDR *topleft;
    LDR *botleft;
    LDR *botright;
    Servo* servo;
  public:
    Eixo(LDR *topright, LDR *topleft, LDR *botleft, LDR *botright, Servo* servo) {
      this->topright = topright;
      this->topleft = topleft;
      this->botleft = botleft;
      this->botright = botright;
      this->servo = servo;
    };
    int incremento (int late) {
      if (late >= 180) return late;
      else return (late + 3);
    }
    int decremento (int late) {
      if (late <= 0) return late;
      else return (late - 3);
    }
    void atualizar() {
      float media = ((*topright).getIntensity() + (*topleft).getIntensity()) / 2;
      float otomedia = ((*botleft).getIntensity() + (*botright).getIntensity()) / 2;
      float diferenca = fabs(media - otomedia);

      if (diferenca > 1100) {
        if (media > otomedia)  lat = incremento(lat);
        else lat = decremento(lat);
        (*servo).write(lat);
      }
    }

};

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
