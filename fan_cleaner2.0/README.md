# Fan Cleaner2.0

## Overview
> Motor driver pins

This folder contains Arduino sketch code for **Fan Cleaner2.0** running on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`fan_cleaner2.0.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `in1` | `2` | `OUTPUT` | Interface connection |
| `in2` | `3` | `OUTPUT` | Interface connection |
| `in3` | `4` | `OUTPUT` | Interface connection |
| `in4` | `5` | `OUTPUT` | Interface connection |
| `limit` | `6` | `INPUT_PULLUP` | Interface connection |

## Software Dependencies & Libraries
- No external libraries required (uses core Arduino standard framework).

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 5 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
3. Controls output states (toggling LEDs, buzzers, relays, or motor control signals) based on conditional logic.
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
   - Open `fan_cleaner2.0.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
