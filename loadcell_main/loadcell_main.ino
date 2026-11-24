#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "HX711.h"

#define calibration_factor -620.0 //This value is obtained using the SparkFun_HX711_Calibration sketch

#define LOADCELL_DOUT_PIN  5
#define LOADCELL_SCK_PIN  4

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//
HX711 scale;

void setup() {
  Serial.begin(9600);

   if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  Serial.println("HX711 scale demo");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(calibration_factor); //This value is obtained by using the SparkFun_HX711_Calibration sketch
  scale.tare(); //Assuming there is no weight on the scale at start up, reset the scale to 0

  Serial.println("Readings:");
}

void loop() {
  float dx = map(scale.get_units(), 0, 165, 0.0, 100.0);
  display.clearDisplay();
  display.print("Reading: ");
  display.print(scale.get_units(), 1); //scale.get_units() returns a float
  display.print(" g"); //You can change this to kg but you'll need to refactor the calibration_factor
  display.println();
  display.print(dx);
  display.print(" percent");
  display.println();
  
}
