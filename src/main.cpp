#include <Arduino.h>
#include <Servo.h>
#include <light_intensity.h>
#include <axis.h>

#define baseServoPin 9
#define rollServoPin 10
#define serialTime 300

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
  base_servo.attach(baseServoPin); //must have pwm
  roll_servo.attach(rollServoPin); //must have pwm
  time_control = millis();
}


void loop() {
  //Atualiza os valores de intensidade de luz.
  topright.update();
  topleft.update();
  botright.update();
  botleft.update();
  //Atualiza a posicao dos servos.
  eixoX.update();
  eixoY.update();

  if(millis()-time_control>=serialTime){
    time_control = millis();
    Serial.print(topright.getIntensity());
    Serial.print(",");
    Serial.print(topleft.getIntensity());
    Serial.print(",");
    Serial.print(botleft.getIntensity());
    Serial.print(",");
    Serial.print(botright.getIntensity());
    Serial.println();
  }
}
