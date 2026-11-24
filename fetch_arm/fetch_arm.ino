
#include <Servo.h>


Servo servo1;  // joint to robot
Servo servo2;  //joint holding spoon



void fetch (){

   // Move servo 1 from 0 to 90 degrees
  for (int angle = 0; angle <= 90; angle++) {
    // Move servo 2 from 90 to 0 degrees
    servo2.write(90 - angle);
    delay(15); // wait for the servos to reach the position
  }
  
  delay(500);

   // Move servo 1 from 0 to 90 degrees
  for (int angle = 0; angle <= 90; angle++) {
    servo2.write(angle);
    delay(15); // wait for the servos to reach the position
  }
  }


void feed(){

  // Move servo 1 from 0 to 90 degrees
  for (int angle = 0; angle <= 90; angle++) {
    servo1.write(angle);
    
    // Move servo 2 from 90 to 0 degrees
    servo2.write(90 - angle);
    delay(15); // wait for the servos to reach the position
  }
  delay(5000); // wait for 5 second before repeating

   // Move servo 1 from 0 to 90 degrees
  for (int angle = 0; angle <= 90; angle++) {
    servo2.write(angle);
    // Move servo 2 from 90 to 0 degrees
    servo1.write(90 - angle);
    delay(15); // wait for the servos to reach the position
  }
  }                                     

// twelve servo objects can be created on most boards


void setup() {
  servo1.attach(4);  // attaches the servo on pin 4 to the servo object
  servo2.attach(6);  // attaches the servo on pin 5 to the servo object
  servo1.write(0);
  servo2.write(90);
 
}

void loop() {
  
fetch();

delay(1000);

feed();

}
