#include <Stepper.h>

const int stepsPerRevolution = 2048;  // Change this to match your stepper motor

// Initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 10, 9, 11);

const int waterSensorPin = A0;
const int redLEDPin = 2;
const int greenLEDPin = 3;

void setup() {
  
  
  // Set the LED pins as outputs:
  pinMode(redLEDPin, OUTPUT);
  pinMode(greenLEDPin, OUTPUT);

  // Set the water sensor pin as input:
  pinMode(waterSensorPin, INPUT);

  // Set the stepper speed:
  myStepper.setSpeed(60);  // Adjust the speed as needed

  // Start with the red LED off and green LED off:
  digitalWrite(redLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);

  Serial.begin(9600);  // Begin serial communication for debugging
}

void loop() {
  int waterSensorValue = analogRead(waterSensorPin);

  // Adjust threshold value based on your water sensor and testing
  if (waterSensorValue < 500) {  // Assume water is not detected when value is less than 500
    myStepper.step(stepsPerRevolution / 100);  // Move the stepper motor
    digitalWrite(redLEDPin, HIGH);  // Turn on the red LED
    digitalWrite(greenLEDPin, LOW);  // Turn off the green LED
    Serial.println("No water detected. Stepper running, Red LED on.");
  } else {
    myStepper.step(0);  // Stop the stepper motor
    digitalWrite(redLEDPin, LOW);  // Turn off the red LED
    digitalWrite(greenLEDPin, HIGH);  // Turn on the green LED
    Serial.println("Water detected. Stepper stopped, Green LED on.");
  }

  delay(100);  // Short delay for stability
}
