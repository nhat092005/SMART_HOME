# SMART HOME IoT System

## Overview

A complete IoT-based Smart Home system featuring ESP32 firmware, web dashboard, Android mobile application, and custom PCB design. The system enables real-time monitoring of environmental sensors and remote control of home appliances through MQTT communication and Firebase cloud services.

## System Architecture

```
+------------------+     +------------------+     +------------------+
|   ESP32 Device   |---->|   MQTT Broker    |<----|   Web Dashboard  |
|   (Sensors/IO)   |     |   (HiveMQ Cloud) |     |   (Firebase)     |
+------------------+     +------------------+     +------------------+
                                  ^                        ^
                                  |                        |
                         +--------+--------+               |
                         |                                 |
                  +------+-------+                +--------+--------+
                  | Mobile App   |--------------->|    Firebase     |
                  | (Android)    |                | Realtime DB     |
                  +--------------+                +-----------------+
```

## Features

### Environmental Monitoring
- Temperature and humidity sensing (SHT3x)
- Light intensity measurement (BH1750)
- Real-time clock with battery backup (DS3231)
- OLED display for local status

### Device Control
- Fan control via relay
- Light control via relay
- AC control via relay
- Physical button interface
- Remote MQTT commands

### Communication
- WiFi with captive portal provisioning
- MQTT over SSL/TLS (HiveMQ Cloud)
- Firebase Realtime Database synchronization
- WebSocket for real-time updates

### User Interfaces
- Web dashboard (responsive design)
- Android mobile application (Capacitor)
- OLED display on device
- LED status indicators

## Project Structure

```
SMART_HOME/
├── esp/                    # ESP32 firmware (ESP-IDF)
│   ├── main/               # Application entry point
│   └── components/         # Modular components
│       ├── application/    # Business logic
│       ├── communication/  # WiFi, MQTT, WebServer
│       ├── hardware/       # Buttons, LEDs, Relays
│       ├── sensor/         # Sensor drivers
│       └── utilities/      # Helper modules
├── web/                    # Web dashboard
│   ├── index.html          # Main dashboard
│   ├── login.html          # Authentication
│   └── assets/             # CSS and JavaScript
│       ├── css/            # Modular stylesheets
│       └── js/             # ES6 modules
├── app/                    # Mobile application
│   ├── www/                # Web assets (Capacitor)
│   └── android/            # Android native project
├── pcb/                    # PCB design files
│   └── PCB_Project/        # Altium Designer project
├── broker/                 # MQTT broker configuration
├── documents/              # Project documentation
│   └── MQTT_COMMANDS.md    # MQTT command reference
└── README.md               # This file
```

## Hardware Components

| Component | Model | Function |
|-----------|-------|----------|
| MCU | ESP32 | Main controller |
| Temp/Humidity | SHT3x | Environmental sensing |
| Light Sensor | BH1750 | Ambient light measurement |
| RTC | DS3231 | Real-time clock |
| Display | SH1106 OLED | Status display |
| Relays | 3x Relay Module | Device control |
| Buttons | 5x Tactile | User input |
| LEDs | 3x Status LED | Visual indicators |

## Technology Stack

### Embedded
- ESP-IDF v5.x (FreeRTOS)
- C language
- MQTT over SSL
- I2C sensor interface

### Web Frontend
- HTML5, CSS3, JavaScript ES6
- Firebase Authentication
- Firebase Realtime Database
- Paho MQTT (WebSocket)
- Chart.js

### Mobile Application
- Capacitor 8.0.0
- Android SDK (API 36)
- WebView-based hybrid app

### Cloud Services
- HiveMQ Cloud (MQTT Broker)
- Firebase (Auth + Realtime DB)

### PCB Design
- Altium Designer
- Custom ESP32 carrier board

## Quick Start

### 1. ESP32 Firmware

```bash
cd esp
idf.py set-target esp32
idf.py menuconfig      # Configure settings
idf.py build
idf.py -p PORT flash monitor
```

### 2. Web Dashboard

```bash
cd web
# Serve with any HTTP server
python -m http.server 8000
# Open http://localhost:8000
```

### 3. Mobile Application

```bash
cd app
npm install
npx cap sync android
npx cap open android   # Build in Android Studio
```

## MQTT Topics

| Topic | Direction | Description |
|-------|-----------|-------------|
| SmartHome/{id}/data | ESP -> Cloud | Sensor readings |
| SmartHome/{id}/state | ESP -> Cloud | Device states |
| SmartHome/{id}/info | ESP -> Cloud | Device information |
| SmartHome/{id}/command | Cloud -> ESP | Control commands |

## MQTT Commands

### Control Device
```json
{
  "id": "cmd_001",
  "command": "set_device",
  "params": {
    "device": "fan",
    "state": 1
  }
}
```

### Set Mode
```json
{
  "id": "cmd_002",
  "command": "set_mode",
  "params": {
    "mode": 1
  }
}
```

### Sync Time
```json
{
  "id": "cmd_003",
  "command": "sync_time",
  "params": {
    "timestamp": 1735545600
  }
}
```

See [documents/MQTT_COMMANDS.md](documents/MQTT_COMMANDS.md) for complete command reference.

## Firebase Database Structure

```
{
  "devices": {
    "esp_01": {
      "name": "Living Room",
      "sensors": {
        "temperature": 25.5,
        "humidity": 60.2,
        "light": 450
      },
      "state": {
        "fan": true,
        "light": false,
        "ac": false
      }
    }
  },
  "history": {
    "esp_01": {
      "records": { ... }
    }
  }
}
```

## Configuration

### ESP32 (menuconfig)
- WiFi credentials and AP settings
- MQTT broker URI and credentials
- GPIO pin assignments
- Sensor intervals

### Web/App
- Firebase configuration (localStorage)
- MQTT broker settings (localStorage)

## Documentation

| Directory | Documentation |
|-----------|---------------|
| esp/ | [ESP32 Firmware](esp/README.md) |
| web/ | [Web Dashboard](web/README.md) |
| app/ | [Mobile Application](app/README.md) |
| pcb/ | [PCB Design](pcb/README.md) |
| documents/ | [MQTT Commands](documents/MQTT_COMMANDS.md) |

### Component Documentation

**ESP32 Components:**
- [Application Layer](esp/components/application/README.md)
- [Communication Layer](esp/components/communication/README.md)
- [Hardware Layer](esp/components/hardware/README.md)
- [Sensor Drivers](esp/components/sensor/README.md)
- [Utilities](esp/components/utilities/README.md)

**Web Assets:**
- [CSS Architecture](web/assets/css/README.md)
- [JavaScript Modules](web/assets/js/README.md)

**Mobile Assets:**
- [Mobile CSS](app/www/assets/css/README.md)
- [Mobile JavaScript](app/www/assets/js/README.md)

## Development Tools

| Tool | Purpose |
|------|---------|
| ESP-IDF | ESP32 development framework |
| VS Code | IDE with ESP-IDF extension |
| Android Studio | Mobile app development |
| Altium Designer | PCB design |
| MQTT Explorer | MQTT debugging |
| Firebase Console | Cloud management |

## Requirements

### Software
- ESP-IDF v5.x
- Node.js 18+
- Android Studio (for mobile)
- Python 3.8+

### Hardware
- ESP32 development board
- Custom PCB or breadboard setup
- SHT3x, BH1750, DS3231 sensors
- SH1106 OLED display
- Relay modules
- Power supply (5V/3.3V)

## Authors

- Le Quang Minh Nhat
- Thai Huu Loi
- Tran Huu Dao

## License

This project is licensed under the MIT License - see [LICENSE.md](LICENSE.md) for details.

## Acknowledgments

- Espressif Systems for ESP-IDF
- Firebase for cloud services
- HiveMQ for MQTT broker
- Open source sensor libraries
