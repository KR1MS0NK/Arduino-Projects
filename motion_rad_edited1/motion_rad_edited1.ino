#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

// Ultrasound initialize
const int trigPin = 3;
const int echoPin = 4;
float duration, distance;

// PIR initialize
int sensor = 2;              // the pin that the sensor is attached to
int state = LOW;             // by default, no motion detected
int val = 0;                 // variable to store the sensor status (value)

// Buzzers initialize
int buzzer1 = 11;
int buzzer2 = 12;

// Push buttons and LED
const int button1 = 5;    // Pin for the first push button
const int button2 = 6;    // Pin for the second push button
const int led = 7;        // Pin for LED indicator

bool systemEnabled = true;  // Variable to keep track of whether the detection system is enabled

void detect() {
  lcd.setCursor(1, 0);
  lcd.print("Alarm Triggered!");
  digitalWrite(buzzer1, HIGH);
  digitalWrite(buzzer2, HIGH);
  delay(5000);
  digitalWrite(buzzer1, LOW);
  digitalWrite(buzzer2, LOW);
}

void setup() {
  pinMode(sensor, INPUT);    // initialize sensor as an input
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buzzer2, OUTPUT);
  pinMode(buzzer1, OUTPUT);
  
  pinMode(button1, INPUT_PULLUP); // Set button1 as input with internal pull-up resistor
  pinMode(button2, INPUT_PULLUP); // Set button2 as input with internal pull-up resistor
  pinMode(led, OUTPUT);           // Set LED pin as output
  
  Serial.begin(9600);        // initialize serial
  lcd.init();                // initialize the lcd 
  lcd.backlight();
  lcd.setCursor(3, 0);
  lcd.print("System Active");
}

void loop() {
  // Check if any of the buttons are pressed to disable the system
  if (digitalRead(button1) == LOW ) {
    systemEnabled = false;
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("System Disabled");
    digitalWrite(led, LOW);
    digitalWrite(buzzer1, LOW); // Turn off the buzzers if the system is disabled
    digitalWrite(buzzer2, LOW);
  }
//   else {
//    systemEnabled = true;
//    lcd.clear();
//    lcd.setCursor(3, 0);
//    lcd.print("System Active");
//  }

  // If the system is enabled, proceed with motion detection
  if (systemEnabled) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration * 0.0343) / 2;

    Serial.print("Distance: ");
    Serial.println(distance);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("System Active");

    val = digitalRead(sensor);   // read sensor value
    if (val == HIGH) {           // check if the sensor is HIGH
      delay(100);                // delay 100 milliseconds 

      if (state == LOW) {
        Serial.println("Motion detected!");
        lcd.setCursor(1, 0);
        lcd.print("Motion detected!");
        state = HIGH;       // update variable state to HIGH

        if (distance <= 100){
          digitalWrite(led, HIGH);  // Turn on LED indicator
          detect();  // Call the detect function
          lcd.clear();
          lcd.setCursor(3, 0);
        } else {
          // Blink the LED 3 times if only motion is detected
          for (int i = 0; i < 3; i++) {
            digitalWrite(led, HIGH);
            delay(500);  // LED on for 500ms
            digitalWrite(led, LOW);
            delay(500);  // LED off for 500ms
          }
        }
      }
    } 
    else {
      delay(200);             // delay 200 milliseconds 

      if (state == HIGH) {
        Serial.println("Motion stopped!");
        state = LOW;       // update variable state to LOW
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("Motion Stopped!");
        digitalWrite(led, LOW);  // Turn off LED indicator
      }
    }
  }
}
