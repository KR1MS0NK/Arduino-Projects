#include <LiquidCrystal_I2C.h>   // I2C LCD library

// Pins
const int PIR_PIN = 2;          // Motion detector sensor pin
const int TRIG_PIN = 3;         // Ultrasonic sensor trigger pin
const int ECHO_PIN = 4;         // Ultrasonic sensor echo pin
const int LED_PIN = 11;         // LED in detection area (replacing the buzzer)
const int CONTROL_ROOM_BUZZER = 12; // Buzzer in control room
const int DISARM_BUTTON_PIN = 10;   // Push button to disarm

// LCD I2C address and dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the I2C address of the LCD, modify if needed

// Variables
int pirStateCurrent = LOW;       // Current state of motion detector pin
int pirStatePrevious = LOW;      // Previous state of motion detector pin
unsigned long prevTime = 0;      // Time tracking
unsigned long lastDetectionTime = 0; // Time of the last detection
bool isOn = false;               // Alarm status
bool alarmDisabled = false;      // Alarm disable status
const long ULTRASONIC_THRESHOLD = 35; // Threshold distance in cm
const unsigned long resetDelay = 5000; // Time to reset LCD (5 seconds)

void setup() {
  Serial.begin(9600);           // Initialize serial communication

  pinMode(PIR_PIN, INPUT);             // Set motion detector pin to input
  pinMode(TRIG_PIN, OUTPUT);           // Set ultrasonic trigger pin to output
  pinMode(ECHO_PIN, INPUT);            // Set ultrasonic echo pin to input
  pinMode(LED_PIN, OUTPUT);            // Set LED pin to output (replacing the buzzer)
  pinMode(CONTROL_ROOM_BUZZER, OUTPUT);// Set control room buzzer pin to output
  pinMode(DISARM_BUTTON_PIN, INPUT_PULLUP); // Set push button pin to input with pull-up

  lcd.init();                   // Initialize the LCD
  lcd.backlight();              // Turn on the LCD backlight
  lcd.print("System Ready");    // Display initial message
  Serial.println("System Ready");
}

long readUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2; // Calculate distance in cm
  Serial.print("Ultrasonic Distance: ");
  Serial.println(distance);
  return distance;
}

void blinkLED(int count, int delayTime) {
  for (int i = 0; i < count; i++) {
    digitalWrite(LED_PIN, HIGH);
    delay(delayTime);
    digitalWrite(LED_PIN, LOW);
    delay(delayTime);
  }
}

void loop() {
  pirStatePrevious = pirStateCurrent; // Store old state
  pirStateCurrent = digitalRead(PIR_PIN); // Read new state
  unsigned long currentTime = millis();

  // Disarm button check
  if (digitalRead(DISARM_BUTTON_PIN) == LOW) {
    alarmDisabled = true;
    isOn = false;
    digitalWrite(LED_PIN, LOW);
    digitalWrite(CONTROL_ROOM_BUZZER, LOW);
    lcd.clear();
    lcd.print("Alarm Disabled");
    Serial.println("Alarm Disabled");
    delay(500); // Reduced delay for quick reactivation
  }

  // Motion detection logic
  if (!alarmDisabled) {
    // Detect motion with PIR and check ultrasonic distance
    long distance = readUltrasonicDistance();
    if (pirStatePrevious == LOW && pirStateCurrent == HIGH) { 
      Serial.println("Motion detected by PIR!");
      lcd.clear();
      lcd.print("PIR Motion Detected!");
      blinkLED(3, 200);  // Blink 3 times quickly for PIR detection
      isOn = true;
      lastDetectionTime = currentTime;
    }

    if (pirStatePrevious == HIGH && pirStateCurrent == LOW) {
      Serial.println("Motion stopped by PIR!");
    }

    // Check ultrasonic sensor independently for closer objects
    if (distance < ULTRASONIC_THRESHOLD && !alarmDisabled) {
      Serial.println("Ultrasonic sensor triggered!");
      lcd.clear();
      lcd.print("Ultrasonic Detected!");
      blinkLED(5, 100);  // Blink 5 times quickly for ultrasonic detection
      isOn = true;
      lastDetectionTime = currentTime;
    }

    if (currentTime - prevTime >= 10000) {
      if (isOn) {
        digitalWrite(LED_PIN, LOW);
        digitalWrite(CONTROL_ROOM_BUZZER, LOW);
        Serial.println("Stop LED and buzzer");
        isOn = false;
      }
      prevTime = currentTime;
    }

    // Reset LCD if no motion or ultrasonic detection for a period
    if (currentTime - lastDetectionTime >= resetDelay) {
      lcd.clear();
      lcd.print("System Ready");
    }
  }
}
