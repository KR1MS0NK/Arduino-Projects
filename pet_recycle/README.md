# Pet Recycle

## Overview
This project implements an embedded system sketch utilizing **OLED Display (I2C/SPI SSD1306)** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`pet_recycle.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - OLED Display (I2C/SPI SSD1306)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `BTN_DOWN` | `4` | `INPUT_PULLUP` | User input push button / switch |
| `BTN_SEL` | `7` | `INPUT_PULLUP` | User input push button / switch |
| `BTN_UP` | `2` | `INPUT_PULLUP` | User input push button / switch |
| `HEATER_PIN` | `3` | `OUTPUT` | Interface connection |
| `LPWM_PIN` | `6` | `OUTPUT` | Interface connection |
| `RPWM_PIN` | `5` | `OUTPUT` | Interface connection |
| `THERMISTOR_PIN` | `A0` | `Digital/Analog I/O` | Interface connection |
| `editMotorSpeed` | `0` | `Digital/Analog I/O` | Interface connection |
| `motorSpeed` | `0` | `Digital/Analog I/O` | Interface connection |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Arduino.h`**: Required sketch dependency.
- **`U8g2lib.h`**: Required sketch dependency.
- **`Wire.h`** (Built-in Wire): I2C communication bus

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 6 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
3. Samples analog sensor input values and converts them to digital readings.
4. Executes periodic delays between sensor reading cycles and action iterations.

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
   - Open `pet_recycle.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
