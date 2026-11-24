# Air

## Overview
This project implements an embedded system sketch utilizing **Servo Motor, OLED Display (I2C/SPI SSD1306), Load Cell with HX711 24-bit ADC** on the **ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)** platform.

## Files in this Directory
- **`AIr.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)
- **Peripherals & Modules**:
  - Servo Motor
  - OLED Display (I2C/SPI SSD1306)
  - Load Cell with HX711 24-bit ADC

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `LED_PIN` | `19` | `OUTPUT` | Visual status indicator LED |
| `LOADCELL_CLK_PIN` | `33` | `Digital/Analog I/O` | Interface connection |
| `LOADCELL_DOUT_PIN` | `32` | `Digital/Analog I/O` | Interface connection |
| `SERVO_PIN` | `18` | `Digital/Analog I/O` | PWM servo control signal |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Adafruit_GFX.h`** (Adafruit GFX Library): Core graphics library for bitmap and vector rendering
- **`Adafruit_SSD1306.h`** (Adafruit SSD1306): I2C/SPI OLED display controller (128x64 / 128x32)
- **`ESP32Servo.h`** (ESP32Servo): Hardware timer PWM servo control for ESP32
- **`ESPAsyncWebServer.h`**: Required sketch dependency.
- **`HX711.h`** (HX711 (bogde)): 24-bit ADC amplifier for load cells and weight scales
- **`WiFi.h`** (WiFi (ESP32 core)): ESP32 Wi-Fi connectivity and TCP/IP stack
- **`WiFiManager.h`**: Required sketch dependency.
- **`Wire.h`** (Built-in Wire): I2C communication bus

## How the Code Works
1. Connects to the configured local Wi-Fi access point.
2. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
3. Configures 1 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
4. Samples analog sensor input values and converts them to digital readings.
5. Controls output states (toggling LEDs, buzzers, relays, or motor control signals) based on conditional logic.
6. Executes periodic delays between sensor reading cycles and action iterations.

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
   - Open `AIr.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
