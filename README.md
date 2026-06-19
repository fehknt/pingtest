# ESP32 Network Connectivity Monitor

An ESP32-based network monitor that sequentially pings your router, modem, ISP, and the public internet to determine exactly where a network failure has occurred. It displays the current status and outage durations on two SSD1306 OLED displays.

## Features
- **Multi-stage Ping Testing:** Automatically checks the connection chain to find the point of failure:
  - Local Router
  - Modem
  - ISP Gateway
  - Internet (e.g., Google DNS)
- **Dual OLED Display:** Uses two OLED displays (one wide, one narrow) to show current time and network status.
- **Outage Tracking:** Tracks the duration of the outage in seconds and displays which component is failing.
- **NTP Time Sync:** Automatically synchronizes time via WiFi and handles Daylight Saving Time (DST) automatically using POSIX timezone rules.

## Hardware Requirements
- ESP32-S3 DevKitC-1 (or compatible ESP32 board)
- 2x SSD1306 I2C OLED Displays

## Software Dependencies
The project is built using [PlatformIO](https://platformio.org/).
Libraries used:
- `marian-craciunescu/ESP32Ping@^1.7`
- `adafruit/Adafruit SSD1306@^2.5.13`
- `adafruit/Adafruit GFX Library@^1.11.11`
- `arduino-libraries/NTPClient@^3.2.1`

## Setup Instructions

1. **Clone the repository:**
   ```bash
   git clone https://github.com/yourusername/pingtest.git
   ```

2. **Configure Secrets:**
   Copy the `src/secrets.example.h` to `src/secrets.h`:
   ```bash
   cp src/secrets.example.h src/secrets.h
   ```
   Open `src/secrets.h` and fill in your network credentials and target IPs.

3. **Build and Upload:**
   Open the project in VS Code with the PlatformIO extension installed, or run:
   ```bash
   pio run -t upload
   ```

## License
MIT License
