# ESP8266-WebServer
Serve websites using an ESP8266.
This project specifically uses a Wemos D1 Mini but should be usable with any ESP8266 microcontroller.

## Requirements
This project requires VS Code with the PlatformIO extensions. See https://platformio.org/ for details.

## Setup
Under `main.cpp` set the SSID and password to your WiFi network's name and password.

## Deploy
- Plug in your ESP8266
- Navigate to PIO on the left hand side bar then select `Upload Filesystem Image` from Project Tasks. This will upload the content of the "data" folder to your device.
- Once that completes select `Upload and Monitor`. This will compile and upload the source code under "src" folder. Once it completes it will reset the device and connect the serial monitor so you can see the device connecting to your wireless network.

## Test
The serial monitor above will display the IP address of the device once it has connected. Open a browser window and navigate to that IP address to see the webpage.
