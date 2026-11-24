#include <Servo.h>
Servo myservo;
void setup() {
  Serial.begin(9600);
  myservo.attach(7);
}
void loop() {
  myservo.write(0);
  Serial.println("Servo 0:");
  delay(2000);
  myservo.write(90);
  Serial.println("Servo 180:");
  delay(2000);
}
