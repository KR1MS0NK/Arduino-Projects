# Pir Sensor

## Overview
This project implements an embedded system sketch utilizing **GSM/GPRS Cellular Module (SIM800L / A6), Piezo Buzzer / Alarm** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`PIR_Sensor.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - GSM/GPRS Cellular Module (SIM800L / A6)
  - Piezo Buzzer / Alarm

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `PIN_TO_SENSOR` | `2` | `INPUT` | Interface connection |
| `SoftwareSerial RX` | `8` | `Digital/Analog I/O` | Serial Receive pin |
| `SoftwareSerial TX` | `9` | `Digital/Analog I/O` | Serial Transmit pin |
| `buzzer` | `12` | `OUTPUT` | Acoustic audible alert / alarm |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`SoftwareSerial.h`** (SoftwareSerial): Emulated serial communication on digital pins

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
   - Open `PIR_Sensor.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
