# Radar System with Ultrasonic Sensors, TFT Display, and Servo Control

## Overview

This project implements a radar system using STM32 microcontrollers. The radar system continuously sweeps an ultrasonic sensor over a 180-degree arc using a servo motor and displays the measured distance to any obstacles on a TFT screen. The system controls LEDs based on the proximity of objects and can be managed via a remote control to start/stop the radar or adjust its speed.

![WhatsApp Image 2024-10-07 at 11 10 20 PM](https://github.com/user-attachments/assets/4bcf9d45-cba9-4db7-a566-b783e1c212e2)


## Features

- **Ultrasonic Distance Sensing:** Detects objects within the radar's range using an ultrasonic sensor.
- **Servo Motor Control:** A servo motor sweeps the ultrasonic sensor from 0 to 180 degrees and back, simulating a radar scan.
- **TFT Display Output:** Displays the current angle and distance of detected objects in real-time on a TFT display.
- **LED Indicators:** Two LEDs are used to indicate proximity:
  - Red LED turns ON if an object is within 5 cm.
  - Green LED turns ON if no object is within 5 cm.
- **Remote Control Support:** Use a remote control to:
  - Start or stop the radar.
  - Increase or decrease the scanning speed.
- **PWM-Based Servo Control:** A PWM signal controls the servo motor, determining the angle of rotation.
- **Adjustable Scanning Speed:** Radar speed is adjustable based on the delay between sweeps.

## Hardware Components

- **STM32F103/STM32F401 Microcontroller**: Core controller for the system.
- **Ultrasonic Sensor (HC-SR04)**: Used for distance measurement.
- **Servo Motor**: Controls the angular position of the ultrasonic sensor.
- **TFT Display**: Displays angle and distance values.
- **Red and Green LEDs**: Indicate object proximity.
- **Remote Control**: For managing radar operations (start/stop, speed control).
- **SPI Interface**: Used for communication between the MCU and TFT display.
- **Bluetooth Module and ESP8266 (Optional)**: Can be integrated for remote control and monitoring.

## Software Components

- **STM32 Peripheral Drivers**: For GPIO, USART, RCC, Timer, SPI, and SysTick.
- **PWM Configuration**: To control the servo motor based on the calculated angles.
- **Ultrasonic Sensor Driver**: Handles the trigger and echo to calculate distance.
- **TFT Display Driver**: For updating the display with angle and distance information.
- **Remote Control Interface**: Captures and processes remote control inputs for controlling the radar system.

## Setup and Configuration

### 1. Pin Configuration

- **GPIOA, Pin 5**: SPI Clock (SCK)
- **GPIOA, Pin 7**: SPI MOSI (Master Output Slave Input)
- **GPIOA, Pin 9**: Ultrasonic Sensor Trigger Pin
- **GPIOA, Pin 11**: Red LED
- **GPIOB, Pin 1**: Green LED
- **GPIOA, Pin 8**: PWM Output for Servo Control

### 2. Peripheral Initialization

- **RCC (Reset and Clock Control)**: Initializes and enables clocks for GPIOA, GPIOB, SPI1, Timer1, Timer2, Timer3, and other peripherals.
- **SPI1 Interface**: Initializes communication for the TFT display.
- **SysTick**: Provides time delays for controlling the speed of operations.
- **Timer3**: Configured for PWM generation to control the servo motor's angular position.
- **Timer2**: Handles microsecond-level delays for precise timing (used for ultrasonic sensor and other delay needs).

### 3. TFT Display

- The display is initialized with `HTFT_voidInit()` and updated with current angle and distance values using the `displayInfo()` function.
- Angle and distance information is dynamically updated only when their values change, reducing redundant display updates.

### 4. Ultrasonic Sensor Operation

- The ultrasonic sensor is triggered using `HULTRA_Trig()`, and the distance is measured using `HULTRA_Distance()`.
- Distance values are used to control the LEDs:
  - Red LED turns ON if the measured distance is less than 5 cm.
  - Green LED turns ON if the distance is greater than or equal to 5 cm.

### 5. Remote Control Operation

- The remote control allows users to:
  - **Start/Stop the Radar**: Toggle the radar's sweeping operation.
  - **Adjust Speed**: Increase or decrease the radar's speed by modifying the delay between servo movements.
- Key handling is performed in the `HandleRemoteKey()` function, which processes the pressed key and adjusts the radar accordingly.

### 6. Servo Motor Control

- The servo motor is controlled via PWM signals, which are updated as the radar sweeps from 0 to 180 degrees and back.
- The angle is mapped to a PWM signal using the `map()` function, ensuring precise movement of the servo based on the calculated angle.

## Code Structure

- **Main Function**: Initializes all peripherals and handles the main radar loop, performing sweeps and checking for remote control inputs.
- **Peripheral Initialization**: The necessary peripherals (GPIO, SPI, timers, etc.) are initialized at the start of the program.
- **Radar Sweeping Logic**: The servo motor sweeps the ultrasonic sensor across 0 to 180 degrees, capturing distance data and updating the TFT display and LEDs.
- **Remote Key Handling**: Manages key inputs from the remote control to start/stop the radar or adjust the speed.

## Usage Instructions

1. **Power the System**: Ensure that all hardware components are connected and powered appropriately.
2. **Initialize the Radar**: Upon powering up, the radar system initializes its peripherals and starts the radar sweep automatically.
3. **Use the Remote Control**:
   - Press the `STOP` key to toggle between start and stop states.
   - Use the `MODE` key to increase the speed (reduce delay).
   - Press the `MUTE` key to decrease the speed (increase delay).
4. **Monitor Output**: View the current angle and distance on the TFT display, and observe the LEDs indicating object proximity.

## Future Enhancements

- **Bluetooth Integration**: Add Bluetooth support to remotely monitor the radar system via a mobile app.
- **FOTA Updates**: Implement Firmware Over-The-Air (FOTA) updates to easily upgrade the system.
- **Multiple Sensor Support**: Extend the system to support multiple ultrasonic sensors for wider area coverage.

