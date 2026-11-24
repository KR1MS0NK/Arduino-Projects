# Loadcell Main

## Overview
This project implements an embedded system sketch utilizing **OLED Display (I2C/SPI SSD1306), Load Cell with HX711 24-bit ADC** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`loadcell_main.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - OLED Display (I2C/SPI SSD1306)
  - Load Cell with HX711 24-bit ADC

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `LOADCELL_DOUT_PIN` | `5` | `Digital/Analog I/O` | Interface connection |
| `LOADCELL_SCK_PIN` | `4` | `Digital/Analog I/O` | Interface connection |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Adafruit_GFX.h`** (Adafruit GFX Library): Core graphics library for bitmap and vector rendering
- **`Adafruit_SSD1306.h`** (Adafruit SSD1306): I2C/SPI OLED display controller (128x64 / 128x32)
- **`HX711.h`** (HX711 (bogde)): 24-bit ADC amplifier for load cells and weight scales
- **`SPI.h`** (Built-in SPI): SPI communication bus
- **`Wire.h`** (Built-in Wire): I2C communication bus

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.

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
   - Open `loadcell_main.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
