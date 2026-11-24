#include <Wire.h>
#include <Servo.h>
#include <MPU6050.h>

MPU6050 mpu;

// Servo definitions
Servo forearmServo;     // Moves up/down: 45° to 180°
Servo baseUpDownServo;  // Moves up/down: 180° to 90°
Servo rotateServo;      // Rotates left/right: 0° to 180°

int buttonPin = 7;
int buttonState = 0;
int lastButtonState = 0;
int mode = 0;  // Toggles between 0 and 1 on button press

// Servo angles
int forearmPos = 90;
int baseUpDownPos = 135;
int rotatePos = 90;

// MPU6050 variables (from your gyro_basic_readings.ino)
int16_t ax, ay, az;
int16_t gx, gy, gz;
float AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  // Initialize MPU6050 with your configuration
  Serial.println("Initializing MPU6050...");
  mpu.begin();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed!");
    while (1);
  } else {
    Serial.println("MPU6050 connected successfully!");
  }

  // Attach servos
  forearmServo.attach(3);     // Forearm joint servo
  baseUpDownServo.attach(5);  // Base up/down servo
  rotateServo.attach(6);      // Rotation servo

  pinMode(buttonPin, INPUT_PULLUP);

  // Initial servo positions
  forearmServo.write(forearmPos);
  baseUpDownServo.write(baseUpDownPos);
  rotateServo.write(rotatePos);
  
  delay(1000);
  Serial.println("System ready!");
}

void loop() {
  // --- READ MPU6050 DATA (as in gyro_basic_readings.ino) ---
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Convert raw readings to g-forces and deg/s
  AcX = (float)ax / 16384.0;
  AcY = (float)ay / 16384.0;
  AcZ = (float)az / 16384.0;
  GyX = (float)gx / 131.0;
  GyY = (float)gy / 131.0;
  GyZ = (float)gz / 131.0;

  // --- BUTTON HANDLING ---
  buttonState = digitalRead(buttonPin);
  if (buttonState == LOW && lastButtonState == HIGH) {
    mode = !mode; // Toggle mode (0 ↔ 1)
    delay(250);   // Debounce delay
  }
  lastButtonState = buttonState;

  // --- FOREARM JOINT CONTROL (mode = 0) ---
  if (mode == 0) {
    if (AcX < -2 && AcZ < 9) {
      forearmPos--;
      if (forearmPos < 45) forearmPos = 45;
      forearmServo.write(forearmPos);
    } 
    else if (AcX > 2 && AcZ < 9) {
      forearmPos++;
      if (forearmPos > 180) forearmPos = 180;
      forearmServo.write(forearmPos);
    }
  }

  // --- BASE UP/DOWN CONTROL (mode = 1) ---
  if (mode == 1) {
    if (AcX < -2 && AcZ < 9) {
      baseUpDownPos++;
      if (baseUpDownPos > 180) baseUpDownPos = 180;
      baseUpDownServo.write(baseUpDownPos);
    } 
    else if (AcX > 2 && AcZ < 9) {
      baseUpDownPos--;
      if (baseUpDownPos < 90) baseUpDownPos = 90;
      baseUpDownServo.write(baseUpDownPos);
    }
  }

  // --- ROTATION CONTROL (Always Active) ---
  if (AcY < -2 && AcZ < 9) {
    rotatePos--;
    if (rotatePos < 0) rotatePos = 0;
    rotateServo.write(rotatePos);
  } 
  else if (AcY > 2 && AcZ < 9) {
    rotatePos++;
    if (rotatePos > 180) rotatePos = 180;
    rotateServo.write(rotatePos);
  }

  // --- SERIAL MONITOR DEBUG ---
  Serial.print("AcX: "); Serial.print(AcX);
  Serial.print("  AcY: "); Serial.print(AcY);
  Serial.print("  AcZ: "); Serial.print(AcZ);
  Serial.print("  Mode: "); Serial.print(mode);
  Serial.print("  Forearm: "); Serial.print(forearmPos);
  Serial.print("  Base: "); Serial.print(baseUpDownPos);
  Serial.print("  Rotate: "); Serial.println(rotatePos);

  delay(50);
}
