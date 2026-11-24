#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <Mouse.h>

Adafruit_MPU6050 mpu;

// Button pins
const int leftButtonPin = 2;
const int rightButtonPin = 3;

// Movement thresholds
const float movementThreshold = 2.0;  // rad/s for sensitivity
const float deadZone = 0.5;          // rad/s dead zone to prevent drift

// Timing
unsigned long lastReadTime = 0;
const int readInterval = 10; // ms between readings

// Button states
bool leftButtonState = false;
bool rightButtonState = false;
bool lastLeftButtonState = false;
bool lastRightButtonState = false;

// Calibration offsets
float gxOffset = 0, gyOffset = 0, gzOffset = 0;

void setup(void) {
  // Initialize buttons
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  
  // Initialize serial for debugging
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 Mouse Test!");

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Configure MPU6050 with same settings as your code
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
  
  // Calibrate gyroscope
  calibrateGyro();
  
  // Initialize mouse
  Mouse.begin();
  
  Serial.println("Mouse emulator ready!");
  delay(1000);
}

void loop() {
  unsigned long currentTime = millis();
  
  if (currentTime - lastReadTime >= readInterval) {
    lastReadTime = currentTime;
    
    // Process mouse movement
    processMouseMovement();
    
    // Process button clicks
    processButtons();
  }
}

void processMouseMovement() {
  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Apply calibration offsets to gyroscope
  float gyroX = g.gyro.x - gxOffset;
  float gyroY = g.gyro.y - gyOffset;
  float gyroZ = g.gyro.z - gzOffset;

  int mouseX = 0;
  int mouseY = 0;
  
  // Use gyroscope for mouse movement
  // Map gyroscope values to mouse movement
  if (abs(gyroY) > deadZone) {
    mouseX = mapFloatToInt(gyroY, -movementThreshold, movementThreshold, -15, 15);
  }
  
  if (abs(gyroX) > deadZone) {
    mouseY = mapFloatToInt(gyroX, -movementThreshold, movementThreshold, -15, 15);
  }
  
  // Move mouse if there's significant movement
  if (mouseX != 0 || mouseY != 0) {
    Mouse.move(mouseX, mouseY, 0);
    
    // Debug output (optional - comment out if too verbose)
    /*
    Serial.print("Mouse moved - X: ");
    Serial.print(mouseX);
    Serial.print(", Y: ");
    Serial.print(mouseY);
    Serial.print(" | Gyro - X: ");
    Serial.print(gyroX, 4);
    Serial.print(", Y: ");
    Serial.print(gyroY, 4);
    Serial.println(" rad/s");
    */
  }
}

void processButtons() {
  // Read current button states
  leftButtonState = !digitalRead(leftButtonPin);  // Inverted because of INPUT_PULLUP
  rightButtonState = !digitalRead(rightButtonPin);
  
  // Left button click (mouse left click)
  if (leftButtonState && !lastLeftButtonState) {
    Mouse.press(MOUSE_LEFT);
    Serial.println("Left button pressed");
  }
  if (!leftButtonState && lastLeftButtonState) {
    Mouse.release(MOUSE_LEFT);
    Serial.println("Left button released");
  }
  
  // Right button click (mouse right click)
  if (rightButtonState && !lastRightButtonState) {
    Mouse.press(MOUSE_RIGHT);
    Serial.println("Right button pressed");
  }
  if (!rightButtonState && lastRightButtonState) {
    Mouse.release(MOUSE_RIGHT);
    Serial.println("Right button released");
  }
  
  // Save current states for next comparison
  lastLeftButtonState = leftButtonState;
  lastRightButtonState = rightButtonState;
}

void calibrateGyro() {
  Serial.println("Calibrating gyroscope... Keep sensor still!");
  delay(3000);
  
  float gxSum = 0, gySum = 0, gzSum = 0;
  const int numSamples = 500;
  
  for (int i = 0; i < numSamples; i++) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    gxSum += g.gyro.x;
    gySum += g.gyro.y;
    gzSum += g.gyro.z;
    
    delay(5);
  }
  
  gxOffset = gxSum / numSamples;
  gyOffset = gySum / numSamples;
  gzOffset = gzSum / numSamples;
  
  Serial.println("Gyroscope calibration complete!");
  Serial.print("Offsets - X: ");
  Serial.print(gxOffset, 6);
  Serial.print(", Y: ");
  Serial.print(gyOffset, 6);
  Serial.print(", Z: ");
  Serial.println(gzOffset, 6);
}

// Helper function to map float values to integers
int mapFloatToInt(float x, float in_min, float in_max, int out_min, int out_max) {
  // Constrain input value
  if (x < in_min) x = in_min;
  if (x > in_max) x = in_max;
  
  return (int)((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}
