
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
