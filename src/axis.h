
class Axis {

    LightIntensity *topright;
    LightIntensity *topleft;
    LightIntensity *botleft;
    LightIntensity *botright;
    Servo* servo;

    int angle = 90;

  public:
    Axis(LightIntensity *topright, LightIntensity *topleft, LightIntensity *botleft, LightIntensity *botright, Servo* servo) {
      this->topright = topright;
      this->topleft = topleft;
      this->botleft = botleft;
      this->botright = botright;
      this->servo = servo;
    };
    int incremento (int last_angle) {
      if (last_angle >= 180) return last_angle;
      else return (last_angle + 3);
    }
    int decremento (int last_angle) {
      if (last_angle <= 0) return last_angle;
      else return (last_angle - 3);
    }
    void update() {
      float top = ((*topright).getIntensity() + (*topleft).getIntensity()) / 2;
      float bottom = ((*botleft).getIntensity() + (*botright).getIntensity()) / 2;
      float diferenca = fabs(top - bottom);

      if (diferenca > 1100) {
        if (top > bottom)  angle = incremento(angle);
        else angle = decremento(angle);
        (*servo).write(angle);
      }
    }

};
