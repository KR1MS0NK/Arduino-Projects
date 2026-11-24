
#include <Servo.h>


Servo myservo;  // joint to robot


int pos1 = 0;    // variable to store the servo1 position

// twelve servo objects can be created on most boards

void setup() {
  myservo.attach(6);  // attaches the servo on pin 9 to the servo object
}

void loop() {

  for (pos1 = 0; pos1 <= 90; pos1 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }

  delay(1000);
  
  for (pos1 = 90; pos1 >= 0; pos1 -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15 ms for the servo to reach the position
  }

  delay(500);
//myservo1.write( pos1);
  
}
