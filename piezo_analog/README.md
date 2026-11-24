# Piezo Analog

## Overview
> code for using the piezoelectric plate as a sensor using the analog ports
> the piezoelectric plate is connected to the analog ort A0 with the actuator port(led)at D12
> the small potential difference produced by the piezoelectric late is detected by the arduino analog port
> when potential is detected above a certain value, system is triggered

This folder contains Arduino sketch code for **Piezo Analog** running on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`piezo_analog.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `ledPin` | `12` | `OUTPUT` | Visual status indicator LED |
| `sensorPin` | `A0` | `Digital/Analog I/O` | Interface connection |
| `sensorValue` | `0` | `Digital/Analog I/O` | Interface connection |

## Software Dependencies & Libraries
- No external libraries required (uses core Arduino standard framework).

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 1 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
3. Samples analog sensor input values and converts them to digital readings.
4. Controls output states (toggling LEDs, buzzers, relays, or motor control signals) based on conditional logic.
5. Executes periodic delays between sensor reading cycles and action iterations.

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
   - Open `piezo_analog.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
