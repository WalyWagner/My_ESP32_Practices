# ESP32 FreeRTOS Project

## Overview
This project implements an ESP32-based system using FreeRTOS that remains inactive until a button is pressed. Upon pressing the button, the system counts a random time between 2 and 5 seconds, activates an LED indicator, and starts a high-priority task to measure input from an analog-to-digital converter (ADC) at the highest possible resolution for up to 2 seconds. After the measurement period, the LED indicator turns off, and the system returns to its initial state, displaying the results of the last conversion on the serial monitor.

## Project Structure
```
esp32-freertos-project
├── src
│   ├── main.cpp            # Entry point of the application
│   ├── tasks
│   │   ├── button_task.cpp # Implementation of the button task
│   │   ├── adc_task.cpp    # Implementation of the ADC task
│   │   └── led_task.cpp    # Management of the LED indicator
│   └── types
│       └── index.h        # Type definitions and structures
├── include
│   └── config.h           # Configuration settings
├── platformio.ini         # PlatformIO configuration file
└── README.md              # Project documentation
```

## Setup Instructions
1. Clone the repository or download the project files.
2. Open the project in your preferred IDE that supports PlatformIO.
3. Connect your ESP32 board to your computer.
4. Ensure the necessary libraries for FreeRTOS are included in the `platformio.ini` file.
5. Compile and upload the project to your ESP32 board.

## Usage
- Press the designated button to start the measurement process.
- The LED indicator will turn on during the measurement period.
- After 2 seconds, the LED will turn off, and the results will be displayed on the serial monitor.

## Functionality
- **Button Task**: Monitors the button state and initiates the measurement process upon detection of a press.
- **ADC Task**: Collects analog measurements for 2 seconds and stores the values with timestamps.
- **LED Task**: Controls the LED indicator based on signals from the button and ADC tasks.

## Notes
- Ensure that the button and LED are connected to the correct pins as defined in `config.h`.
- Modify the configuration settings in `config.h` as needed for your specific hardware setup.