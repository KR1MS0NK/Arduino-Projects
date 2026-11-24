#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Servo.h>

// MPU6050 object
Adafruit_MPU6050 mpu;

// Servo objects
Servo forearmServo;    // For up/down motion of forearm
Servo baseUpDownServo; // For up/down motion of base  
Servo baseRotateServo; // For left/right rotation of base

// Button pin
const int BUTTON_PIN = 4;

// Servo positions
int forearmPos = 90;        // Start at middle position (45-180 range)
int baseUpDownPos = 135;    // Start at middle position (90-180 range)  
int baseRotatePos = 90;     // Start at middle position (0-180 range)

// Button state variables
int buttonState = 0;
int lastButtonState = 0;
int buttonMode = 0;        // 0 = forearm control, 1 = base up/down control

void setup() {
  Serial.begin(115200);
  
  // Initialize MPU6050 with configuration from gyro_basic_readings.ino
  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  
  // Initialize servos
  forearmServo.attach(2);
  baseUpDownServo.attach(3);
  baseRotateServo.attach(6);
  
  // Initialize button
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  // Set initial servo positions
  forearmServo.write(forearmPos);
  baseUpDownServo.write(baseUpDownPos);
  baseRotateServo.write(baseRotatePos);
  
  delay(1000); // Allow servos to reach initial positions
}

void loop() {
  // Read accelerometer data
  float ax, ay, az;
  readAccelerometer(ax, ay, az);
  
  // Read button state with debouncing
  readButton();
  
  // Control servos based on accelerometer data and button mode
  controlServos(ax, ay, az);
  
  delay(50); // Small delay for stability
}

void readAccelerometer(float &ax, float &ay, float &az) {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Use acceleration values in m/s²
  ax = a.acceleration.x;
  ay = a.acceleration.y;
  az = a.acceleration.z;
  
  // Debug output
  Serial.print("AX: "); Serial.print(ax);
  Serial.print(" AY: "); Serial.print(ay); 
  Serial.print(" AZ: "); Serial.print(az);
  Serial.print(" ButtonMode: "); Serial.println(buttonMode);
}

void readButton() {
  buttonState = digitalRead(BUTTON_PIN);
  
  // Detect button press (LOW because of INPUT_PULLUP)
  if (buttonState == LOW && lastButtonState == HIGH) {
    // Toggle between forearm and base up/down control
    buttonMode = 1 - buttonMode; // Toggle between 0 and 1
    Serial.print("Button mode changed to: ");
    Serial.println(buttonMode);
    delay(250); // Debounce delay
  }
  
  lastButtonState = buttonState;
}

void controlServos(float ax, float ay, float az) {
  // Only control servos when mostly upright (z < 9 m/s², approximately < 0.9g)
  if (az < 9) {
    
    // 1. Forearm joint control (buttonMode = 0)
    if (buttonMode == 0) {
      if (ax > 2 && az < 9) { // Move forearm up
        if (forearmPos > 45) {
          forearmPos -= 3;
          forearmServo.write(forearmPos);
          Serial.println("Moving forearm UP");
        }
      }
      else if (ax < -2 && az < 9) { // Move forearm down
        if (forearmPos < 180) {
          forearmPos += 3;
          forearmServo.write(forearmPos);
          Serial.println("Moving forearm DOWN");
        }
      }
    }
    
    // 2. Base up/down control (buttonMode = 1)  
    else if (buttonMode == 1) {
      if (ax < -2 && az < 9) { // Move base up
        if (baseUpDownPos < 180) {
          baseUpDownPos += 3;
          baseUpDownServo.write(baseUpDownPos);
          Serial.println("Moving base UP");
        }
      }
      else if (ax > 2 && az < 9) { // Move base down
        if (baseUpDownPos > 90) {
          baseUpDownPos -= 3;
          baseUpDownServo.write(baseUpDownPos);
          Serial.println("Moving base DOWN");
        }
      }
    }
    
    // 3. Base rotation control (independent of button mode)
    if (ay > 2 && az < 9) { // Rotate base clockwise (viewed from top)
      if (baseRotatePos > 0) {
        baseRotatePos -= 3;
        baseRotateServo.write(baseRotatePos);
        Serial.println("Rotating base CLOCKWISE");
      }
    }
    else if (ay < -2 && az < 9) { // Rotate base counter-clockwise
      if (baseRotatePos < 180) {
        baseRotatePos += 3;
        baseRotateServo.write(baseRotatePos);
        Serial.println("Rotating base COUNTER-CLOCKWISE");
      }
    }
  }
  
  // Print current positions for debugging
  Serial.print("Forearm: "); Serial.print(forearmPos);
  Serial.print(" BaseUD: "); Serial.print(baseUpDownPos);
  Serial.print(" BaseRot: "); Serial.println(baseRotatePos);
} 
