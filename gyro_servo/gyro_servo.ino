#include <BMI160Gen.h>
#include <Servo.h>

Servo xServo;
Servo yServo;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  BMI160.begin(BMI160GenClass::SPI_MODE, 10);
  uint8_t dev_id = BMI160.getDeviceID();
  Serial.print("DEVICE ID: ");
  Serial.println(dev_id, HEX);
  BMI160.setGyroRange(250);

  xServo.attach(2);
  yServo.attach(3);
}

void loop() {
  int gxRaw, gyRaw, gzRaw;
  float gx, gy, gz;

  BMI160.readGyro(gxRaw, gyRaw, gzRaw);
  gx = convertRawGyro(gxRaw);
  gy = convertRawGyro(gyRaw);
  gz = convertRawGyro(gzRaw);

  int xAngle = map(gx, -250, 250, 0, 180);
  int yAngle = map(gy, -250, 250, 0, 180);

  xServo.write(xAngle);
  yServo.write(yAngle);

  delay(500);
}

float convertRawGyro(int gRaw) {
  float g = (gRaw * 250.0) / 32768.0;
  return g;
}
