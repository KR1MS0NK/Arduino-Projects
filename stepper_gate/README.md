# Stepper Gate

## Overview
This project implements an embedded system sketch utilizing **HC-SR04 Ultrasonic Distance Sensor, Stepper Motor & Driver** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`stepper_gate.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - HC-SR04 Ultrasonic Distance Sensor
  - Stepper Motor & Driver

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `echoPin` | `10` | `INPUT` | Ultrasonic echo pulse input |
| `rasp` | `Assigned in code` | `INPUT` | Interface connection |
| `trigPin` | `9` | `OUTPUT` | Ultrasonic pulse trigger output |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Stepper.h`** (Stepper (Built-in)): Unipolar and bipolar stepper motor control

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 3 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
3. Calculates distance by sending ultrasonic trigger pulses and timing the reflected echo.
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
   - Open `stepper_gate.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
