// Pin assignments
int red = 2;
int yellow = 3; // Corrected spelling
int green = 4;
int buttonPin = 5; // Pin for the push button
int buttonState = 0; // Variable to store the button state

void setup() {
  pinMode(red, OUTPUT);
  digitalWrite(red, LOW);
  pinMode(yellow, OUTPUT);
  digitalWrite(yellow, LOW);
  pinMode(green, OUTPUT);
  digitalWrite(green, LOW);
  pinMode(buttonPin, INPUT); // Set button pin as input
  Serial.begin(9600);
}

void loop() {
  buttonState = digitalRead(buttonPin); // Read the state of the button

  if (buttonState == HIGH) { // If the button is pressed
    digitalWrite(red, HIGH); // Turn on the red light
    digitalWrite(yellow, LOW); // Ensure yellow light is off
    digitalWrite(green, LOW); // Ensure green light is off
  } else { // If the button is not pressed
    // Normal traffic light sequence
    // Yellow light flashing
    for (int i = 0; i < 2; i++) {
      digitalWrite(yellow, HIGH);
      delay(500);
      digitalWrite(yellow, LOW);
      delay(500);
    }

    // Green light sequence
    digitalWrite(green, HIGH);
    delay(10000);
    digitalWrite(green, LOW);
    for (int i = 0; i < 3; i++) {
      digitalWrite(green, HIGH);
      delay(500);
      digitalWrite(green, LOW);
      delay(500);
    }

    // Red light sequence
    digitalWrite(red, HIGH);
    delay(10000);
    digitalWrite(red, LOW);
    for (int i = 0; i < 3; i++) {
      digitalWrite(red, HIGH);
      delay(500);
      digitalWrite(red, LOW);
      delay(500);
    }
  }
}
