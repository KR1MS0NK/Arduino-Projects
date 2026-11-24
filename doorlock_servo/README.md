# Doorlock Servo

## Overview
This project implements an embedded system sketch utilizing **Servo Motor, RFID Module (RC522 / 13.56 MHz contactless), Relay Module, Piezo Buzzer / Alarm** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`doorlock_servo.ino`**: Arduino sketch source code.
- **`circuit diagram.webp`**: Auxiliary source, header, or project resource file.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - Servo Motor
  - RFID Module (RC522 / 13.56 MHz contactless)
  - Relay Module
  - Piezo Buzzer / Alarm

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `BUZZER` | `2` | `OUTPUT` | Acoustic audible alert / alarm |
| `LED_G` | `5` | `OUTPUT` | Visual status indicator LED |
| `LED_R` | `4` | `OUTPUT` | Visual status indicator LED |
| `RELAY` | `Assigned in code` | `OUTPUT` | Relay coil control switch |
| `RST_PIN` | `9` | `Digital/Analog I/O` | Interface connection |
| `SERVO` | `3` | `Digital/Analog I/O` | PWM servo control signal |
| `SS_PIN` | `10` | `Digital/Analog I/O` | Interface connection |
| `Servo Control Pin` | `3` | `Digital/Analog I/O` | PWM servo control signal |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`MFRC522.h`** (MFRC522): RFID-RC522 13.56 MHz contactless reader/writer
- **`SPI.h`** (Built-in SPI): SPI communication bus
- **`Servo.h`** (Servo (Built-in)): RC Hobby Servo motor control

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 4 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
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
   - Open `doorlock_servo.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
