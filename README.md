# SmartWaterDispenser

This project involves an Arduino-based water dispenser monitoring system using an ESP8266 module. The system detects the presence of a pet, checks the water level, and refills the water when necessary. It also sends alerts via MQTT if the water level needs to be manually refilled.
## Hardware Components

*    ESP8266 (NodeMCU)
*    Ultrasonic sensor (HC-SR04)
*    Water level sensor
*    Relay module to control the water pump
*    Water pump
*    Connecting wires and breadboard

## Software Components

* Arduino IDE for programming the ESP8266
* Python for monitoring MQTT messages
* MQTT broker (mqtt-dashboard.com)

# Arduino Code

The Arduino code connects to a WiFi network, reads sensor data, controls a water pump, and sends MQTT messages. Below is a brief overview of the main functions:
Functions

*    setup_wifi(): Connects the ESP8266 to the specified WiFi network.
*    callback(char* topic, byte* payload, unsigned int length): Placeholder for processing incoming MQTT messages.
 *   reconnect(): Reconnects to the MQTT broker if the connection is lost.
  *  setup(): Initializes serial communication, WiFi, MQTT, and sensor pins.
   * loop(): Main loop that checks for MQTT connection and periodically checks for pet presence.
* checkPetPresence(): Uses the ultrasonic sensor to detect the presence of a pet and checks the water level if no pet is detected.
 *   readSensor(): Reads the water level sensor and returns the current reading.

# Python MQTT Monitor

This Python script subscribes to the water-dispenser topic on the MQTT broker and prints any received messages to the console.
Prerequisites

*    Python 3.x
*    paho-mqtt library (install via pip)

## Installation

Install the required library using pip:


```pip install paho-mqtt```

## Running the Python Script

Save the script to a file, for example, mqtt_listener.py, and run it using Python:



```python mqtt_listener.py```

The script will print any messages received on the water-dispenser topic to the console, allowing you to monitor the status of your water dispenser.

# Phone monitoring

Install MQTT app from either AppStore or IoS store. Connect to the `water-dispenser` topic. Get messages when you need to refill the dispenser.
