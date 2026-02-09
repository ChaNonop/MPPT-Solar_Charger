// -- Configuration --
const MAX_DATA_POINTS = 50; // จำนวนจุดสูงสุดบนกราฟ I-V
const MAX_TIME_POINTS = 30; // จำนวนจุดสูงสุดบนกราฟ Power vs Time

// -- Setup I-V Curve Chart (Scatter Plot) --
const ctxIV = document.getElementById('ivCurveChart').getContext('2d');
const ivChart = new Chart(ctxIV, {
    type: 'scatter',
    data: {
        datasets: [{
            label: 'Operating Point',
            data: [],
            backgroundColor: 'rgba(34, 211, 238, 1)', // Cyan
            borderColor: 'rgba(34, 211, 238, 0.5)',
            borderWidth: 2,
            showLine: true, // ลากเส้นเชื่อมจุด
            pointRadius: 3,
            tension: 0.4 // ความโค้งของเส้น
        },
        {
            label: 'Current Point', // จุดปัจจุบันที่ใหญ่กว่า
            data: [],
            backgroundColor: 'rgba(250, 204, 21, 1)', // Yellow
            pointRadius: 8,
            pointHoverRadius: 10
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            x: {
                type: 'linear',
                position: 'bottom',
                title: { display: true, text: 'Voltage (V)', color: '#94a3b8' },
                grid: { color: 'rgba(255, 255, 255, 0.1)' },
                ticks: { color: '#94a3b8' }
            },
            y: {
                title: { display: true, text: 'Current (A)', color: '#94a3b8' },
                grid: { color: 'rgba(255, 255, 255, 0.1)' },
                ticks: { color: '#94a3b8' }
            }
        },
        plugins: {
            legend: { labels: { color: '#e2e8f0' } }
        }
    }
});

// -- Setup Power vs Time Chart (Line Chart) --
const ctxPower = document.getElementById('powerTimeChart').getContext('2d');
const powerChart = new Chart(ctxPower, {
    type: 'line',
    data: {
        labels: [], // เวลา
        datasets: [{
            label: 'Power (W)',
            data: [],
            borderColor: '#4ade80', // Green
            backgroundColor: 'rgba(74, 222, 128, 0.2)',
            fill: true,
            tension: 0.3
        }]
    },
    options: {
        responsive: true,
        maintainAspectRatio: false,
        scales: {
            x: { display: false }, // ซ่อนแกนเวลาเพื่อความสะอาด
            y: {
                grid: { color: 'rgba(255, 255, 255, 0.1)' },
                ticks: { color: '#94a3b8' },
                beginAtZero: true
            }
        },
        plugins: {
            legend: { display: false }
        },
        animation: { duration: 0 } // ปิด animation เพื่อ performance ที่ดีเมื่อ update ถี่
    }
});

// -- Function to Fetch Data --
async function fetchData() {
    try {
        const response = await fetch('/api/data');
        const data = await response.json();

        // 1. Update Text UI
        document.getElementById('val-volt').innerText = data.voltage.toFixed(2);
        document.getElementById('val-curr').innerText = data.current.toFixed(2);
        document.getElementById('val-power').innerText = data.power.toFixed(2);
        document.getElementById('val-duty').innerText = data.duty_cycle;

        // 2. Update I-V Curve Logic
        
        // เพิ่มจุดใหม่
        ivChart.data.datasets[0].data.push({x: data.voltage, y: data.current});
        
        // เรียงลำดับตาม Voltage
        ivChart.data.datasets[0].data.sort((a, b) => a.x - b.x);

        // จำกัดจำนวนจุดไม่ให้เยอะเกินไป (Performance)
        if (ivChart.data.datasets[0].data.length > MAX_DATA_POINTS) {
             ivChart.data.datasets[0].data.splice(0, 1); 
        }

        // จุดปัจจุบัน (Current Operating Point)
        ivChart.data.datasets[1].data = [{x: data.voltage, y: data.current}];
        ivChart.update();

        // 3. Update Power Chart (Time based)
        const now = new Date().toLocaleTimeString();
        powerChart.data.labels.push(now);
        powerChart.data.datasets[0].data.push(data.power);

        if (powerChart.data.labels.length > MAX_TIME_POINTS) {
            powerChart.data.labels.shift();
            powerChart.data.datasets[0].data.shift();
        }
        powerChart.update();

    } catch (error) {
        console.error("Error fetching data:", error);
        
        // --- ส่วนจำลองข้อมูล (สำหรับทดสอบตอนยังไม่ต่อบอร์ดจริง) ---
        // ถ้า fetch ไม่ได้ จะ generate dummy data แทน
        // ลบส่วนนี้ออกได้เมื่อใช้งานจริง
        /*
        const v = Math.random() * 20;
        const i = Math.random() * 5;
        const p = v * i;
        const dummy = {
            voltage: v,
            current: i,
            power: p,
            duty_cycle: Math.floor(Math.random() * 100)
        };
        // Update charts with dummy... (logic same as above)
        */
    }
}

// เรียกข้อมูลทุกๆ 500ms (0.5 วินาที)
setInterval(fetchData, 500);