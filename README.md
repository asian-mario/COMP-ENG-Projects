# COMP-ENG Projects

### Radar.ino
A simple and basic RADAR system that uses a 180 degree servo with an attached 3D printed frame to mount an Ultrasonic sensor to, uses an Arduino Uno and Processing to display graphics. Vulnerable to Gas/Volume interference.

### NVG.ino
A digital NVG system which would display thermal information to the user through a circular LCD display, the original IR camera module used was mistakenly bought as it only had an 8x8 grid, in the .ino file I have attempted interpolation to make 128x128 grid for added resolution. The circular LCD screen used is part of the AMG88xx series (replica). Temporary testing also used Processing to adjust temperature display colours.

### Overview
STRATCOM is a personal project which is essentially a layout of macro keys designed to work with the Helldivers 2 Strategem input. Using 5 Gateron Mechanical Keyboard switches, I connected the switches to the Arduino Pro Micro via breadboard and used an internal pull-up resistor in order to recieve the input signals to the Pro Micro. 

**Components Required:**

 1. Arduino Pro Micro
 2. 9V Battery (Connected to RAW)
 3. TMB12A05 Buzzer
 4. 16*2 LCD I2C
 5. Any Mechanical Keyboard Switches (x5)

**In order to use the project do the following (Breadboard):**

 1. Attach Pro Micro to the breadboard.
 2. Set up GND and 5V Power Rails on the side of the Breadboard (or seperate into 3.3V and 5V Rails).
 3. One of the switch's will need to connect to the 5V Power Rail and the other to GND Rail
 4. One of the Buzzer's Pins will be attached to a Pro Micro pin and the other to the GND Rail*
 5. Finally, use Pin 0 and 1 (SDA & SCL) on the Pro Micro and connect VCC to the 5V Rail and GND to the GND Rail
 6. Copy & Paste the .ino file into your own project

*Note that the TMB12A05 is an active buzzer, its recommended to get a passive buzzer but I got duped.
