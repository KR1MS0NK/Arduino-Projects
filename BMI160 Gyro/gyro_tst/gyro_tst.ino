#include <Wire.h>
#include <BMI160Gen.h>  // Include the BMI160 library

BMI160GenClass IMU;  // Create an instance of the BMI160 class

void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!IMU.begin()) {
    Serial.println("Failed to initialize BMI160! Check your connections.");
    while (1);
  }
}

void loop() {
  // Read gyroscope data
  BMI160RawData gyroData = IMU.readGyro();

  // Print the raw gyroscope data
  Serial.print("X: "); Serial.print(gyroData.x);
  Serial.print("\tY: "); Serial.print(gyroData.y);
  Serial.print("\tZ: "); Serial.println(gyroData.z);

  delay(500);  // Add a delay to avoid overwhelming the serial monitor
}
