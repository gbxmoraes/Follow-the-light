#include <Arduino.h>
#include <Servo.h>
#include <light_intensity.h>
#include <axis.h>

Servo base_servo;
Servo roll_servo;
/*
116.05, 969637.93
94.11, 257044.87
63.90, 181490.92);

10788.33, 5122322.50
68.69, 374481.43
*/
//posiciona o motor de acordo com a media dos dois primeiros LightIntensity  comparada aos dois outros.
LightIntensity topright(8200, A0, 1000, 1);
LightIntensity topleft(8200, A1, 1000, 1);
LightIntensity botleft(8200, A2, 1000, 1);
LightIntensity botright(8200, A3, 1000, 1);
Axis eixoY(&topright, &topleft, &botleft, &botright, &roll_servo);
Axis eixoX(&topleft, &botleft, &botright, &topright, &base_servo);

long time_control;
/*
class Benchmark {
public:
float min, max;
Benchmark(){
  max = 0;
  min = 3.4E+38;
}
void update(float v_val){
  if(v_val < min) min = v_val;
  if(v_val > max) max = v_val;
}
float get_min(){
  return min;
}
float get_max(){
  return max;
}
};

Benchmark bench_1;
Benchmark bench_2;
Benchmark bench_3;
Benchmark bench_4;
*/
void setup() {
  Serial.begin(9600);
  base_servo.attach(9); //must have pwm
  roll_servo.attach(10); //must have pwm
  time_control = millis();
}


void loop() {

  topright.update();
  topleft.update();
  botright.update();
  botleft.update();
  /*
  bench_1.update(topright.getIntensity());
  bench_2.update(topleft.getIntensity());
  bench_3.update(botright.getIntensity());
  bench_4.update(botleft.getIntensity());
  */
if(time_control + 300 < millis()){
  time_control = millis();
  Serial.print(topright.getIntensity());
  Serial.print(",");
  Serial.print(topleft.getIntensity());
  Serial.print(",");
  Serial.print(botleft.getIntensity());
  Serial.print(",");
  Serial.print(botright.getIntensity());
  Serial.println("");
  //Serial.print(bench_3.get_min());
  //Serial.print(", ");
  //Serial.println(bench_3.get_max());
}
  eixoX.update();
  eixoY.update();

  delay(50);

}
