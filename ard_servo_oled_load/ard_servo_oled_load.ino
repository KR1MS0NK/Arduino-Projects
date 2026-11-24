#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HX711.h>
#include <Servo.h> // Using the Arduino Servo library for Arduino Uno

#define OLED_RESET -1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HX711 scale;
Servo servo;

const int loadCellDT = 5;
const int loadCellSCK = 4;
const int servoPin = 9; // Change this to the appropriate pin for the Servo on Arduino Uno

unsigned long previousMillis = 0;
int sprayDuration = 5;
int servoPosition = 0;

bool pressed = false;
int setTime = 0;

int batt = 0; // Simulated battery value for Arduino Uno

void disp();
void spray();
void buttonPress();

void setup() {
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();
  display.setTextSize(1.5);
  display.setTextColor(SSD1306_WHITE);

  scale.begin(loadCellDT, loadCellSCK);
  scale.set_scale(-620);
  scale.tare();

  servo.attach(servoPin);
  servo.write(servoPosition);

  display.setCursor(0, 28);
  display.println("IP ADDRESS!");
  display.display();
  delay(60000);
  display.clearDisplay();
}

void loop() {
  disp();
  spray();
  buttonPress();  // Check for button press and update spray duration
}

void disp() {
  display.clearDisplay();
  display.setCursor(0, 0);
  float sprayLevel = map(scale.get_units(), 58, 165, 0.0, 100.0);

  display.print("BATTERY: ");
  display.print(batt);
  display.println("%");

  display.print("SPRAY: ");
  display.print(sprayLevel);
  display.print(" %");
  display.println();

  if (scale.get_units() <= 58) {
    sprayLevel = 0;
  } else if (scale.get_units() >= 165) {
    sprayLevel = 100;
  }

  display.print("TIME SET(m): ");
  display.println(sprayDuration);

  display.display();
  delay(100);
}

void spray() {
  unsigned long currentMillis = millis();
  unsigned long sprayDurationMillis = sprayDuration * 60000;

  if (currentMillis - previousMillis >= sprayDurationMillis) {
    for (int pos = 0; pos <= 45; pos += 1) {
      servo.write(pos);
      delay(10);
    }

    for (int pos = 45; pos >= 0; pos -= 1) {
      servo.write(pos);
      delay(10);
    }

    previousMillis = currentMillis; // Update the previousMillis value for the next spray cycle
  }
}

void buttonPress() {
  // Assuming you have button handling code that sets 'pressed' and 'setTime' variables

  if (pressed) {
    sprayDuration = setTime;
    previousMillis = millis(); // Reset the spray timer
    pressed = false;
  }
}
