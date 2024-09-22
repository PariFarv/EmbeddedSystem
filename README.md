# Embedded System Final Project

This repository contains the final project for the Embedded Systems course. The project involves various embedded system functionalities such as ADC, PWM, Timer, UART communication, and task handling.

## Project Structure

- **`main.c`**: The main file that contains the core logic of the project.
- **Header Files**:
  - `ADC.h`: Functions and definitions related to Analog-to-Digital Conversion.
  - `CircularBuffer.h`: Implementation of a circular buffer for managing data.
  - `Parser.h`: Functions for parsing commands or data.
  - `PWM.h`: Pulse Width Modulation-related functions.
  - `TaskHandler.h`: Task scheduling and handling functions.
  - `TIMER.h`: Timer functionalities for scheduling and delays.
  - `UART.h`: Functions for UART communication.

## Getting Started

### Prerequisites

- MPLAB X IDE (or another compatible IDE for PIC microcontrollers)
- XC16 Compiler (for compiling the code)
- Hardware setup for testing (PIC Microcontroller, sensors, etc.)

### How to Build

1. Clone the repository:
    ```bash
    git clone https://github.com/yourusername/Group6_FinalProject.X.git
    cd Group6_FinalProject.X
    ```

2. Open the project in MPLAB X IDE.

3. Build the project.

4. Load the compiled hex file to your microcontroller.

### Features

- **ADC (Analog to Digital Converter)**: Converts analog signals to digital values.
- **PWM (Pulse Width Modulation)**: Used for controlling peripherals like motors.
- **Timer**: Used for managing time delays and task scheduling.
- **UART**: Serial communication functionality for debugging and interaction.

