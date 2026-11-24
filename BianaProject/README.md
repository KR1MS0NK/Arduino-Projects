# Bianaproject

## Overview
This project implements an embedded system sketch utilizing **RFID Module (RC522 / 13.56 MHz contactless), Relay Module, Piezo Buzzer / Alarm** on the **ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)** platform.

## Files in this Directory
- **`BianaProject.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)
- **Peripherals & Modules**:
  - RFID Module (RC522 / 13.56 MHz contactless)
  - Relay Module
  - Piezo Buzzer / Alarm

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `BLUE_PIN` | `14` | `OUTPUT` | Visual status indicator LED |
| `BUZZER_PIN` | `15` | `OUTPUT` | Acoustic audible alert / alarm |
| `GREEN_PIN` | `13` | `OUTPUT` | Visual status indicator LED |
| `RED_PIN` | `12` | `OUTPUT` | Visual status indicator LED |
| `RELAY_PIN` | `22` | `OUTPUT` | Relay coil control switch |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`MFRC522Debug.h`**: Required sketch dependency.
- **`MFRC522DriverPinSimple.h`**: Required sketch dependency.
- **`MFRC522DriverSPI.h`**: Required sketch dependency.
- **`MFRC522v2.h`**: Required sketch dependency.

## How the Code Works
1. Initializes serial communication at 115200 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 5 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
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
   - Open `BianaProject.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **115200 baud** to view system status, sensor readings, and debug traces.
