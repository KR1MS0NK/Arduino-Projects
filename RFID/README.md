# Rfid

## Overview
> The DIY Life
> Michael Klements
> 27 January 2020

This project implements an embedded system sketch utilizing **Servo Motor, RFID Module (RC522 / 13.56 MHz contactless)** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`RFID.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - Servo Motor
  - RFID Module (RC522 / 13.56 MHz contactless)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `Servo Control Pin` | `3` | `Digital/Analog I/O` | PWM servo control signal |
| `accessGrantedSize` | `2` | `Digital/Analog I/O` | Interface connection |
| `greenLEDPin` | `6` | `OUTPUT` | Visual status indicator LED |
| `redLEDPin` | `5` | `OUTPUT` | Visual status indicator LED |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`RFID.h`**: Required sketch dependency.
- **`SPI.h`** (Built-in SPI): SPI communication bus
- **`Servo.h`** (Servo (Built-in)): RC Hobby Servo motor control

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 2 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
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
   - Open `RFID.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
