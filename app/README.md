# Mobile Application

## Overview

The Smart Home mobile application is built using Capacitor, which wraps the web dashboard into a native Android application. This provides a native app experience while leveraging the existing web codebase with mobile-optimized UI components.

## Technology Stack

### Framework
- **Capacitor 8.0.0** - Native runtime for web apps
- **Android Native** - Target platform

### Web Technologies
- HTML5, CSS3, JavaScript ES6
- Firebase JS SDK v10.7.1
- Paho MQTT v1.0.1
- Chart.js v3.9.1

### Build Tools
- Gradle 8.14.3
- Android SDK (API 36)
- Node.js and npm

## Project Structure

```
app/
├── capacitor.config.json    # Capacitor configuration
├── package.json             # Node.js dependencies
├── www/                     # Web assets (source)
│   ├── index.html           # Main dashboard
│   ├── login.html           # Authentication page
│   └── assets/              # CSS and JavaScript
│       ├── css/             # Mobile-optimized styles
│       └── js/              # JavaScript modules
└── android/                 # Android native project
    ├── app/                 # Main application module
    │   ├── build.gradle     # App build configuration
    │   └── src/             # Android source code
    ├── build.gradle         # Project build configuration
    ├── variables.gradle     # Version variables
    └── gradle/              # Gradle wrapper
```

## Configuration

### capacitor.config.json
```json
{
  "appId": "com.smarthome.app",
  "appName": "SMART HOME",
  "webDir": "www",
  "plugins": {
    "StatusBar": {
      "overlaysWebView": true,
      "style": "DARK",
      "backgroundColor": "#1e293b"
    }
  },
  "android": {
    "allowMixedContent": true
  }
}
```

**Key Settings:**
- `appId` - Android package name
- `appName` - Application display name
- `webDir` - Web assets directory
- `allowMixedContent` - Allow HTTP connections (for MQTT)
- `StatusBar` - Dark theme status bar

### Android Configuration

**variables.gradle:**
```groovy
minSdkVersion = 24        // Android 7.0+
compileSdkVersion = 36    // Latest SDK
targetSdkVersion = 36     // Target API
```

**build.gradle (app):**
- Application ID: `com.smarthome.app`
- Version: 1.0
- ProGuard disabled for debug builds

## Dependencies

### Node.js (package.json)
```json
{
  "@capacitor/android": "^8.0.0",
  "@capacitor/cli": "^8.0.0",
  "@capacitor/core": "^8.0.0"
}
```

### Android (build.gradle)
- AndroidX AppCompat
- AndroidX CoordinatorLayout
- Core SplashScreen
- Capacitor Android runtime

## Features

### Native App Experience
- Android native wrapper
- Status bar integration (dark theme)
- Splash screen support
- Native navigation gestures

### Mobile-Optimized UI
- Bottom navigation bar (3 tabs)
- Touch-friendly controls
- Single-column layouts
- Full-screen modals

### Smart Home Functions
- Real-time device monitoring
- Sensor data display (temperature, humidity, light)
- Device control (fan, lamp, pump)
- Interactive charts
- WiFi setup guide
- Time synchronization

### Communication
- MQTT over WebSocket (real-time)
- Firebase Realtime Database (data sync)
- Firebase Authentication (user login)

## Build Instructions

### Prerequisites
1. Node.js 18+ and npm
2. Android Studio
3. Android SDK (API 36)
4. Java Development Kit (JDK 17)

### Setup
```bash
# Install Node.js dependencies
cd app
npm install

# Sync web assets to Android
npx cap sync android
```

### Development Build
```bash
# Open in Android Studio
npx cap open android

# Or build from command line
cd android
./gradlew assembleDebug
```

### Release Build
```bash
cd android
./gradlew assembleRelease
```

### APK Locations
- Debug: `android/app/build/outputs/apk/debug/app-debug.apk`
- Release: `android/app/build/outputs/apk/release/app-release.apk`

## Development Workflow

### Web Development
1. Edit files in `www/` directory
2. Test in browser: `cd www && python -m http.server 8000`
3. Sync to Android: `npx cap sync android`
4. Build and test on device

### Android Development
1. Open project: `npx cap open android`
2. Make native changes in Android Studio
3. Build and run on device/emulator

### Hot Reload (Development)
```bash
# Start live reload server
npx cap run android --livereload --external
```

## App Architecture

### Entry Point
```
Android App Launch
    ↓
MainActivity (Capacitor)
    ↓
WebView loads www/index.html
    ↓
main.js initializes application
    ↓
Authentication check
    ↓
MQTT + Firebase initialization
    ↓
Device grid rendering
```

### Data Flow
```
ESP Device → MQTT Broker → WebSocket → Web App
                                          ↓
                                    Firebase Sync
                                          ↓
                                    UI Update
```

## Minimum Requirements

### Android Device
- Android 7.0 (API 24) or higher
- Internet connection (WiFi or mobile data)
- WebView with ES6 support

### Permissions
- Internet access (for MQTT and Firebase)
- Network state (for connection monitoring)

## Differences from Web Version

| Feature | Web Dashboard | Mobile App |
|---------|---------------|------------|
| Navigation | Sidebar | Bottom nav |
| Data Export | Excel export | Not included |
| Configuration | Full settings | Simplified |
| Layout | Multi-column | Single column |
| Device Detail | Tab view | Modal popup |
| WiFi Setup | Not available | Included |
| Manual Time | Not available | Included |

## File Structure Details

### www/ Directory
Contains all web assets loaded by the native WebView:
- [www/README.md](www/README.md) - Web assets overview
- [www/assets/css/README.md](www/assets/css/README.md) - CSS documentation
- [www/assets/js/README.md](www/assets/js/README.md) - JavaScript documentation

### android/ Directory
Native Android project generated by Capacitor:
- `app/` - Main application module
- `capacitor-cordova-android-plugins/` - Plugin support
- `gradle/` - Build system

## Troubleshooting

### Build Errors
```bash
# Clean and rebuild
cd android
./gradlew clean
./gradlew assembleDebug
```

### Sync Issues
```bash
# Force sync
npx cap sync android --force
```

### WebView Debugging
1. Enable USB debugging on device
2. Open Chrome on computer
3. Navigate to `chrome://inspect`
4. Select device and inspect WebView

### Common Issues

**MQTT Connection Failed:**
- Check `allowMixedContent: true` in capacitor.config.json
- Verify broker hostname and port
- Ensure device has internet access

**Firebase Error:**
- Verify Firebase configuration
- Check API key validity
- Ensure Authentication is enabled

**White Screen:**
- Check console for JavaScript errors
- Verify all dependencies loaded
- Clear app cache and reinstall

## Security Considerations

- HTTPS recommended for production
- Firebase security rules enforced
- MQTT credentials in localStorage
- No sensitive data in APK
- ProGuard enabled for release builds

## Maintenance

### Updating Dependencies
```bash
npm update
npx cap sync android
```

### Updating Capacitor
```bash
npm install @capacitor/core@latest @capacitor/cli@latest @capacitor/android@latest
npx cap sync android
```

### Updating Android SDK
1. Open Android Studio
2. SDK Manager > Update SDK tools
3. Rebuild project

## Authors

- Le Quang Minh Nhat
- Thai Huu Loi
- Tran Huu Dao

## License

This application is part of the SMART_HOME IoT project.
