Code Functionality
  1. Initialization: The code initializes the DHT11 sensor, LED pins, LDR pins,
        and the servo motor pin.
  2. WiFi Setup: Connects to the specified WiFi network using the provided credentials.
  3. Main Setup: Calls the setupMain(), setupAccion(), and setupDHT() functions to initialize
        the main functionalities, additional actions, and the DHT sensor respectively.
  5. Main Loop: Continuously calls the loopMain(), loopAccion(), and loopDHT() functions
       to handle the main loop, additional actions, and reading/printing DHT sensor values.

Requirements
  ESP32 Board
  DHT11 Sensor
  LEDs
  LDRs
  Servo Motor
  Arduino Libraries: DHT, ESP32Servo, ArduinoOTA, WiFi

Installation
  Clone this repository to your local machine.
  Open the ElectronicPart.ino file in the Arduino IDE.
  Ensure you have the necessary libraries installed.
  Upload the code to your ESP32 board.
