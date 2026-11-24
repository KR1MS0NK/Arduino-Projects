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

const float VREF = 3.3; // Reference voltage for ADC
const float VBAT_MIN = 3.3; // Minimum battery voltage
const float VBAT_MAX = 4.2; // Maximum battery voltage

void setup() {
    Wire.begin();

  // Initialize the OLED display
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.clearDisplay();

  // Set up text size and color
  display.setTextSize(1.5);
  display.setTextColor(WHITE);
  
  display.setCursor(0, 28);
  
  Serial.begin(115200);
}

void loop() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 4095.0) * VREF * (R1 + R2) / R2; // Convert raw value to voltage using voltage divider formula
  int batteryPercentage = min(max(map(voltage * 1000, VBAT_MIN * 1000, VBAT_MAX * 1000, 0, 100), 0), 100); // Map voltage to percentage (assuming battery voltage range between VBAT_MIN and VBAT_MAX)

  Serial.print(F("Battery Voltage: "));
  Serial.print(voltage);
  Serial.println(F(" V"));

  Serial.print(F("Battery Percentage: "));
  Serial.print(batteryPercentage);
  Serial.println(F(" %"));

    display.print(F("Battery Voltage: "));
    display.print(voltage);
    display.println(F(" V"));

    display.print(F("Battery Percentage: "));
    display.print(batteryPercentage);
    display.println(F(" %"));

    esp_sleep_enable_timer_wakeup(5000000); // Set timer to wake up after 5 seconds
    esp_deep_sleep_start(); // Enter deep sleep mode
}
