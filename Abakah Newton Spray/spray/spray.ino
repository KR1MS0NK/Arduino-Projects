#include <ESP32Servo.h>

Servo servo;
const int servoPin = 18;      // Pin connected to the servo motor
const int buttonPin = 19;     // Pin connected to the button
const int initialPos = 90;   // Initial position of the servo motor
bool isSpraying = false;     // State of the spray (true or false)

void setup() {
  servo.attach(servoPin);    // Attach the servo to the specified pin
  servo.write(initialPos);   // Set the initial position of the servo motor
  pinMode(buttonPin, INPUT_PULLUP);  // Set the button pin as input with internal pull-up resistor
}

void loop() {
  int buttonState = digitalRead(buttonPin);  // Read the state of the button

  if (buttonState == LOW) {
    toggleSpray();          // If button is pressed, toggle the state of the spray
  }
}

void toggleSpray() {
  servo.write(0);            // Move the servo to press the spray
    isSpraying = true;
    delay(500);
    servo.write(initialPos);   // Move the servo to release the spray
    isSpraying = false;
//  if (isSpraying) {
//    servo.write(initialPos);   // Move the servo to release the spray
//    isSpraying = false;
//  } else {
//    servo.write(0);            // Move the servo to press the spray
//    isSpraying = true;
//  }
}
