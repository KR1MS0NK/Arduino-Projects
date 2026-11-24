# Master

## Overview
This folder contains Arduino sketch code for **Master** running on the **ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)** platform.

## Files in this Directory
- **`master.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `buttonPin` | `0` | `INPUT_PULLUP` | User input push button / switch |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`WiFi.h`** (WiFi (ESP32 core)): ESP32 Wi-Fi connectivity and TCP/IP stack
- **`esp_now.h`**: Required sketch dependency.

## How the Code Works
1. Connects to the configured local Wi-Fi access point.
2. Initializes serial communication at 115200 baud for telemetry, debugging, or peripheral interfacing.
3. Configures 1 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
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
   - Open `master.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `ESP32 (NodeMCU-32S / ESP32 Dev Module / ESP32-CAM / ESP32-S3)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **115200 baud** to view system status, sensor readings, and debug traces.
