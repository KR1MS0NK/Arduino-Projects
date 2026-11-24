#include <Servo.h>

// Define the servo pins
const int servo1Pin = 9;
const int servo2Pin = 10;

// Create servo objects
Servo servo1;
Servo servo2;

void setup() {
  // Attach the servos to the pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
}

void loop() {
  // Move servo 1 from 0 to 90 degrees
  for (int angle = 0; angle <= 90; angle++) {
    servo1.write(angle);
    // Move servo 2 from 90 to 0 degrees
    servo2.write(90 - angle);
    delay(15); // wait for the servos to reach the position
  }
  delay(3000); // wait for 1 second before repeating

   // Move servo 1 from 0 to 90 degrees
  for (int angle = 0; angle <= 90; angle++) {
    servo2.write(angle);
    // Move servo 2 from 90 to 0 degrees
    servo1.write(90 - angle);
    delay(15); // wait for the servos to reach the position
  }
}
