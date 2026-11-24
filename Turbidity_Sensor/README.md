# Quality

## Overview
This project implements an embedded system sketch utilizing **Analog Turbidity Sensor (Water Quality)** on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`Turbidity_Sensor.ino`**: Arduino sketch source code.
- **`Determine Water Quality using Arduino and Turbidity Sensor  DIY Turbidity meter.png`**: Auxiliary source, header, or project resource file.
- **`Determine Water Quality using Arduino and Turbidity Sensor - DIY Turbidity meter.mp4`**: Auxiliary source, header, or project resource file.
- **`Determine Water Quality using Arduino and Turbidity Sensor DIY Turbidity meter.png`**: Auxiliary source, header, or project resource file.
- **`Proteus Simulation.pdsprj`**: Auxiliary source, header, or project resource file.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
- **Peripherals & Modules**:
  - Analog Turbidity Sensor (Water Quality)
## Software Dependencies & Libraries
- No external libraries required (uses core Arduino standard framework).

## How the Code Works
1. Samples analog sensor input values and converts them to digital readings.
2. Executes periodic delays between sensor reading cycles and action iterations.

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
   - Open `Turbidity_Sensor.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
