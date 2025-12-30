# Web Assets - Mobile App

## Overview

This directory contains the web assets (HTML, CSS, JavaScript) for the Smart Home Capacitor mobile application. These files are served by the native Android WebView and provide the user interface for the mobile app. The codebase is optimized for mobile-first design with bottom navigation, touch interactions, and compact layouts.

## Directory Structure

```
www/
├── index.html             # Main dashboard page
├── login.html             # Authentication page
├── assets/                # Static assets
│   ├── css/               # Stylesheets (mobile-optimized)
│   │   ├── base/          # Variables, reset, typography
│   │   ├── layouts/       # Bottom nav, main content, responsive
│   │   ├── components/    # Buttons, cards, badges, modals
│   │   ├── views/         # Login, dashboard, settings styles
│   │   └── style.css      # Main stylesheet
│   └── js/                # JavaScript modules
│       ├── core/          # Firebase, auth, login
│       ├── mqtt/          # MQTT client and handlers
│       ├── devices/       # Device management
│       ├── charts/        # Chart.js integration
│       ├── settings/      # Settings and WiFi guide
│       ├── ui/            # UI utilities
│       └── main.js        # Entry point
├── assets/css/README.md   # CSS documentation
└── assets/js/README.md    # JavaScript documentation
```

## Pages

### index.html - Main Dashboard

Mobile-optimized dashboard with bottom navigation:

**Home Tab (Dashboard)**
- Device grid with sensor cards
- Real-time temperature, humidity, light readings
- Device detail modal with charts
- Quick controls (fan, lamp, pump toggles)

**Dashboard Tab (Report)**
- Chart view via device detail modal
- Sensor type selection (temp/humid/light)
- Historical data visualization

**Settings Tab**
- System clock display
- Device information table
- Device actions:
  - Reboot device
  - Sync NTP time
  - Manual time configuration
  - WiFi setup guide

### login.html - Authentication

Simple mobile login page:
- Email and password inputs
- Sign in / Sign up toggle
- Error message display
- Redirect to dashboard on success

## Key Features

### Mobile-Optimized UI
- Bottom navigation bar (3 tabs)
- Icon-only navigation for space efficiency
- Full-width single-column layouts
- Touch-friendly controls (min 44px tap targets)
- Safe area insets for notched devices

### Device Detail Modal
- Full sensor data display (temperature, humidity, light)
- Quick control toggles (fan, lamp, pump)
- Interactive Chart.js charts
- Chart type switching
- Compact mobile layout

### WiFi Setup Guide
- ESP32 Access Point credentials
- Step-by-step connection instructions
- Device IP address (192.168.4.1)
- Network configuration guide

### Time Configuration
- NTP time synchronization
- Manual time setting
- Date and time pickers
- Time preview before apply

## Architecture

### Application Flow
```
Entry (index.html)
    ↓
Load main.js module
    ↓
Authentication check (requireAuth)
    ↓
Initialize Firebase
    ↓
Initialize MQTT client
    ↓
Initialize device manager
    ↓
Render device grid
    ↓
Start real-time listeners
```

### Navigation Flow
```
Bottom Navigation:
├── Home (dashboard) → Device grid → Device detail modal
├── Dashboard (report) → Device detail modal with charts
└── Settings → Clock, device table, actions
```

### Device Control Flow
```
Device Card Tap → openReportDetail(deviceId)
    ↓
Display sensor stats + controls
    ↓
Toggle switch → toggleFeature(feature)
    ↓
sendMQTTCommand() → MQTT Broker → ESP Device
    ↓
State update → Firebase sync → UI update
```

## Differences from Web Version

| Feature | Web | Mobile App |
|---------|-----|------------|
| Navigation | Sidebar | Bottom nav (3 tabs) |
| Layout | Multi-column | Single column |
| Data Export | Full table + Excel | Not included |
| Firebase Config | Settings panel | Not included |
| MQTT Config | Settings panel | Not included |
| Device Detail | Tab view | Modal popup |
| WiFi Guide | Not available | Included |
| Manual Time | Not available | Included |
| Chart View | Dedicated tab | Inside modal |

## External Dependencies

### CDN Libraries
- Firebase JS SDK v10.7.1
- Paho MQTT v1.0.1
- Chart.js v3.9.1
- Font Awesome 6.4.0
- Google Fonts (Inter)

### Load Method
```html
<!-- Fonts -->
<link href="https://fonts.googleapis.com/css2?family=Inter..." rel="stylesheet" />
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.4.0/css/all.min.css" />

<!-- Libraries -->
<script defer src="https://cdnjs.cloudflare.com/ajax/libs/paho-mqtt/1.0.1/mqttws31.min.js"></script>
<script defer src="https://cdnjs.cloudflare.com/ajax/libs/Chart.js/3.9.1/chart.min.js"></script>
```

## Configuration

### Viewport Meta
```html
<meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover, user-scalable=no" />
```

### Theme Color
```html
<meta name="theme-color" content="#2563eb" />
```

### MQTT Settings
Pre-configured in localStorage or defaults:
- Host: broker.emqx.io
- Port: 8083
- Path: /mqtt

### Firebase Settings
Pre-configured in localStorage or defaults from firebase-config.js

## Capacitor Integration

These web assets are loaded by Capacitor in the native Android WebView:

- `webDir: "www"` in capacitor.config.json
- Assets copied to Android assets folder during build
- WebView renders HTML/CSS/JS as native app
- Mixed content allowed for HTTP connections

## Development

### Local Testing
Files can be tested in browser before building:
```bash
cd www
python -m http.server 8000
```

### Building for Android
```bash
cd ..
npx cap sync android
npx cap open android
```

## File Organization

### CSS Modules
```
css/
├── base/          # Foundation (variables, reset, typography)
├── layouts/       # Structure (bottom-nav, main-content, responsive)
├── components/    # UI elements (buttons, cards, modals)
└── views/         # Page styles (login, dashboard, settings)
```

### JavaScript Modules
```
js/
├── core/          # Firebase, auth
├── mqtt/          # MQTT communication
├── devices/       # Device management
├── charts/        # Data visualization
├── settings/      # Settings management
└── ui/            # UI utilities
```

## Detailed Documentation

- [CSS Documentation](assets/css/README.md) - Stylesheet architecture
- [JavaScript Documentation](assets/js/README.md) - Module documentation

## Maintenance

When modifying web assets:
1. Make changes in www/ directory
2. Test in browser first
3. Run `npx cap sync android` to copy to Android
4. Build and test on device/emulator
5. Update documentation if needed
