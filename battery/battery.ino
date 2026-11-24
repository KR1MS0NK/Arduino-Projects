#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define BATTERY_PIN 35
#define R1 27000 // 27K ohms
#define R2 100000 // 100K ohms

#define OLED_RESET -1  // Reset pin not used
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {
    Wire.begin();

  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Set up text size and color
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 28);
  //display.display();
  //display.clearDisplay();

  
  Serial.begin(115200);
}

void loop() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 4095.0) * 3.3 * (R1 + R2) / R2; // Convert raw value to voltage using voltage divider formula
  int batteryPercentage = map(voltage * 1000, 3300, 4200, 0, 100); // Map voltage to percentage (assuming battery voltage range between 3.3V and 4.2V)

  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");

  Serial.print("Battery Percentage: ");
  Serial.print(batteryPercentage);
  Serial.println(" %");

    display.print("Battery Voltage: ");
  display.print(voltage);
  display.println(" V");

  display.print("Battery Percentage: ");
  display.print(batteryPercentage);
  display.println(" %");

  delay(5000); // Delay for 5 seconds before reading again
}
