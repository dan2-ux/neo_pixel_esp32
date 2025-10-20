# IoT Smart Lighting System – NeoPixel Control via Web Interface

This repository contains an IoT-based smart lighting project that allows users to control a NeoPixel LED strip through an interactive web interface. Designed as a home decoration system, the project combines creativity with technology, enabling users to personalize their lighting experience with just a few clicks.

At its core, the system uses an ESP32 microcontroller that communicates seamlessly with a React.js web application via a Go (Golang) backend server. The ESP32 acts as the main controller, receiving real-time commands from the server to adjust the NeoPixel’s color, brightness, and animation effects according to the user’s preferences.

This project demonstrates the power of IoT integration — connecting hardware and web technologies to create a responsive, customizable, and visually appealing lighting experience.

## Workflow
- The React.js web interface serves as the user control panel, allowing users to choose lighting effects, colors, or brightness levels.

- When a user makes a change, the web app sends a PATCH request to the Go server, which updates the relevant data in the database.

- The ESP32, connected to both the Wi-Fi network and the server, continuously checks for updates.

- Once a new command is detected, the ESP32 retrieves the latest data and instantly updates the NeoPixel LED strip to reflect the user’s chosen settings.

- An LCD display attached to the ESP32 provides a live visual status, showing information such as the selected mode or brightness level.

This workflow ensures real-time interaction and smooth synchronization between the user interface, database, and physical device.

<img width="1917" height="967" alt="image" src="https://github.com/user-attachments/assets/e53f1eec-8958-4b72-8a2c-75eb603ef415" />


## Software Requirements

- Go (Golang): Handles backend logic and server-side communication.

- React.js: Provides a modern, responsive, and user-friendly web interface.

- Arduino IDE: Used for programming and uploading code to the ESP32.

- Visual Studio Code (VSCode): Recommended for editing and managing the codebase.

## Hardware Requirements

- ESP32 microcontroller: The main IoT device responsible for communication and control.

- LCD display: Used to display system information and lighting status.

- NeoPixel WS2812 LED strip: A programmable RGB LED strip for visual lighting effects.

## Guidance for running this repository

Disclamer: This repository includes go server, frontend and esp32 controller file.

### First: running the Go server
To run Go server, you need to go to google and download Golang first. After, finished download Go, you should create a folder.
<pre>
  mkdir myproject
  cd myproject
</pre>

Then, create you Go mod.
<pre>
  go mod init <module-name> 
</pre>
To run you Go file, execute the command below:
<pre>
  go run <your-go-file>
</pre>

### Second: running the frontend
First, you need to clone this repository into a folder. Before go any further make sure you have node installed, if not react js doesn't work.
Run **npm run dev** to start frontend when everything is installed.

### Finally: run the esp32 controll file
Open the ArduinoIDE, on the left hand side select the library icon. Then you should find and install **Adafruit_NeoPixel** and **LiquidCrystal_I2C** libraries
Then execute the ino file.
