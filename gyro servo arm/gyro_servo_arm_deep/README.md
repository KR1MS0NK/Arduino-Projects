# Gyro Servo Arm Deep

## Overview
This project implements an embedded system sketch utilizing **MPU-6050 6-Axis Accelerometer & Gyroscope Sensor, Servo Motor** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`gyro_servo_arm_deep.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - MPU-6050 6-Axis Accelerometer & Gyroscope Sensor
  - Servo Motor

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `BUTTON_PIN` | `4` | `INPUT_PULLUP` | User input push button / switch |
| `Servo Control Pin` | `6` | `Digital/Analog I/O` | PWM servo control signal |
| `buttonMode` | `0` | `Digital/Analog I/O` | User input push button / switch |
| `buttonState` | `0` | `Digital/Analog I/O` | User input push button / switch |
| `lastButtonState` | `0` | `Digital/Analog I/O` | User input push button / switch |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Adafruit_MPU6050.h`** (Adafruit MPU6050): MPU-6050 6-Axis Accelerometer and Gyroscope
- **`Adafruit_Sensor.h`** (Adafruit Unified Sensor): Sensor abstraction layer
- **`Servo.h`** (Servo (Built-in)): RC Hobby Servo motor control
- **`Wire.h`** (Built-in Wire): I2C communication bus

## How the Code Works
1. Initializes serial communication at 115200 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 1 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
3. Executes periodic delays between sensor reading cycles and action iterations.

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
   - Open `gyro_servo_arm_deep.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **115200 baud** to view system status, sensor readings, and debug traces.
