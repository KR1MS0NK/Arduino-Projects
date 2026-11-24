#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <HX711.h>
#include <ESP32Servo.h>

#define OLED_RESET -1  // Reset pin not used
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
HX711 scale;
Servo servo;

const int loadCellDT = 32;   // HX711 DT pin
const int loadCellSCK = 33;  // HX711 SCK pin
const int servoPin = 18;    // Servo control pin
const int statLED = 19;   //status LED;
const int BATTERY_PIN = 35;  // Battery check pin

unsigned long previousMillis = 0;

int sprayDuration = 5;
int setTime = 0;
int servoPosition = 0;

bool pressed = false;

int batteryPercentage = 0;
int R1 = 27000; // 27K ohms
int R2 = 100000; // 100K ohms

void disp();
void spray();
void battery();
void buttonPress();

void setup() {

   pinMode(statLED,OUTPUT); 
  // Initialize the I2C communication for the OLED display
  Wire.begin();

  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Set up text size and color
  display.setTextSize(1.5);
  display.setTextColor(WHITE);

  // Initialize the load cell
  scale.begin(loadCellDT, loadCellSCK);
  scale.set_scale(620);  // Adjust this value to calibrate the load cell
  scale.tare();           // Set the current weight as the zero point

  // Initialize the servo motor
  servo.attach(servoPin);
  servo.write(servoPosition);  // Set the initial position of the servo motor

  // Display startup text
  display.setCursor(0, 28);
  display.println("192.168.137.126");
  display.display();
  delay(10000);
  display.clearDisplay();
}

void loop() {

  digitalWrite(statLED,HIGH);
  delay(500);
  digitalWrite(statLED,LOW);
  delay(2000);
  
  disp();
  spray();
  battery();
  buttonPress();  // Check for button press and update spray duration

}

void disp() {
  display.clearDisplay();
  display.setCursor(0, 0);
  float sprayLevel = map(scale.get_units(), 58, 165, 0.0, 100.0);

  display.print("BATTERY: ");
  display.print(batteryPercentage);
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
  delay(100);  // Adjust the delay as per your requirements
}

void spray() {
  unsigned long currentMillis = millis();
  unsigned long sprayDurationMillis = sprayDuration * 60000;

  if (currentMillis - previousMillis >= sprayDurationMillis) {
    for (int pos = 0; pos <= 180; pos += 1) {
      servo.write(pos);
      delay(10);
    }

//    for (int pos = 45; pos >= 0; pos -= 1) {
//      servo.write(pos);
//      delay(10);
//    }

    previousMillis = currentMillis; // Update the previousMillis value for the next spray cycle
  }
}

void battery (){
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 4095.0) * 3.3 * (R1 + R2) / R2; // Convert raw value to voltage using voltage divider formula
  int batteryPercentage = map(voltage * 1000, 3300, 4200, 0, 100); // Map voltage to percentage (assuming battery voltage range between 3.3V and 4.2V)

  }

void buttonPress() {
  // Assuming you have button handling code that sets 'pressed' and 'setTime' variables

  if (pressed) {
    sprayDuration = setTime;
    previousMillis = millis(); // Reset the spray timer
    pressed = false;
  }
}
