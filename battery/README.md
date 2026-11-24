# Battery

## Overview
This project implements an embedded system sketch utilizing **OLED Display (I2C/SPI SSD1306)** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`battery.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - OLED Display (I2C/SPI SSD1306)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `BATTERY_PIN` | `35` | `Digital/Analog I/O` | Interface connection |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Adafruit_GFX.h`** (Adafruit GFX Library): Core graphics library for bitmap and vector rendering
- **`Adafruit_SSD1306.h`** (Adafruit SSD1306): I2C/SPI OLED display controller (128x64 / 128x32)
- **`SPI.h`** (Built-in SPI): SPI communication bus
- **`Wire.h`** (Built-in Wire): I2C communication bus

## How the Code Works
1. Initializes serial communication at 115200 baud for telemetry, debugging, or peripheral interfacing.
2. Samples analog sensor input values and converts them to digital readings.
3. Executes periodic delays between sensor reading cycles and action iterations.

## How to Run & Execution Guide
1. **Prerequisites & Tooling**:
   - Install the latest [Arduino IDE](https://www.arduino.cc/en/software).
2. **Install Required Libraries**:
   - In the Arduino IDE, navigate to **Sketch** > **Include Library** > **Manage Libraries...**.
   - Search for and install any external libraries listed in the Dependencies section above.
3. **Hardware Wiring**:
   - Connect the components and modules to your board following the pin configurations table above.
   - Ensure common ground (GND) is shared between all connected modules and the microcontroller.
4. **Open the Sketch**:
   - Open `battery.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **115200 baud** to view system status, sensor readings, and debug traces.
