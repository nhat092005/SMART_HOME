/**
 * chart-config.js
 * Chart configuration constants
 */

export const CHART_CONFIG = {
    MAX_DATA_POINTS: 20,    // Maximum points to show on chart
    animation: false,       // Disable animation for smooth real-time updates
    
    types: {
        temp: {
            label: 'Nhiệt Độ',
            title: 'Biểu đồ Nhiệt Độ',
            unit: '°C',
            color: 'rgb(239, 68, 68)',
            bgColor: 'rgba(239, 68, 68, 0.1)'
        },
        humid: {
            label: 'Độ Ẩm',
            title: 'Biểu đồ Độ Ẩm',
            unit: '%',
            color: 'rgb(59, 130, 246)',
            bgColor: 'rgba(59, 130, 246, 0.1)'
        },
        light: {
            label: 'Ánh Sáng',
            title: 'Biểu đồ Ánh Sáng',
            unit: 'Lux',
            color: 'rgb(234, 179, 8)',
            bgColor: 'rgba(234, 179, 8, 0.1)'
        }
    }
};
