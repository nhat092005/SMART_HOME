# MQTT Manager Component

## Overview

MQTT Manager handles MQTT communication for ESP32 smart home devices with dynamic topic routing and command callback support.

## Features

- ✅ Dynamic device ID from Kconfig
- ✅ Auto-generated topics: `smartHome/{device_id}/data|command|response`
- ✅ JSON-based messaging using `json_helper`
- ✅ Command callback pattern for flexible control
- ✅ Connection state management
- ✅ QoS configuration support

## Topics

### Published (ESP32 → Server)

#### `smartHome/{device_id}/data`
Sensor data published at configured interval (default 30s)
```json
{
  "timestamp": 1701388800,
  "temperature": 25.6,
  "humidity": 65.2,
  "light": 450,
  "interval": 30
}
```

#### `smartHome/{device_id}/response`
Command execution response
```json
{
  "timestamp": 1701388801,
  "cmd": "set_led",
  "status": "success",
  "message": "LED turned on"
}
```

### Subscribed (Server → ESP32)

#### `smartHome/{device_id}/command`
Remote commands with parameters
```json
{
  "timestamp": 1701388800,
  "cmd": "set_led",
  "params": {
    "state": true,
    "brightness": 80
  }
}
```

## Configuration

Configure via `idf.py menuconfig` → **MQTT Manager Configuration**

- **Device ID**: Unique identifier (e.g., `esp_01`, `esp_living_room`)
- **Broker URI**: MQTT broker address
- **Username/Password**: Optional credentials
- **Base Topic**: Default `smartHome`
- **Data Interval**: Publish frequency (5-3600 seconds)

## Usage Example

```c
#include "mqtt_manager.h"
#include "json_helper.h"
#include <time.h>

// Command handler callback
void my_command_handler(const char *cmd, cJSON *params)
{
    if (strcmp(cmd, "set_led") == 0) {
        bool state = json_helper_get_bool(params, "state", false);
        status_led_set_state(state);
        
        // Send response
        mqtt_publish_response(time(NULL), cmd, true, "LED state changed");
        
    } else if (strcmp(cmd, "reboot") == 0) {
        mqtt_publish_response(time(NULL), cmd, true, "Rebooting...");
        vTaskDelay(pdMS_TO_TICKS(1000));
        esp_restart();
    }
}

void app_main(void)
{
    // Initialize WiFi first
    wifi_manager_init();
    wifi_manager_connect();
    
    // Wait for WiFi connection
    while (!wifi_manager_is_connected()) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    
    // Initialize MQTT
    mqtt_manager_init();
    
    // Register command callback
    mqtt_register_command_callback(my_command_handler);
    
    // Start MQTT
    mqtt_manager_start();
    
    // Publish sensor data
    while (1) {
        if (mqtt_is_connected()) {
            float temp = 25.6;    // Read from sensor
            float hum = 65.2;     // Read from sensor
            int light = 450;      // Read from sensor
            
            mqtt_publish_sensor_data(time(NULL), temp, hum, light, 30);
        }
        
        vTaskDelay(pdMS_TO_TICKS(30000));  // 30 seconds
    }
}
```

## API Reference

### Connection Management

- `mqtt_manager_init()` - Initialize MQTT client
- `mqtt_manager_start()` - Connect to broker
- `mqtt_manager_stop()` - Disconnect
- `mqtt_is_connected()` - Check connection status

### Publishing

- `mqtt_publish_sensor_data(timestamp, temp, hum, light, interval)` - Publish sensor data
- `mqtt_publish_response(timestamp, cmd, success, message)` - Publish command response

### Command Handling

- `mqtt_register_command_callback(callback)` - Register command handler

## Dependencies

- `mqtt` - ESP-IDF MQTT client
- `json` - cJSON library
- `json_helper` - Custom JSON utilities
- `esp_wifi` - WiFi functionality
- `esp_netif` - Network interface

## Notes

- Ensure WiFi is connected before starting MQTT
- Command callback is called from MQTT event handler task
- All JSON strings are freed automatically
- QoS 0 for data (fire and forget)
- QoS 1 for responses (at least once)
