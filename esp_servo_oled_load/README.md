# Esp Servo Oled Load

## Overview
This project implements an embedded system sketch utilizing **Servo Motor, OLED Display (I2C/SPI SSD1306), Load Cell with HX711 24-bit ADC** on the **ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)** platform.

## Files in this Directory
- **`esp_servo_oled_load.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)
- **Peripherals & Modules**:
  - Servo Motor
  - OLED Display (I2C/SPI SSD1306)
  - Load Cell with HX711 24-bit ADC

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `BATTERY_PIN` | `35` | `Digital/Analog I/O` | Interface connection |
| `loadCellSCK` | `33` | `Digital/Analog I/O` | Interface connection |
| `servoPin` | `18` | `Digital/Analog I/O` | PWM servo control signal |
| `servoPosition` | `0` | `Digital/Analog I/O` | PWM servo control signal |
| `statLED` | `19` | `OUTPUT` | Visual status indicator LED |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Adafruit_GFX.h`** (Adafruit GFX Library): Core graphics library for bitmap and vector rendering
- **`Adafruit_SSD1306.h`** (Adafruit SSD1306): I2C/SPI OLED display controller (128x64 / 128x32)
- **`ESP32Servo.h`** (ESP32Servo): Hardware timer PWM servo control for ESP32
- **`HX711.h`** (HX711 (bogde)): 24-bit ADC amplifier for load cells and weight scales
- **`SPI.h`** (Built-in SPI): SPI communication bus
- **`Wire.h`** (Built-in Wire): I2C communication bus

## How the Code Works
1. Configures 1 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
2. Samples analog sensor input values and converts them to digital readings.
3. Controls output states (toggling LEDs, buzzers, relays, or motor control signals) based on conditional logic.
4. Executes periodic delays between sensor reading cycles and action iterations.

## How to Run & Execution Guide
1. **Prerequisites & Tooling**:
   - Install the latest [Arduino IDE](https://www.arduino.cc/en/software).
   - Ensure the **ESP32 Board Package** is installed via the Boards Manager (`https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`).
2. **Install Required Libraries**:
   - In the Arduino IDE, navigate to **Sketch** > **Include Library** > **Manage Libraries...**.
   - Search for and install any external libraries listed in the Dependencies section above.
3. **Hardware Wiring**:
   - Connect the components and modules to your board following the pin configurations table above.
   - Ensure common ground (GND) is shared between all connected modules and the microcontroller.
4. **Open the Sketch**:
   - Open `esp_servo_oled_load.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
