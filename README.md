
## Usage

To use this project, follow these steps:

1. Set up the hardware and connections as per the project requirements.
2. Open `main.ino` in the Arduino IDE.
3. Install the necessary libraries (`arduinoFFT`, `WiFi`, `WebServer`) if not already installed.
4. Configure the SSID and password for your WiFi network in the `main.ino` file.
5. Compile and upload the code to your ESP32 board.
6. Access the web server hosted by the ESP32 by connecting to its IP address using a web browser.
7. The web page will display the FFT analysis results, indicating the presence of a bell, fire alarm, or both.

## Dependencies

This project depends on the following libraries:

- `arduinoFFT`: Library for FFT calculation.
- `WiFi`: Library for WiFi connectivity.
- `WebServer`: Library for creating a web server.

Make sure to install these libraries before compiling and running the code.