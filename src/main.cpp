#include <Arduino.h>
#include <Servo.h>

Servo base_servo;
Servo roll_servo;




#include <light_intensity.h>
#include <axis.h>
//posiciona o motor de acordo com a media dos dois primeiros LightIntensity  comparada aos dois outros.
LightIntensity topright(390, A0, 0, 1);
LightIntensity topleft(390, A1, 0, 1);
LightIntensity botleft(390, A2, 0, 1);
LightIntensity botright(390, A3, 0, 1);
Axis eixoY(&topright, &topleft, &botleft, &botright, &roll_servo);
Axis eixoX(&topleft, &botleft, &botright, &topright, &base_servo);



void setup() {
  Serial.begin(9600);
  base_servo.attach(9); //must have pwm
  roll_servo.attach(10); //must have pwm
}

void loop() {

  topright.update();
  topleft.update();
  botright.update();
  botleft.update();

  Serial.print(topright.getIntensity());
  Serial.print(",");
  Serial.print(topleft.getIntensity());
  Serial.print(",");
  Serial.print(botleft.getIntensity());
  Serial.print(",");
  Serial.print(botright.getIntensity());
  Serial.println("");

  eixoX.update();
  eixoY.update();

  delay(50);

}
