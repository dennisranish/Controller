# Controller
An Arduino library for creating web based game-style controllers to control an esp8266/esp32 using your phone or computer over wifi. No installation is required on the controlling device, a web page is sent when it connects to the server, and live two-way communication is done through websockets. The controller ui layout is fully customizable, including adding new interactive elements, and updating elements on the fly.

This library is designed to work with esp8266/esp32 chipset, but may be able to work with other wifi enabled microcontrollers compatible with the Arduino IDE.

[Video Demo](https://youtu.be/34rOOseLU-4)

## Installation
1. Install [Arduino IDE](https://www.arduino.cc/en/Main/Software)
2. Install [ESP8266 Library for arduino (tutorial link)](https://arduino-esp8266.readthedocs.io/en/latest/installing.html)
3. Download [arduinoWebSockets](https://github.com/Links2004/arduinoWebSockets) library
4. In Arduino IDE click **Sketch > Include Library > Add .ZIP Library...** and select the downloaded arduinoWebSockets zip file
5. Download this library
6. In Arduino IDE click **Sketch > Include Library > Add .ZIP Library...** and select the downloaded zip file

The library includes some example, but there is no documentation yet.