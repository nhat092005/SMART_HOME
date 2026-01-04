# CSS Assets - Mobile App

## Overview

This directory contains all CSS stylesheets for the Smart Home mobile application. The styles are optimized for mobile-first design with bottom navigation, touch-friendly controls, and compact layouts suitable for smartphone screens.

## Structure

```
css/
├── style.css              # Main stylesheet (imports all modules)
├── base/                  # Foundation styles
│   ├── variables.css      # CSS custom properties (colors, spacing, shadows)
│   ├── reset.css          # CSS reset and base element styles
│   └── typography.css     # Font styles and text utilities
├── layouts/               # Structural components
│   ├── bottom-nav.css     # Bottom navigation bar (icon-only)
│   ├── main-content.css   # Main content area layout
│   └── mobile-responsive.css # Mobile-specific responsive styles
├── components/            # Reusable UI components
│   ├── buttons.css        # Button styles and variants
│   ├── cards.css          # Device card styles
│   ├── badges.css         # Status indicator styles
│   ├── modals.css         # Base modal dialog styles
│   ├── modal-detail.css   # Device detail modal with charts
│   ├── modal-time.css     # Time configuration modal
│   └── modal-wifi.css     # WiFi setup guide modal
└── views/                 # Page-specific styles
    ├── login.css          # Login page styles
    ├── dashboard.css      # Device grid dashboard styles
    └── settings.css       # Settings page styles
```

## Architecture

### Base Layer

**variables.css**
- Color palette (primary, success, warning, danger)
- Sidebar and navigation colors
- Spacing scale
- Border radius values
- Shadow definitions
- Z-index layers
- Transition durations
- Sensor-specific colors (temperature, humidity, light)

**reset.css**
- Browser default reset
- Box-sizing normalization
- Base element styles

**typography.css**
- Font family (Inter)
- Font sizes and weights
- Line heights
- Text utilities

### Layout Layer

**bottom-nav.css**
- Fixed bottom navigation bar
- Icon-only navigation items (3 tabs: Home, Dashboard, Settings)
- Active state highlighting
- Touch-optimized tap targets (50px height)
- Dark theme styling

**main-content.css**
- Content area with bottom padding for navigation
- Full-width mobile layout
- Scroll behavior
- Safe area insets for notched devices

**mobile-responsive.css**
- Compact card padding
- Optimized button sizes
- Reduced table spacing
- Touch-friendly element sizes
- Viewport-specific adjustments

### Component Layer

**buttons.css**
- Primary, secondary, danger variants
- Compact mobile sizing
- Touch-friendly padding
- Disabled states

**cards.css**
- Device cards with sensor data
- Compact layout for mobile
- Toggle switches
- Status indicators
- Touch interaction feedback

**badges.css**
- Connection status badges
- Compact inline badges
- Color-coded states

**modals.css**
- Base modal styles
- Full-screen mobile modals
- Modal header with close button
- Scroll handling

**modal-detail.css**
- Device detail view
- Stats row with sensor values (temperature, humidity, light)
- Quick control toggles (fan, lamp, pump)
- Chart container
- Compact sensor buttons

**modal-time.css**
- Manual time configuration
- Date and time inputs
- Preview display
- Action buttons

**modal-wifi.css**
- WiFi setup guide steps
- Step indicators
- Code blocks for credentials
- Instruction text

### View Layer

**login.css**
- Centered authentication form
- Full-screen mobile layout
- Input field styling
- Error message display

**dashboard.css**
- Device grid layout
- Single-column mobile view
- Card spacing
- Empty state styling

**settings.css**
- Settings sections
- Clock display
- Device info table
- Action buttons (reboot, time sync, WiFi setup)
- Compact table layout

## Mobile-First Design

### Navigation Pattern
- Bottom navigation bar (iOS/Android native pattern)
- Icon-only tabs for space efficiency
- 3 main sections: Home, Dashboard, Settings
- No sidebar (mobile-optimized)

### Touch Optimization
- Minimum tap targets: 44px
- Adequate spacing between interactive elements
- Visual feedback on touch
- Swipe-friendly layouts

### Viewport Considerations
- Safe area insets for notched devices
- Viewport-fit: cover
- User-scalable: no (native app feel)
- Full-width layouts

### Differences from Web Version
| Feature | Web | Mobile App |
|---------|-----|------------|
| Navigation | Sidebar | Bottom nav |
| Data Export | Full table | Not included |
| Chart View | Tab | Modal popup |
| Layout | Multi-column | Single column |
| Config Panels | Expanded | Compact |

## Color Scheme

```css
--primary-color: #2563eb    /* Main actions */
--success-color: #10b981    /* Positive states */
--warning-color: #f59e0b    /* Caution states */
--danger-color: #ef4444     /* Errors */
--sidebar-bg: #1e293b       /* Navigation background */
--temp-color: #f97316       /* Temperature */
--humid-color: #3b82f6      /* Humidity */
--light-color: #eab308      /* Light */
```

## Typography

- Font family: Inter (Google Fonts)
- Base size: 14px (mobile optimized)
- Weights: 400, 500, 600, 700

## Usage

The main stylesheet automatically imports all modules:

```html
<link rel="stylesheet" href="assets/css/style.css" />
```

## Import Order

1. Base styles (variables, reset, typography)
2. Layout components (bottom-nav, main-content, mobile-responsive)
3. UI components (buttons, cards, badges, modals)
4. View-specific styles (login, dashboard, settings)

## Maintenance

When adding new styles:
1. Place in appropriate directory
2. Import in style.css following order
3. Use CSS variables for consistency
4. Test on multiple screen sizes
5. Ensure touch-friendly sizing
