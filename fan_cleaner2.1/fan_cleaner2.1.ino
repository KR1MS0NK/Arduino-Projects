// Motor driver pins
const int enA = 12;
const int in1 = 2;
const int in2 = 3;
const int in3 = 4;
const int in4 = 5;
const int enB = 13;

// Limit switch pin
const int limitSwitch = 6;

// Variables
unsigned long cleaningStartTime = 0;
const unsigned long cleaningDuration = 7000; // 5 seconds for cleaning cycle
bool isCleaning = false;
bool shouldStop = false;
bool isMovingBackward = false;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set motor control pins as outputs
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  // Set limit switch as input with pullup
  pinMode(limitSwitch, INPUT_PULLUP);
  
  // Initialize motors to stop
  stopMotors();
  
  Serial.println("Ceiling Fan Cleaner Ready");
}

void loop() {
  // Check for Bluetooth commands
  if (Serial.available()) {
    int command = Serial.read();
    Serial.print("Received command: ");
    Serial.println(command);
    
    // Process commands
    switch(command) {
      case '1': // Move forward
        if (!isCleaning) {
          moveForward();
          isMovingBackward = false;
        }
        break;
      case '2': // Move backward if limit switch pressed
        if (digitalRead(limitSwitch) == LOW) { // Switch pressed
          moveBackward();
          isMovingBackward = true;
        } else {
          stopMotors();
          Serial.println("Limit switch not pressed - cannot move back");
          isMovingBackward = false;
        }
        break;
      case '3': // Stop all movement
        emergencyStop();
        break;
      case '4': // Automatic cleaning cycle
        if (!isCleaning) {
          startCleaningCycle();
        }
        break;
    }
  }
  
  // Safety check: Stop if limit switch opens during backward movement
  if (isMovingBackward && digitalRead(limitSwitch) == HIGH) {
    Serial.println("Limit switch opened - stopping motors");
    emergencyStop();
  }
  
  // Handle automatic cleaning cycle
  if (isCleaning) {
    if (shouldStop) {
      emergencyStop();
      Serial.println("Cleaning cycle interrupted");
    } 
    else if (millis() - cleaningStartTime < cleaningDuration) {
      // Forward phase
      moveForward();
      isMovingBackward = false;
    } 
    else if (millis() - cleaningStartTime < cleaningDuration * 2) {
      // Backward phase
      if (digitalRead(limitSwitch) == LOW) {
        moveBackward();
        isMovingBackward = true;
      } else {
        Serial.println("Cannot move back - limit switch not pressed");
        emergencyStop();
      }
    } 
    else {
      // Cleaning cycle complete
      stopMotors();
      isCleaning = false;
      isMovingBackward = false;
      Serial.println("Cleaning cycle complete");
    }
  }
}

void moveForward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  Serial.println("Moving forward");
}

void moveBackward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enA, 255);
  analogWrite(enB, 255);
  Serial.println("Moving backward");
}

void stopMotors() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enA, 0);
  analogWrite(enB, 0);
  Serial.println("Motors stopped");
}

void emergencyStop() {
  stopMotors();
  shouldStop = false;
  isCleaning = false;
  isMovingBackward = false;
}

void startCleaningCycle() {
  isCleaning = true;
  shouldStop = false;
  cleaningStartTime = millis();
  Serial.println("Starting cleaning cycle");
}
