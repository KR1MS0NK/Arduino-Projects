#include <Wire.h>
#include <BMI160Gen.h>



const int8_t i2c_addr = 0x68; // Default I2C address for the BMI160

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize the BMI160
  BMI160.i2c_init(i2c_addr);

  // Set the accelerometer range to ±2 g
  BMI160.setAccelRange(BMI160::ACCEL_RANGE_2G);

  // Set the gyroscope range to ±2000 deg/s
  BMI160.setGyroRange(BMI160::GYRO_RANGE_2000DPS);
}

void loop() {
  // Read accelerometer data
  int16_t accelX, accelY, accelZ;
  BMI160.readAccel(accelX, accelY, accelZ);

  // Read gyroscope data
  int16_t gyroX, gyroY, gyroZ;
  BMI160.readGyro(gyroX, gyroY, gyroZ);

  // Print accelerometer and gyroscope data to serial
  Serial.print("Accel: ");
  Serial.print(accelX);
  Serial.print(", ");
  Serial.print(accelY);
  Serial.print(", ");
  Serial.println(accelZ);

  Serial.print("Gyro: ");
  Serial.print(gyroX);
  Serial.print(", ");
  Serial.print(gyroY);
  Serial.print(", ");
  Serial.println(gyroZ);

  delay(100);
}
