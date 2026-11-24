# Handle Electrical Equipments Using Sms By Tronics Lk

## Overview
> SUBSCRIBE 'Tronics Lk' YouTube Channel to get latest Electronics and Programming Tutorials
>    Wiring Guide
>    ============
>    GSM Module >
>               Tx Pin >  8 Pin
>               Rx Pin >  9 Pin
>               Gnd Pin > Gnd Pin
>               Vcc Pin > 3.7V - 4.2V (Power supply should be 2A)
>    Device 1 (LED 1) > 13
>    Device 1 (LED 2) > 12

This project implements an embedded system sketch utilizing **GSM/GPRS Cellular Module (SIM800L / A6)** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`Handle_Electrical_Equipments_Using_SMS_by_Tronics_Lk.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - GSM/GPRS Cellular Module (SIM800L / A6)

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `SIM800_RX_PIN` | `3` | `Digital/Analog I/O` | Serial Receive pin |
| `SIM800_TX_PIN` | `2` | `Digital/Analog I/O` | Serial Transmit pin |
| `device_1` | `Assigned in code` | `OUTPUT` | Interface connection |
| `device_2` | `Assigned in code` | `OUTPUT` | Interface connection |

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
   - Open `Handle_Electrical_Equipments_Using_SMS_by_Tronics_Lk.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
