# Water Bin

## Overview
> Code Name: Arduino Bluetooth Car with Front and Back Light Control 
> Code URI: https://circuitbest.com/category/arduino-projects/
> Before uploading the code you have to install the "Adafruit Motor Shield" library
> Open Arduino IDE >> Go to sketch >> Include Libray >> Manage Librays...  >> Search "Adafruit Motor Shield" >> Install the Library
> AFMotor Library: https://learn.adafruit.com/adafruit-motor-shield/library-install
> Author: Make DIY
> Author URI: https://circuitbest.com/author/admin/
> Description: This program is used to control a robot using an app that communicates with Arduino through an HC-05 Bluetooth Module.
> App URI: https://bit.ly/3mn6LuZ
> Version: 1.0
> License: Remixing or Changing this Thing is allowed. Commercial use is not allowed.

This folder contains Arduino sketch code for **Water Bin** running on the **Arduino Uno / Nano / Pro Mini (ATmega328P)** platform.

## Files in this Directory
- **`water_bin.ino`**: Arduino sketch source code.

## Hardware & Components
- **Target Microcontroller**: Arduino Uno / Nano / Pro Mini (ATmega328P)
## Software Dependencies & Libraries
The sketch relies on the following libraries:
- **`AFMotor.h`**: Required sketch dependency.

## How the Code Works
1. Initializes serial communication at 9600 baud for telemetry, debugging, or peripheral interfacing.

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
   - Open `water_bin.ino` in the Arduino IDE.
5. **Select Board & Port**:
   - Select `Arduino Uno / Nano / Pro Mini (ATmega328P)` under **Tools** > **Board**.
   - Connect your microcontroller via USB and select the detected COM port under **Tools** > **Port**.
6. **Compile & Upload**:
   - Click the **Upload** button (right arrow icon).
7. **Monitor Serial Output**:
   - Open **Tools** > **Serial Monitor**.
   - Set the baud rate to **9600 baud** to view system status, sensor readings, and debug traces.
