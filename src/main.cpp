#include <Arduino.h>
#include <Servo.h>
#include <light_intensity.h>
#include <axis.h>

Servo base_servo;
Servo roll_servo;

//posiciona o motor de acordo com a media dos dois primeiros LightIntensity  comparada aos dois outros.
LightIntensity topright(8200, A0, 1000, 1);
LightIntensity topleft(8200, A1, 1000, 1);
LightIntensity botleft(8200, A2, 1000, 1);
LightIntensity botright(8200, A3, 1000, 1);
Axis eixoY(&topright, &topleft, &botleft, &botright, &roll_servo);
Axis eixoX(&topleft, &botleft, &botright, &topright, &base_servo);

long time_control;

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
}
  eixoX.update();
  eixoY.update();

  //delay(50);

}
