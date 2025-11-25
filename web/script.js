import { auth, db } from './firebase-config.js';
import { requireAuth, getCurrentUser, logout } from './auth.js';
import { ref, onValue, set, update } from "https://www.gstatic.com/firebasejs/10.7.1/firebase-database.js";

// Check Authentication
requireAuth();

// State Management
const state = {
    currentRoom: 'living-room',
    rooms: {
        'living-room': { name: 'Living Room', temp: 24.5, humidity: 45, light: 350, controls: { lights: false, fan: false, ac: false }, history: [] },
        'bedroom': { name: 'Bedroom', temp: 22.0, humidity: 50, light: 100, controls: { lights: false, fan: false, ac: false }, history: [] },
        'kitchen': { name: 'Kitchen', temp: 26.0, humidity: 60, light: 800, controls: { lights: false, fan: false, ac: false }, history: [] },
        'server-room': { name: 'Server Room', temp: 18.0, humidity: 35, light: 50, controls: { lights: true, fan: true, ac: true }, history: [] }
    },
    isSimulation: true // Default to simulation until Firebase connects
};

// Chart Configuration
let tempChart;
const MAX_HISTORY_POINTS = 24; // 24 hours

// Initialize
document.addEventListener('DOMContentLoaded', () => {
    initUser();
    initClock();
    initChart();
    initSidebar();
    initControls();

    // Try to connect to Firebase
    if (db) {
        initFirebaseListeners();
    } else {
        console.warn("Firebase DB not initialized. Running in simulation mode.");
        startSimulation();
    }
});

function initUser() {
    const user = getCurrentUser();
    if (user) {
        // Update profile UI
        const profileEl = document.getElementById('user-profile');
        profileEl.querySelector('.avatar').textContent = user.email[0].toUpperCase();
        profileEl.querySelector('span').textContent = user.displayName || user.email.split('@')[0];
    }

    document.getElementById('logout-btn').addEventListener('click', logout);
}

// Firebase Listeners
function initFirebaseListeners() {
    const roomsRef = ref(db, 'rooms');

    onValue(roomsRef, (snapshot) => {
        const data = snapshot.val();
        if (data) {
            state.isSimulation = false;
            // Merge data into state
            Object.keys(data).forEach(key => {
                if (state.rooms[key]) {
                    state.rooms[key] = { ...state.rooms[key], ...data[key] };
                }
            });
            updateUI();
            updateChartData();
        } else {
            // If no data exists, seed it
            console.log("No data found, seeding database...");
            set(roomsRef, state.rooms);
            state.isSimulation = false;
        }
    }, (error) => {
        console.error("Firebase read failed:", error);
        console.log("Falling back to simulation.");
        startSimulation();
    });
}

// Simulation Mode
function startSimulation() {
    if (!state.isSimulation) return;

    setInterval(() => {
        Object.keys(state.rooms).forEach(roomKey => {
            const room = state.rooms[roomKey];

            // Randomize sensors
            const tempChange = (Math.random() - 0.5) * 0.5;
            room.temp = parseFloat((room.temp + tempChange).toFixed(1));
            room.humidity = Math.max(0, Math.min(100, Math.round(room.humidity + (Math.random() - 0.5) * 2)));
            room.light = Math.max(0, Math.round(room.light + (Math.random() - 0.5) * 10));

            // Update History
            const now = new Date();
            room.history.push({ time: now, value: room.temp });
            if (room.history.length > MAX_HISTORY_POINTS) room.history.shift();
        });

        updateUI();
        updateChartData();
    }, 3000);
}

// UI Updates
function updateUI() {
    const room = state.rooms[state.currentRoom];

    // Header
    document.getElementById('current-room').textContent = room.name;

    // Sensors
    document.getElementById('temp-val').textContent = room.temp;
    document.getElementById('humid-val').textContent = room.humidity;
    document.getElementById('light-val').textContent = room.light;

    // Controls
    updateControlState('lights', room.controls.lights);
    updateControlState('fan', room.controls.fan);
    updateControlState('ac', room.controls.ac);

    // Badges (Active devices count)
    Object.keys(state.rooms).forEach(key => {
        const r = state.rooms[key];
        const activeCount = Object.values(r.controls).filter(v => v).length;
        const badge = document.getElementById(`badge-${key}`);
        if (badge) badge.textContent = activeCount;
    });
}

function updateControlState(device, isOn) {
    const toggle = document.getElementById(`toggle-${device}`);
    const status = document.getElementById(`status-${device}`);
    const card = document.getElementById(`card-${device}`);

    if (toggle) toggle.checked = isOn;
    if (status) status.textContent = isOn ? 'On' : 'Off';
    if (card) {
        if (isOn) card.classList.add('active');
        else card.classList.remove('active');
    }
}

// Device Control Logic
function initControls() {
    ['lights', 'fan', 'ac'].forEach(device => {
        const toggle = document.getElementById(`toggle-${device}`);
        toggle.addEventListener('change', (e) => {
            const isOn = e.target.checked;

            // Update Local State
            state.rooms[state.currentRoom].controls[device] = isOn;
            updateUI(); // Optimistic update

            // Update Firebase
            if (!state.isSimulation && db) {
                const updates = {};
                updates[`rooms/${state.currentRoom}/controls/${device}`] = isOn;
                update(ref(db), updates).catch(err => console.error("Update failed", err));
            }
        });
    });
}

// Sidebar Logic
function initSidebar() {
    const navItems = document.querySelectorAll('.nav-links li[data-room]');
    navItems.forEach(item => {
        item.addEventListener('click', () => {
            document.querySelector('.nav-links li.active').classList.remove('active');
            item.classList.add('active');
            state.currentRoom = item.dataset.room;
            updateUI();
            updateChartData();
        });
    });
}

// Clock
function initClock() {
    const updateTime = () => {
        const now = new Date();
        document.getElementById('clock-display').textContent = now.toLocaleTimeString('en-US', { hour: '2-digit', minute: '2-digit' });
        document.getElementById('date-display').textContent = now.toLocaleDateString('en-US', { weekday: 'long', month: 'long', day: 'numeric' });
    };
    updateTime();
    setInterval(updateTime, 1000);
}

// Chart.js
function initChart() {
    const ctx = document.getElementById('tempChart').getContext('2d');

    const gradient = ctx.createLinearGradient(0, 0, 0, 400);
    gradient.addColorStop(0, 'rgba(79, 70, 229, 0.2)'); // Indigo 600 low opacity
    gradient.addColorStop(1, 'rgba(79, 70, 229, 0.0)');

    tempChart = new Chart(ctx, {
        type: 'line',
        data: {
            labels: [],
            datasets: [{
                label: 'Temperature',
                data: [],
                borderColor: '#4f46e5', // Indigo 600
                backgroundColor: gradient,
                borderWidth: 2,
                fill: true,
                tension: 0.4,
                pointRadius: 0,
                pointHoverRadius: 4
            }]
        },
        options: {
            responsive: true,
            maintainAspectRatio: false,
            plugins: {
                legend: { display: false },
                tooltip: {
                    mode: 'index',
                    intersect: false,
                    backgroundColor: '#1f2937', // Gray 800
                    titleColor: '#f9fafc',
                    bodyColor: '#d1d5db',
                    borderColor: 'rgba(255,255,255,0.1)',
                    borderWidth: 1,
                    padding: 10,
                    cornerRadius: 8
                }
            },
            scales: {
                x: { display: false },
                y: {
                    grid: { color: 'rgba(0, 0, 0, 0.05)' }, // Light gray grid
                    ticks: { color: '#6b7280', font: { family: "'Inter', sans-serif" } }, // Gray 500
                    suggestedMin: 15,
                    suggestedMax: 35,
                    border: { display: false }
                }
            },
            interaction: { intersect: false, mode: 'nearest' }
        }
    });
}

function updateChartData() {
    if (!tempChart) return;
    const room = state.rooms[state.currentRoom];

    // Mock history if empty
    if (room.history.length === 0) {
        const now = new Date();
        for (let i = 10; i >= 0; i--) {
            room.history.push({
                time: new Date(now - i * 3600000),
                value: room.temp - (Math.random() * 2)
            });
        }
    }

    tempChart.data.labels = room.history.map(h => new Date(h.time).toLocaleTimeString());
    tempChart.data.datasets[0].data = room.history.map(h => h.value);
    tempChart.update('none');
}
