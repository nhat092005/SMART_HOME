# JavaScript Assets - Mobile App

## Overview

This directory contains all JavaScript modules for the Smart Home mobile application. The codebase follows ES6 module architecture optimized for mobile use cases with simplified navigation, device control modals, and mobile-specific features like WiFi setup guide.

## Structure

```
js/
├── main.js                # Application entry point
├── core/                  # Core functionality
│   ├── firebase-config.js # Firebase initialization
│   ├── auth.js            # Authentication logic
│   └── login.js           # Login page controller
├── mqtt/                  # MQTT communication
│   ├── mqtt-client.js     # MQTT client management
│   ├── mqtt-handlers.js   # Message handling
│   └── mqtt-to-firebase.js # Data synchronization
├── devices/               # Device management
│   ├── device-manager.js  # Device CRUD operations
│   └── device-card.js     # Device UI rendering
├── charts/                # Data visualization
│   ├── chart-manager.js   # Chart.js integration
│   └── chart-config.js    # Chart configuration
├── settings/              # Settings management
│   └── settings-manager.js # Device settings and WiFi guide
└── ui/                    # UI utilities
    └── ui-helpers.js      # UI helper functions
```

## Module Documentation

### main.js - Entry Point

**Purpose:** Coordinates all modules and initializes the mobile application

**Key Functions:**
- `initializeApp()` - Application startup
- `subscribeToDeviceOnce(deviceId)` - MQTT subscription management
- `switchTab(tabName)` - Tab navigation (dashboard, report, settings)
- `openReportDetail(deviceId)` - Opens device detail modal
- `closeReportDetail()` - Closes device detail modal
- `toggleFeature(feature)` - Controls device features (fan, lamp, pump)
- `selectChartType(type)` - Switches chart type in detail view

**Global Exports:**
- `switchTab`, `logout` - Navigation functions
- `openReportDetail`, `closeReportDetail` - Modal management
- `toggleFeature`, `selectChartType` - Device controls
- Settings functions for WiFi and time configuration

### Core Modules

#### core/firebase-config.js
**Purpose:** Firebase configuration and initialization

**Exports:**
- `app` - Firebase app instance
- `auth` - Authentication instance
- `db` - Realtime Database instance

**Features:**
- User-configurable Firebase settings
- Config stored in localStorage
- Default demo configuration

#### core/auth.js
**Purpose:** User authentication

**Key Functions:**
- `requireAuth()` - Checks authentication, redirects if needed
- `logout()` - Signs out user
- `getCurrentUser()` - Returns current user

#### core/login.js
**Purpose:** Login page controller

**Features:**
- Email/password login
- User registration
- Error handling
- Redirect on success

### MQTT Modules

#### mqtt/mqtt-client.js
**Purpose:** MQTT client management

**Key Functions:**
- `initializeMQTTClient(callbacks)` - Initialize MQTT connection
- `subscribeToDevice(deviceId)` - Subscribe to device topics
- `sendMQTTCommand(deviceId, command, value)` - Send commands
- `isMQTTConnected()` - Check connection status

**Topics:**
- `data/{deviceId}` - Sensor data
- `state/{deviceId}` - Device states
- `info/{deviceId}` - Device info
- `command/{deviceId}` - Commands

#### mqtt/mqtt-handlers.js
**Purpose:** MQTT message handling

**Key Functions:**
- `handleMQTTMessage(topic, message)` - Process incoming messages
- `handleMQTTConnect()` - Handle connection
- `handleMQTTConnectionLost()` - Handle disconnection
- `getMQTTCachedState(deviceId)` - Get cached state

#### mqtt/mqtt-to-firebase.js
**Purpose:** Sync MQTT data to Firebase

**Key Functions:**
- `saveSensorDataToFirebase(deviceId, data)` - Save sensor readings
- `saveDeviceStateToFirebase(deviceId, state)` - Save device state
- `saveDeviceInfoToFirebase(deviceId, info)` - Save device info

### Device Modules

#### devices/device-manager.js
**Purpose:** Device CRUD operations

**Key Functions:**
- `initializeDeviceManager(viewType)` - Initialize manager
- `addDevice(deviceId, name)` - Create device
- `updateDevice(deviceId, data)` - Update device
- `deleteDevice(deviceId)` - Remove device
- `getAllDevices()` - Get all devices
- `setViewType(type)` - Set view mode

#### devices/device-card.js
**Purpose:** Device UI rendering

**Key Functions:**
- `renderDeviceGrid(devices, viewType)` - Render device grid
- `createDeviceCard(device, viewType)` - Create device card
- `getAllDevicesData()` - Get all device data
- `getDeviceData(deviceId)` - Get specific device data

**Features:**
- Compact mobile cards
- Sensor value display
- Toggle controls
- Touch interactions

### Chart Modules

#### charts/chart-manager.js
**Purpose:** Chart.js integration for detail modal

**Key Functions:**
- `initializeChart(deviceId, chartType)` - Create chart
- `switchChartType(chartType)` - Change chart type
- `cleanupChart()` - Destroy chart instance

**Chart Types:**
- `temp` - Temperature chart (orange)
- `humid` - Humidity chart (blue)
- `light` - Light chart (yellow)

#### charts/chart-config.js
**Purpose:** Chart configuration

**Exports:**
- `CHART_CONFIG` - Chart settings per sensor type
- `CHART_COLORS` - Color definitions

### Settings Module

#### settings/settings-manager.js
**Purpose:** Device settings and WiFi configuration

**Key Functions:**
- `initializeSettings()` - Initialize settings page
- `loadDeviceInfoTable()` - Load device table
- `rebootDevice(deviceId)` - Send reboot command
- `syncTimeToAllDevices()` - Sync NTP time
- `syncTimestampToDevices(timestamp)` - Sync manual time
- `showWiFiGuideForDevice(deviceId)` - Show WiFi setup guide
- `closeWiFiGuideModal()` - Close WiFi modal
- `showTimeModalForDevice(deviceId)` - Show time config modal
- `applyManualTime()` - Apply manual time
- `closeManualTimeModal()` - Close time modal
- `updateManualTimePreview()` - Update time preview

**WiFi Setup Guide:**
- ESP32 AP credentials display (IP: 192.168.4.1)
- Step-by-step instructions
- Connection guide

**Time Configuration:**
- NTP sync option
- Manual time setting
- Time preview

### UI Module

#### ui/ui-helpers.js
**Purpose:** UI utility functions

**Key Functions:**
- `updateStatusBadge(badgeId, status, text)` - Update status
- `switchTab(tabName)` - Tab navigation
- `showModal(modalId)` - Open modal
- `hideModal(modalId)` - Close modal
- `initializeSidebar()` - Initialize UI
- `startClock()` - Start clock display
- `stopClock()` - Stop clock
- `updateClock()` - Update clock display

## Data Flow

### Device Control (Mobile)
```
User Tap → toggleFeature() → sendMQTTCommand() → MQTT Broker → ESP Device
                                                                    ↓
UI Update ← Firebase Listener ← saveSensorDataToFirebase() ← MQTT Message
```

### Device Detail Modal
```
User taps device card → openReportDetail(deviceId)
    ↓
Load device data → Update stats display (temp/humid/light)
    ↓
Initialize chart → Start Firebase listener
    ↓
User interaction → toggleFeature() or selectChartType()
    ↓
closeReportDetail() → cleanupChart()
```

### WiFi Setup Flow
```
Settings → showWiFiGuideForDevice(deviceId)
    ↓
Display AP credentials (SSID: SmartHome, Pass: 12345678)
    ↓
Guide user to connect phone to ESP32 AP
    ↓
Configure device WiFi via 192.168.4.1
```

## Differences from Web Version

| Feature | Web | Mobile App |
|---------|-----|------------|
| Data Export | Full module | Not included |
| Firebase Config | Settings page | Not included |
| MQTT Config | Settings page | Not included |
| Navigation | Sidebar tabs | Bottom nav |
| Device Detail | Tab view | Modal popup |
| WiFi Guide | Not included | Included |
| Time Modal | Not included | Included |

## Dependencies

### External Libraries
- Firebase JS SDK v10.7.1 (app, auth, database)
- Paho MQTT v1.0.1 (WebSocket MQTT client)
- Chart.js v3.9.1 (data visualization)
- Font Awesome 6.4.0 (icons)

### Load Order
1. Paho MQTT (deferred)
2. Chart.js (deferred)
3. main.js (ES6 module)

## Configuration

### MQTT Config
Stored in localStorage key `mqtt_config`:
```json
{
  "host": "broker.emqx.io",
  "port": 8083,
  "path": "/mqtt",
  "useSSL": false
}
```

### Firebase Config
Stored in localStorage key `user_firebase_config`:
```json
{
  "apiKey": "...",
  "databaseURL": "...",
  "projectId": "..."
}
```

## Error Handling

All modules implement consistent error handling:
```javascript
try {
    // Operation
} catch (error) {
    console.error('[ModuleName] Error:', error.message);
}
```

## Usage

Main dashboard:
```html
<script type="module" src="assets/js/main.js"></script>
```

Login page:
```html
<script type="module" src="assets/js/core/login.js"></script>
```

## Maintenance

When extending functionality:
1. Create module in appropriate directory
2. Export functions explicitly
3. Import in main.js if needed
4. Follow error handling patterns
5. Test on mobile devices
6. Update this documentation
