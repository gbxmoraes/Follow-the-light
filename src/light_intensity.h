#define sample_number 100
#define alpha 0.8

class LightIntensity {

  uint8_t pin;
  float resistor;
  float adjust_bright;
  float adjust_dark;
  float intensity = 1023.0; //A intensidade precisa comecar em um valor diferente de zero.

public:
  LightIntensity(float resistor, int pin, float bright, float dark) {
    this->resistor = resistor;
    this->pin = pin;
    if(pin>=14) pin -= 14; //Acontece que as constantes A0-A5 sao simplismente 14-19, entao essa linha re-mapeia elas a 0-5 para o multiplexador.
    pin &= 0x07; //Mascara bits irrelevantes para o multiplexador.
    adjust_bright = bright;
    adjust_dark = dark;
  }

  //A funcao update agora esta extremamente rapida. isso deve acabar com o problema de latencia onde o movimento nao chegava a um ponto de equilibrio
  void update(){
    float newIntensity = 0;
    ADMUX = (_BV(REFS0) | pin); //Selecionando o pino correto no multiplexador do registro de portas analogicas.
    //Leitura analogica e feita tao rapido quanto o ADC suporta. tao rapido que as 100 leituras terminam em 131 microsegundos.
    //Por comparacao, antes cada leitura levava aproximadamente 4 milisegundos por valor. isso e um aumento de 3077x em velocidade.
    for(int i=0; i<=sample_number; i++){
      ADCSRA |= _BV(ADSC); //Definindo bit ADSC no registro ADCSRA inicia o processo de conversao analogica.
      while(bit_is_set(ADCSRA, ADSC)); //Prende o processador num while que nao faz nada enquanto a conversao acontece.
      if(i!=0) newIntensity += ((ADCL | (ADCH<<8))/sample_number); //E sim, o primeiro resultado e descartado.
    }
    intensity /= sample_number;
    //Mapeando so o resultado final salva muito tempo de processamento. Operacoes de float sao bem lentas.
    newIntensity = map(newIntensity, 0, 1023, 0, 5000) / 1000.0;
    newIntensity = ((resistor * newIntensity) / (5 - newIntensity));
    //Ajusta a intensidade aos valores ajustados obtidos experimentalmente por ldr. Ainda me pergunto se isso e realmente necessario.
    newIntensity = (newIntensity-adjust_bright)/adjust_dark;
    if(adjust_bright > newIntensity) adjust_bright = newIntensity;
    if(adjust_dark < newIntensity) adjust_dark = newIntensity;
    //Filtragem exponencial pois nao gasta tanta memoria quanto media movel.
    intensity = alpha*newIntensity+(1-alpha)*intensity;
    //Desabilita filtragem. comente a linha acima e descomente a linha abaixo.
    //intensity = newIntensity;
  }

  float getIntensity() {
    return intensity;
  }
};
