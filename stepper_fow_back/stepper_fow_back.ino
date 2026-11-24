#include <Stepper.h>

const int stepsPerRevolution = 400;  // number of steps per revolution



Stepper myStepper1(stepsPerRevolution, 2, 4, 3, 5);

void setup() {
  // set the speed at 60 rpm:
  myStepper1.setSpeed(60);
  
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {

  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper1.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper1.step(-stepsPerRevolution);
  delay(500);
  


}
