
class Axis {

  LightIntensity *topright;
  LightIntensity *topleft;
  LightIntensity *botleft;
  LightIntensity *botright;
  Servo* servo;

  uint8_t angle = 90;
  float last_diference = 0;

public:
  Axis(LightIntensity *topright, LightIntensity *topleft, LightIntensity *botleft, LightIntensity *botright, Servo* servo) {
    this->topright = topright;
    this->topleft = topleft;
    this->botleft = botleft;
    this->botright = botright;
    this->servo = servo;
  };

  float incremento (int last_angle, float diference) {
    if (last_angle >= 180) return last_angle;
    else return (last_angle + diference);
  }

  float decremento (int last_angle, float diference) {
    if (last_angle <= 0) return last_angle;
    else return (last_angle - diference);
  }
  //Assim que eu entender como essa funcao funciona eu tento otimizar ela.
  void update() {
    float top = ((*topright).getIntensity() + (*topleft).getIntensity()) / 2;
    float bottom = ((*botleft).getIntensity() + (*botright).getIntensity()) / 2;
    float diference = fabs(top - bottom);
    float delta = fabs(diference - last_diference);
    float p = 0.4, d = 0.1;
    float control = diference*p - delta*d;
    if (top > bottom)  angle = incremento(angle, control);
    else angle = decremento(angle, control);
    if(diference != last_diference){
      (*servo).write((int) angle); //Tenta impedir o servo de ficar pulando na mesma posicao.
      last_diference = diference; //Pra que gastar um ciclo extra desnessessariamente?
    }
  }

};
