# Control-system-of-motor-based-machine
Embedded C program for analog joystick to control a stepper motor and interact with painter on PC

In this project we used an MSP430G2553 chip to control a stepper motor using an analog joystick, while also allowing the user to paint on the PC screen using the joystick. We used an Embedded C program for the MCU, and the user control the application using Python from the PC, communicating the the MCU using UART protocol.

The application contain 4 modes, which translate to a 4 state final state machine on the MCU:
1. Manual control of motor using joystick- After moving the motor to angle 0, the motor will turn in real time to the angle the joystick is pointing to.
2. Joystick based PC painter- A screen will pop up on PC side, allowing the user to paint and erase on the screen using the joystick.
3. Calibration- The user will control the motor to stop at angle 0, then allow it to make a full 360 turn, allowing the system to calibrate a single step angle.
4. Script mode- The user can upload up to 3 pre-written sctipts and run them on the MCU, making it control motor, LEDS, and RGB bulb. Command list is included.
