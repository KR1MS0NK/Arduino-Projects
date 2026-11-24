# Hand Reciev

## Overview
This folder contains Arduino sketch code for **Hand Reciev** running on the **ESP8266 (NodeMCU v2/v3 / WeMos D1 Mini)** platform.

## Files in this Directory
- **`hand_reciev.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: ESP8266 (NodeMCU v2/v3 / WeMos D1 Mini)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `led0` | `16` | `OUTPUT` | Visual status indicator LED |
| `sensorValue0` | `0` | `Digital/Analog I/O` | Interface connection |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`ArduinoJson.h`** (ArduinoJson): Fast JSON parsing and serialization
- **`ESP8266WebServer.h`** (ESP8266WebServer): Lightweight HTTP web server for ESP8266
- **`ESP8266WiFi.h`** (ESP8266WiFi): ESP8266 Wi-Fi connectivity

## How the Code Works
1. Connects to the configured local Wi-Fi access point.
2. Initializes serial communication at 115200 baud for telemetry, debugging, or peripheral interfacing.
3. Configures 1 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
4. Controls output states (toggling LEDs, buzzers, relays, or motor control signals) based on conditional logic.
5. Executes periodic delays between sensor reading cycles and action iterations.

## How to Run & Execution Guide
1. **Prerequisites & Tooling**:
   - Install the latest [Arduino IDE](https://www.arduino.cc/en/software).
   - Ensure the **ESP8266 Board Package** is installed via the Boards Manager (`http://arduino.esp8266.com/stable/package_esp8266com_index.json`).
2. **Install Required Libraries**:
   - In the Arduino IDE, navigate to **Sketch** > **Include Library** > **Manage Libraries...**.
   - Search for and install any external libraries listed in the Dependencies section above.
3. **Hardware Wiring**:
   - Connect the components and modules to your board following the pin configurations table above.
   - Ensure common ground (GND) is shared between all connected modules and the microcontroller.
4. **Open the Sketch**:
   - Open `hand_reciev.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `ESP8266 (NodeMCU v2/v3 / WeMos D1 Mini)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **115200 baud** to view system status, sensor readings, and debug traces.
