// Motor driver pins
#define in1 2
#define in2 3
#define in3 4
#define in4 5

// Limit switch pin (Normally open connection)
#define limit 6

unsigned long timer = 3000;  // Default automatic cycle time
int incom = 0;               // For storing incoming serial data

// State machine variables for autom() function
enum AutoState { STOPPED, MOVING_FORWARD, MOVING_BACK };
AutoState autoState = STOPPED;
unsigned long previousMillis = 0;
bool autoMode = false;

void setup() {
  Serial.begin(9600);
  
  pinMode(limit, INPUT_PULLUP);  // Enable internal pull-up resistor
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Initialize all motor control pins to LOW
  hold();
  
  Serial.println("System Initialized");
}

void loop() {
  // Check for serial commands
  if (Serial.available() > 0) {
    incom = Serial.read();
    
    Serial.print("Received command: ");
    Serial.println(incom);
    
    switch(incom) {
      case '1':  // Move forward
        autoMode = false;
        forward();
        break;
      case '2':  // Move back
        autoMode = false;
        back();
        break;
      case '3':  // Stop
        autoMode = false;
        hold();
        break;
      case '4':  // Automatic mode
        autoMode = true;
        autoState = MOVING_FORWARD;
        previousMillis = millis();
        forward();
        Serial.println("Automatic cycle started");
        break;
      default:
        Serial.println("Unknown command");
    }
  }

  // Handle automatic mode state machine
  if (autoMode) {
    unsigned long currentMillis = millis();
    
    switch(autoState) {
      case MOVING_FORWARD:
        if (currentMillis - previousMillis >= timer) {
          autoState = MOVING_BACK;
          previousMillis = currentMillis;
          back();
        }
        break;
        
      case MOVING_BACK:
        if (currentMillis - previousMillis >= timer) {
          autoState = STOPPED;
          autoMode = false;
          hold();
          Serial.println("Automatic cycle completed");
        }
        break;
        
      case STOPPED:
        // Do nothing
        break;
    }
  }

  // Small delay to prevent serial buffer overflow
  delay(10);
}

void forward() {
  Serial.println("Moving forward"); 
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void hold() {
  Serial.println("Stopping"); 
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void back() {
  if (digitalRead(limit) == LOW) {  // Only move back if limit switch is not pressed
    Serial.println("Moving back"); 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  } else {
    Serial.println("Limit reached - cannot move back");
    hold();
  }
}
