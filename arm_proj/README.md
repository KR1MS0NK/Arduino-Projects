# Arm Proj

## Overview
> meArm analog joysticks version 1.3.1 - UtilStudio.com Dec 2018
>    Uses two analogue joysticks and four servos.
>    In version 1.3 was improved recording of coordinates.
>    Some bugs was removed.
>    First joystick moves gripper forwards, backwards, left and right,
>    button start/stop recording positions.
>    Second joystick moves gripper up, down, and closes and opens,
>    button start/stop playing recorded positions.
>    Press button for 2 seconds to autoplay.
>    Pins:
>    Arduino    Stick1    Stick2    Base   Shoulder  Elbow    Gripper   Record/
>       GND       GND       GND    Brown     Brown   Brown     Brown    Auto play
>        5V       VCC       VCC      Red       Red     Red       Red    LED
>        A0       HOR
>        A1       VER
>        PD2      BUTT
>        A2                 HOR
>        A3                 VER
>        PD3                BUTT
>        11                       Yellow
>        10                                 Yellow
>         9                                         Yellow
>         6                                                   Yellow
>        PD4                                                            X

This project implements an embedded system sketch utilizing **Servo Motor** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`arm_proj.ino`**: Arduino sketch source code.
- **`blob_3NHV1kXdjo.jfif`**: Auxiliary source, header, or project resource file.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - Servo Motor

### Pin Configurations & Connections

| Signal / Variable | Pin Number / Identifier | Configured Mode | Purpose / Notes |
| :--- | :--- | :--- | :--- |
| `basePin` | `11` | `Digital/Analog I/O` | Interface connection |
| `buttonPlayCount` | `0` | `Digital/Analog I/O` | User input push button / switch |
| `buttonPlayDelay` | `20` | `Digital/Analog I/O` | User input push button / switch |
| `countServo` | `4` | `Digital/Analog I/O` | PWM servo control signal |
| `elbowPin` | `9` | `Digital/Analog I/O` | Interface connection |
| `gdirPin` | `2` | `INPUT` | Interface connection |
| `gripperPin` | `6` | `Digital/Analog I/O` | Interface connection |
| `pinLedRecord` | `Assigned in code` | `OUTPUT` | Visual status indicator LED |
| `pinPlay` | `A5` | `INPUT` | Interface connection |
| `pinRecord` | `A4` | `INPUT` | Interface connection |
| `posShoulder` | `90` | `Digital/Analog I/O` | Interface connection |
| `shoulderPin` | `10` | `Digital/Analog I/O` | Interface connection |
| `xdirPin` | `0` | `INPUT` | Interface connection |
| `ydirPin` | `1` | `INPUT` | Interface connection |
| `zdirPin` | `3` | `INPUT` | Interface connection |

## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`Servo.h`** (Servo (Built-in)): RC Hobby Servo motor control

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.
2. Configures 7 I/O pins as INPUT/OUTPUT to interface with sensors and actuators.
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
   - Open `arm_proj.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
