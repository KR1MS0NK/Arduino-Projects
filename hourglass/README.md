# Hourglass

## Overview
This folder contains Arduino sketch code for **Hourglass** running on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`hourglass.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `led` | `2` | `Digital/Analog I/O` | Visual status indicator LED |
| `prevSwitchState` | `0` | `Digital/Analog I/O` | User input push button / switch |
| `switchPin` | `8` | `INPUT` | User input push button / switch |
| `switchState` | `0` | `Digital/Analog I/O` | User input push button / switch |
| `x` | `Assigned in code` | `OUTPUT` | Interface connection |

## Software Dependencies & Libraries
- No external libraries required (uses core Arduino standard framework).

## How the Code Works
1. Configures 2 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
2. Controls output states (toggling LEDs, buzzers, relays, or motor control signals) based on conditional logic.

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
   - Open `hourglass.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
