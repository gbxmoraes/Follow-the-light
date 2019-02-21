const int buffer_size = 10;
class LightIntensity {

    float resistor;
    int pin;
    float data_buffer[buffer_size];
    float adjust_bright;
    float adjust_dark;
  public:
    LightIntensity(float resistor, int pin, float bright, float dark) {
      this->resistor = resistor;
      this->pin = pin;
      adjust_bright = bright;
      adjust_dark = dark;
    }

    void update(){
      for (int i = 0; i < buffer_size - 1; i++) {
        data_buffer[i] = data_buffer[i + 1];
      }
      data_buffer[buffer_size - 1] = mapear(pin, resistor);
    }

    float media (float *buffer) {
      float acc = 0;
      for (int i = 0; i < buffer_size; i++) {
        acc += buffer[i];
      }
      acc = acc / buffer_size;
      return acc;
    }

    float getIntensity() {
      float value =((media(data_buffer) - adjust_bright)/adjust_dark);
      if(adjust_bright > value) adjust_bright = value;
      if(adjust_dark < value) adjust_dark = value;
      return value;
    }
    float mapear(int porta, int resistencia) {
      float valorLightIntensity = 1023;
      //while (valorLightIntensity == 1023){
        valorLightIntensity = analogRead(porta);
        delay(3);
      //}
      valorLightIntensity = map(valorLightIntensity, 0, 1023, 0, 5000) / 1000.0;
      valorLightIntensity = ((resistencia * valorLightIntensity) / (5 - valorLightIntensity));
      if(valorLightIntensity<0) Serial.println("erro!");
      return valorLightIntensity;
    }
};
