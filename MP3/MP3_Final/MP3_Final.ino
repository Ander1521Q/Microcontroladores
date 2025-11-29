// Incluye las bibliotecas necesarias
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Adafruit_BMP280.h>

// -------- CONFIGURACIÓN DE LA RED WiFi --------
const char* ssid = "Yo";
const char* password = "1234567890";

// -------- CONFIGURACIÓN DE PINES --------
#define DHTPIN 8           // DHT11 en GPIO8
#define LDR_PIN 9          // LDR en GPIO9 (ADC2_CH8)
#define I2C_SDA 6          // SDA en GPIO6
#define I2C_SCL 5          // SCL en GPIO5

// -------- CONFIGURACIÓN DEL SENSOR DHT --------
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// -------- CONFIGURACIÓN PANTALLA OLED --------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// -------- CONFIGURACIÓN RTC DS1307 --------
RTC_DS1307 rtc;

// -------- CONFIGURACIÓN SENSOR BMP280 --------
Adafruit_BMP280 bmp;
#define BMP280_I2C_ADDRESS 0x76

// -------- CREACIÓN DEL SERVIDOR WEB --------
WebServer server(80);

// -------- VARIABLES GLOBALES --------
float tempMax = -1000, tempMin = 1000;
float humMax = -1000, humMin = 1000;
float presMax = -1000, presMin = 1000;
float temp = 0, hum = 0, pressure = 0;
bool systemEnabled = true;
String currentTime = "--:--:--";
String currentDate = "--/--/--";
int lightLevel = 0;
int stableLightLevel = 0;
bool isDayMode = true;
bool autoMode = true;  // true = automático, false = manual
String dayPeriod = "Día";

// -------- VARIABLES PARA FILTRADO LDR --------
const int LDR_SAMPLES = 10;
int ldrReadings[LDR_SAMPLES];
int ldrIndex = 0;
unsigned long lastLDRUpdate = 0;
const int LDR_UPDATE_INTERVAL = 100;

// -------- SÍMBOLOS UNICODE PARA OLED --------
const uint8_t thermometer_icon[] PROGMEM = {
  0b00000100,
  0b00001010,
  0b00001010,
  0b00001010,
  0b00001110,
  0b00011111,
  0b00011111,
  0b00011111
};

const uint8_t humidity_icon[] PROGMEM = {
  0b00000100,
  0b00000100,
  0b00001010,
  0b00001010,
  0b00010001,
  0b00010001,
  0b00001010,
  0b00000100
};

const uint8_t pressure_icon[] PROGMEM = {
  0b00011111,
  0b00011111,
  0b00011111,
  0b00000000,
  0b00000000,
  0b00011111,
  0b00011111,
  0b00011111
};

const uint8_t light_icon[] PROGMEM = {
  0b00000100,
  0b00010101,
  0b00001110,
  0b00011111,
  0b00001110,
  0b00010101,
  0b00000100,
  0b00000000
};

// -------- CONTENIDO HTML MEJORADO --------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Estación Meteorológica ESP32</title>
  <style>
    :root {
      --day-bg: linear-gradient(135deg, #667eea 0%, #764ba2 100%);
      --night-bg: linear-gradient(135deg, #0f2027 0%, #203a43 50%, #2c5364 100%);
      --card-day: rgba(255, 255, 255, 0.95);
      --card-night: rgba(45, 52, 71, 0.95);
      --text-day: #2d3748;
      --text-night: #e2e8f0;
    }
    
    html { 
      font-family: Arial, sans-serif; 
      text-align: center; 
      min-height: 100vh;
      padding: 20px;
      transition: all 0.5s ease;
    }
    
    .day-mode {
      background: var(--day-bg);
    }
    
    .night-mode {
      background: var(--night-bg);
    }
    
    h2 { 
      font-size: 2.2rem; 
      margin-bottom: 30px; 
      color: white;
      text-shadow: 2px 2px 4px rgba(0,0,0,0.3);
    }
    
    .container { 
      display: flex; 
      justify-content: center; 
      gap: 20px; 
      flex-wrap: wrap;
      max-width: 1200px;
      margin: 0 auto;
    }
    
    .sensor-card {
      border-radius: 20px;
      padding: 20px;
      min-width: 280px;
      transition: all 0.3s ease;
      box-shadow: 0 10px 30px rgba(0,0,0,0.2);
      display: flex;
      flex-direction: column;
    }
    
    .day-mode .sensor-card {
      background: var(--card-day);
      color: var(--text-day);
    }
    
    .night-mode .sensor-card {
      background: var(--card-night);
      color: var(--text-night);
    }
    
    .sensor-header {
      display: flex;
      align-items: center;
      justify-content: center;
      gap: 15px;
      margin-bottom: 15px;
    }
    
    .sensor-icon {
      font-size: 2rem;
    }
    
    .sensor-title {
      font-size: 1.3rem;
      font-weight: bold;
    }
    
    .sensor-value {
      font-size: 2.2rem;
      font-weight: bold;
      margin: 10px 0;
      order: 3;
    }
    
    /* Gráficos mejorados - Barras verticales delgadas */
    .sensor-graphic {
      margin: 15px 0;
      display: flex;
      justify-content: center;
      align-items: flex-end;
      height: 140px;
      order: 2;
    }
    
    /* Barra de temperatura vertical mejorada */
    .temp-bar-container {
      width: 80px;
      height: 120px;
      position: relative;
      background: #ecf0f1;
      border-radius: 8px;
      border: 2px solid #34495e;
      overflow: hidden;
      margin: 0 10px;
    }
    
    .temp-bar-fill {
      position: absolute;
      bottom: 0;
      width: 100%;
      background: linear-gradient(to top, #1e3c72, #2a5298, #3498db, #2ecc71, #f1c40f, #e67e22, #e74c3c);
      border-radius: 6px;
      transition: height 0.8s ease;
    }
    
    .temp-scale {
      position: absolute;
      left: -25px;
      height: 100%;
      display: flex;
      flex-direction: column;
      justify-content: space-between;
      font-size: 0.7rem;
      color: #7f8c8d;
      font-weight: bold;
    }
    
    .temp-marker {
      position: absolute;
      right: -20px;
      background: #2c3e50;
      color: white;
      padding: 2px 6px;
      border-radius: 4px;
      font-size: 0.8rem;
      font-weight: bold;
      transform: translateY(-50%);
    }
    
    /* BARRA DE PRESIÓN MEJORADA - DISEÑO PROFESIONAL */
    .pressure-gauge-container {
      width: 120px;
      height: 140px;
      position: relative;
      margin: 0 10px;
    }
    
    .pressure-gauge {
      width: 100px;
      height: 120px;
      position: relative;
      background: 
        linear-gradient(to top, 
          #8e44ad 0%, 
          #3498db 25%, 
          #2ecc71 50%, 
          #f1c40f 75%, 
          #e74c3c 100%);
      border-radius: 50px 50px 0 0;
      border: 3px solid #2c3e50;
      overflow: hidden;
      transform: rotate(180deg);
      box-shadow: inset 0 0 15px rgba(0,0,0,0.3);
    }
    
    .pressure-gauge-fill {
      position: absolute;
      bottom: 0;
      width: 100%;
      background: linear-gradient(to bottom, 
        rgba(255,255,255,0.9) 0%, 
        rgba(255,255,255,0.6) 50%, 
        rgba(255,255,255,0.3) 100%);
      border-radius: 50px 50px 0 0;
      transition: height 0.8s ease;
    }
    
    .pressure-gauge-mask {
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      background: linear-gradient(135deg, 
        rgba(255,255,255,0.1) 0%, 
        rgba(255,255,255,0) 50%);
      border-radius: 50px 50px 0 0;
    }
    
    .pressure-scale {
      position: absolute;
      bottom: -25px;
      width: 100%;
      display: flex;
      justify-content: space-between;
      font-size: 0.7rem;
      color: #7f8c8d;
      font-weight: bold;
    }
    
    .pressure-marker {
      position: absolute;
      top: 50%;
      left: 50%;
      transform: translate(-50%, -50%) rotate(90deg);
      background: rgba(44, 62, 80, 0.9);
      color: white;
      padding: 4px 8px;
      border-radius: 12px;
      font-size: 0.8rem;
      font-weight: bold;
      white-space: nowrap;
      box-shadow: 0 2px 8px rgba(0,0,0,0.3);
      z-index: 10;
    }
    
    .pressure-needle {
      position: absolute;
      bottom: 10px;
      left: 50%;
      width: 3px;
      height: 40px;
      background: linear-gradient(to top, #2c3e50, #34495e);
      transform-origin: bottom center;
      transform: translateX(-50%) rotate(0deg);
      transition: transform 0.8s ease;
      border-radius: 2px;
      z-index: 5;
      box-shadow: 0 0 5px rgba(0,0,0,0.5);
    }
    
    .pressure-needle::after {
      content: '';
      position: absolute;
      top: -4px;
      left: -4px;
      width: 10px;
      height: 10px;
      background: #e74c3c;
      border-radius: 50%;
      border: 2px solid #2c3e50;
    }
    
    .pressure-zones {
      position: absolute;
      top: 0;
      left: 0;
      width: 100%;
      height: 100%;
      pointer-events: none;
    }
    
    .pressure-zone {
      position: absolute;
      left: 0;
      width: 100%;
      background: rgba(255,255,255,0.1);
      border-top: 1px dashed rgba(255,255,255,0.3);
    }
    
    .pressure-zone-label {
      position: absolute;
      right: -35px;
      transform: translateY(-50%);
      font-size: 0.6rem;
      color: #7f8c8d;
      font-weight: bold;
      background: rgba(255,255,255,0.9);
      padding: 1px 4px;
      border-radius: 3px;
    }
    
    /* Nube de humedad mejorada */
    .humidity-container {
      position: relative;
      width: 140px;
      height: 120px;
    }
    
    .cloud {
      width: 120px;
      height: 60px;
      background: linear-gradient(135deg, #87CEEB, #B0E0E6, #E0F7FA);
      border-radius: 60px;
      position: absolute;
      top: 30px;
      left: 10px;
      box-shadow: 0 8px 25px rgba(135, 206, 235, 0.4);
      border: 2px solid #87CEEB;
    }
    
    .cloud:before, .cloud:after {
      content: '';
      position: absolute;
      background: inherit;
      border-radius: 50%;
      box-shadow: inherit;
    }
    
    .cloud:before {
      width: 55px;
      height: 55px;
      top: -27px;
      left: 25px;
    }
    
    .cloud:after {
      width: 50px;
      height: 50px;
      top: -25px;
      right: 25px;
    }
    
    .raindrop {
      position: absolute;
      width: 6px;
      background: linear-gradient(to bottom, #3498db, #2980b9);
      border-radius: 0 0 4px 4px;
      animation: rain 1.5s infinite ease-in-out;
      box-shadow: 0 0 8px rgba(52, 152, 219, 0.6);
    }
    
    @keyframes rain {
      0% { transform: translateY(-25px) scaleY(0.5); opacity: 0; }
      50% { opacity: 1; transform: translateY(0) scaleY(1); }
      100% { transform: translateY(50px) scaleY(0.5); opacity: 0; }
    }
    
    .datetime-card {
      border-radius: 20px;
      padding: 20px;
      min-width: 280px;
      margin-bottom: 20px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.2);
    }
    
    .day-mode .datetime-card {
      background: var(--card-day);
      color: var(--text-day);
    }
    
    .night-mode .datetime-card {
      background: var(--card-night);
      color: var(--text-night);
    }
    
    .time-display {
      font-size: 2.5rem;
      font-weight: bold;
      font-family: 'Courier New', monospace;
      margin: 8px 0;
    }
    
    .date-display {
      font-size: 1.2rem;
      margin-bottom: 10px;
      opacity: 0.8;
    }
    
    .light-info {
      margin-top: 12px;
      font-size: 0.9rem;
      opacity: 0.8;
    }
    
    .control-panel {
      border-radius: 20px;
      padding: 20px;
      margin-top: 20px;
      box-shadow: 0 10px 30px rgba(0,0,0,0.2);
    }
    
    .day-mode .control-panel {
      background: var(--card-day);
      color: var(--text-day);
    }
    
    .night-mode .control-panel {
      background: var(--card-night);
      color: var(--text-night);
    }
    
    .btn {
      padding: 12px 25px;
      margin: 8px;
      border: none;
      border-radius: 25px;
      font-size: 1rem;
      font-weight: bold;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 4px 12px rgba(0,0,0,0.2);
      color: white;
    }
    
    .btn-disable {
      background: linear-gradient(135deg, #ff6b6b, #ee5a52);
    }
    
    .btn-enable {
      background: linear-gradient(135deg, #4ecdc4, #44a08d);
    }
    
    .btn-mode {
      background: linear-gradient(135deg, #667eea, #764ba2);
    }
    
    .btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 6px 20px rgba(0,0,0,0.3);
    }
    
    .status {
      font-size: 1.1rem;
      margin: 12px 0;
      padding: 10px;
      border-radius: 10px;
      font-weight: bold;
    }
    
    .status-enabled {
      background: #d4edda;
      color: #155724;
    }
    
    .status-disabled {
      background: #f8d7da;
      color: #721c24;
    }
    
    .mode-control {
      display: flex;
      justify-content: center;
      align-items: center;
      gap: 15px;
      margin: 15px 0;
    }
    
    .mode-btn {
      width: 60px;
      height: 60px;
      border: none;
      border-radius: 15px;
      font-size: 2rem;
      cursor: pointer;
      transition: all 0.3s ease;
      box-shadow: 0 4px 12px rgba(0,0,0,0.2);
    }
    
    .day-mode .mode-btn {
      background: var(--card-day);
    }
    
    .night-mode .mode-btn {
      background: var(--card-night);
    }
    
    .mode-btn.active {
      transform: scale(1.1);
      box-shadow: 0 6px 20px rgba(0,0,0,0.3);
    }
    
    .day-mode .mode-btn.active {
      background: #FFD700;
    }
    
    .night-mode .mode-btn.active {
      background: #4B0082;
    }
    
    .auto-indicator {
      font-size: 0.9rem;
      margin-top: 10px;
      padding: 8px 15px;
      border-radius: 20px;
      background: rgba(76, 175, 80, 0.2);
      color: #4CAF50;
      font-weight: bold;
    }
    
    .manual-indicator {
      font-size: 0.9rem;
      margin-top: 10px;
      padding: 8px 15px;
      border-radius: 20px;
      background: rgba(255, 152, 0, 0.2);
      color: #FF9800;
      font-weight: bold;
    }
    
    .minmax {
      font-size: 0.9rem;
      margin-top: 8px;
      opacity: 0.8;
      order: 4;
    }
  </style>
</head>
<body class="day-mode" id="body">
  <h2>🌤 Estación Meteorológica ESP32</h2>

  <div class="datetime-card">
    <div class="date-display" id="date-display">--/--/--</div>
    <div class="time-display" id="time-display">--:--:--</div>
    <div class="light-info">
      💡 Nivel de luz: <span id="lightLevel">--</span> | 
      Modo: <span id="lightMode">Día</span>
    </div>
  </div>

  <div class="container">
    <!-- Tarjeta de Temperatura -->
    <div class="sensor-card">
      <div class="sensor-header">
        <div class="sensor-icon">🌡️</div>
        <div class="sensor-title">Temperatura</div>
      </div>
      <div class="sensor-graphic">
        <div class="temp-bar-container">
          <div class="temp-bar-fill" id="temp-bar-fill"></div>
          <div class="temp-scale">
            <span>50°C</span>
            <span>40°C</span>
            <span>30°C</span>
            <span>20°C</span>
            <span>10°C</span>
            <span>0°C</span>
            <span>-10°C</span>
          </div>
          <div class="temp-marker" id="temp-marker">--°C</div>
        </div>
      </div>
      <div class="sensor-value">
        <span id="temperature">--</span>°C
      </div>
      <div class="minmax">
        Máx: <span id="temperature-max">--</span>°C | Mín: <span id="temperature-min">--</span>°C
      </div>
    </div>

    <!-- Tarjeta de Humedad -->
    <div class="sensor-card">
      <div class="sensor-header">
        <div class="sensor-icon">💧</div>
        <div class="sensor-title">Humedad</div>
      </div>
      <div class="sensor-graphic">
        <div class="humidity-container" id="humidity-container">
          <div class="cloud"></div>
        </div>
      </div>
      <div class="sensor-value">
        <span id="humidity">--</span>%
      </div>
      <div class="minmax">
        Máx: <span id="humidity-max">--</span>% | Mín: <span id="humidity-min">--</span>%
      </div>
    </div>

    <!-- Tarjeta de Presión MEJORADA -->
    <div class="sensor-card">
      <div class="sensor-header">
        <div class="sensor-icon">📊</div>
        <div class="sensor-title">Presión Atmosférica</div>
      </div>
      <div class="sensor-graphic">
        <div class="pressure-gauge-container">
          <div class="pressure-gauge">
            <div class="pressure-gauge-fill" id="pressure-gauge-fill"></div>
            <div class="pressure-gauge-mask"></div>
            <div class="pressure-needle" id="pressure-needle"></div>
            <div class="pressure-zones" id="pressure-zones"></div>
          </div>
          <div class="pressure-marker" id="pressure-marker">-- hPa</div>
          <div class="pressure-scale">
            <span>850</span>
            <span>950</span>
            <span>1050</span>
            <span>1100</span>
          </div>
        </div>
      </div>
      <div class="sensor-value">
        <span id="pressure">--</span> hPa
      </div>
      <div class="minmax">
        Máx: <span id="pressure-max">--</span> | Mín: <span id="pressure-min">--</span>
      </div>
    </div>
  </div>

  <div class="control-panel">
    <div class="status status-enabled" id="systemStatus">✅ SISTEMA ACTIVADO</div>
    <button class="btn btn-disable" onclick="toggleSystem()" id="toggleBtn">🛑 DESACTIVAR SISTEMA</button>
    
    <div style="margin: 20px 0;">
      <button class="btn btn-mode" onclick="toggleMode()" id="modeBtn">🔄 MODO AUTOMÁTICO</button>
      <div id="modeIndicator" class="auto-indicator">🔧 Modo: Automático (LDR)</div>
    </div>
    
    <div class="mode-control" id="manualControl" style="display: none;">
      <button class="mode-btn" onclick="setManualMode(true)" id="dayBtn">☀️</button>
      <button class="mode-btn" onclick="setManualMode(false)" id="nightBtn">🌙</button>
    </div>
  </div>

<script>
// Inicializar zonas de presión
function initPressureZones() {
  const zonesContainer = document.getElementById('pressure-zones');
  const zones = [
    { height: 20, label: 'Baja' },
    { height: 40, label: 'Normal' },
    { height: 40, label: 'Alta' }
  ];
  
  let currentTop = 0;
  zones.forEach((zone, index) => {
    const zoneElement = document.createElement('div');
    zoneElement.className = 'pressure-zone';
    zoneElement.style.height = zone.height + '%';
    zoneElement.style.top = currentTop + '%';
    
    const labelElement = document.createElement('div');
    labelElement.className = 'pressure-zone-label';
    labelElement.textContent = zone.label;
    labelElement.style.top = (currentTop + zone.height / 2) + '%';
    
    zoneElement.appendChild(labelElement);
    zonesContainer.appendChild(zoneElement);
    
    currentTop += zone.height;
  });
}

function updateData() {
  fetch('/sensors')
    .then(response => {
      if (!response.ok) {
        throw new Error('Error del servidor');
      }
      return response.json();
    })
    .then(data => {
      console.log('Datos recibidos:', data);
      
      // Actualizar temperatura y barra
      document.getElementById('temperature').textContent = data.temp.toFixed(1);
      document.getElementById('temperature-max').textContent = data.tempMax.toFixed(1);
      document.getElementById('temperature-min').textContent = data.tempMin.toFixed(1);
      updateTempBar(data.temp);
      
      // Actualizar humedad y nube
      document.getElementById('humidity').textContent = data.hum.toFixed(1);
      document.getElementById('humidity-max').textContent = data.humMax.toFixed(1);
      document.getElementById('humidity-min').textContent = data.humMin.toFixed(1);
      updateHumidityCloud(data.hum);
      
      // Actualizar presión y medidor
      document.getElementById('pressure').textContent = data.pressure.toFixed(1);
      document.getElementById('pressure-max').textContent = data.presMax.toFixed(1);
      document.getElementById('pressure-min').textContent = data.presMin.toFixed(1);
      updatePressureGauge(data.pressure);
      
      // Actualizar fecha y hora
      document.getElementById('time-display').textContent = data.time;
      document.getElementById('date-display').textContent = data.date;
      
      // Actualizar estado del sistema
      document.getElementById('systemStatus').textContent = data.systemEnabled ? 
        '✅ SISTEMA ACTIVADO' : '❌ SISTEMA DESACTIVADO';
      document.getElementById('systemStatus').className = data.systemEnabled ? 
        'status status-enabled' : 'status status-disabled';
      document.getElementById('toggleBtn').textContent = data.systemEnabled ? 
        '🛑 DESACTIVAR SISTEMA' : '✅ ACTIVAR SISTEMA';
      document.getElementById('toggleBtn').className = data.systemEnabled ? 
        'btn btn-disable' : 'btn btn-enable';
      
      // Actualizar información de luz y modo
      document.getElementById('lightLevel').textContent = data.lightLevel;
      document.getElementById('lightMode').textContent = data.dayPeriod;
      
      // Actualizar control de modo
      updateModeControl(data.autoMode, data.isDayMode);
      
      // Cambiar modo día/noche
      document.getElementById('body').className = data.isDayMode ? 'day-mode' : 'night-mode';
    })
    .catch(error => {
      console.error('Error:', error);
    });
}

function updateTempBar(temp) {
  const barFill = document.getElementById('temp-bar-fill');
  const marker = document.getElementById('temp-marker');
  
  // Mapear temperatura (-10°C a 50°C) a altura (0-100%)
  const minTemp = -10;
  const maxTemp = 50;
  const height = ((temp - minTemp) / (maxTemp - minTemp)) * 100;
  const clampedHeight = Math.min(Math.max(height, 0), 100);
  
  barFill.style.height = clampedHeight + '%';
  
  // Posicionar el marcador
  const markerPosition = 100 - clampedHeight;
  marker.style.top = markerPosition + '%';
  marker.textContent = temp.toFixed(1) + '°C';
}

function updatePressureGauge(pressure) {
  const gaugeFill = document.getElementById('pressure-gauge-fill');
  const needle = document.getElementById('pressure-needle');
  const marker = document.getElementById('pressure-marker');
  
  // Mapear presión (850-1100 hPa) a altura (0-100%) y ángulo (0-180°)
  const minPressure = 850;
  const maxPressure = 1100;
  const height = ((pressure - minPressure) / (maxPressure - minPressure)) * 100;
  const clampedHeight = Math.min(Math.max(height, 0), 100);
  
  // Calcular ángulo para la aguja (-90° a 90°)
  const angle = (clampedHeight / 100) * 180 - 90;
  
  gaugeFill.style.height = clampedHeight + '%';
  needle.style.transform = `translateX(-50%) rotate(${angle}deg)`;
  marker.textContent = pressure.toFixed(1) + ' hPa';
  
  // Cambiar color del marcador según la presión
  if (pressure < 950) {
    marker.style.background = 'rgba(231, 76, 60, 0.9)'; // Rojo para baja presión
  } else if (pressure < 1050) {
    marker.style.background = 'rgba(46, 204, 113, 0.9)'; // Verde para presión normal
  } else {
    marker.style.background = 'rgba(52, 152, 219, 0.9)'; // Azul para alta presión
  }
}

function updateHumidityCloud(humidity) {
  const container = document.getElementById('humidity-container');
  // Limpiar gotas existentes
  const existingDrops = container.querySelectorAll('.raindrop');
  existingDrops.forEach(drop => drop.remove());
  
  // Crear nuevas gotas según humedad
  const dropCount = Math.floor((humidity / 100) * 18);
  for (let i = 0; i < dropCount; i++) {
    const drop = document.createElement('div');
    drop.className = 'raindrop';
    drop.style.left = `${15 + (i % 6) * 18}px`;
    drop.style.height = `${10 + Math.random() * 15}px`;
    drop.style.animationDelay = `${Math.random() * 2}s`;
    container.appendChild(drop);
  }
}

function updateModeControl(autoMode, isDayMode) {
  const modeBtn = document.getElementById('modeBtn');
  const modeIndicator = document.getElementById('modeIndicator');
  const manualControl = document.getElementById('manualControl');
  const dayBtn = document.getElementById('dayBtn');
  const nightBtn = document.getElementById('nightBtn');
  
  if (autoMode) {
    modeBtn.textContent = '🔄 MODO AUTOMÁTICO';
    modeIndicator.className = 'auto-indicator';
    modeIndicator.textContent = '🔧 Modo: Automático (LDR)';
    manualControl.style.display = 'none';
  } else {
    modeBtn.textContent = '👤 MODO MANUAL';
    modeIndicator.className = 'manual-indicator';
    modeIndicator.textContent = '👤 Modo: Manual';
    manualControl.style.display = 'flex';
    
    // Actualizar botones activos
    dayBtn.className = isDayMode ? 'mode-btn active' : 'mode-btn';
    nightBtn.className = !isDayMode ? 'mode-btn active' : 'mode-btn';
  }
}

function toggleSystem() {
  fetch('/toggle', { method: 'POST' })
    .then(response => response.json())
    .then(data => {
      updateData();
    });
}

function toggleMode() {
  fetch('/toggleMode', { method: 'POST' })
    .then(response => response.json())
    .then(data => {
      updateData();
    });
}

function setManualMode(dayMode) {
  fetch('/setMode?mode=' + (dayMode ? 'day' : 'night'), { method: 'POST' })
    .then(response => response.json())
    .then(data => {
      updateData();
    });
}

// Inicializar al cargar
document.addEventListener('DOMContentLoaded', function() {
  initPressureZones();
});

// Actualizar cada segundo
setInterval(updateData, 1000);
window.onload = updateData;
</script>
</body>
</html>
)rawliteral";

// -------- FUNCIONES PARA LDR --------
void initLDRFilter() {
  for (int i = 0; i < LDR_SAMPLES; i++) {
    ldrReadings[i] = analogRead(LDR_PIN);
    delay(10);
  }
  Serial.println("✅ Filtro LDR inicializado");
}

int readStableLDR() {
  int newReading = analogRead(LDR_PIN);
  
  ldrReadings[ldrIndex] = newReading;
  ldrIndex = (ldrIndex + 1) % LDR_SAMPLES;
  
  long sum = 0;
  for (int i = 0; i < LDR_SAMPLES; i++) {
    sum += ldrReadings[i];
  }
  
  return sum / LDR_SAMPLES;
}

void determineDayPeriod() {
  if (autoMode) {
    // Modo automático - usar LDR - AJUSTADO HASTA 2800 PARA MODO CLARO
    if (stableLightLevel < 100) {
      dayPeriod = "Luz Extrema";
      isDayMode = true;
    } else if (stableLightLevel < 500) {
      dayPeriod = "Día Brillante";
      isDayMode = true;
    } else if (stableLightLevel < 1500) {
      dayPeriod = "Día";
      isDayMode = true;
    } else if (stableLightLevel < 2800) {  // AJUSTADO: hasta 2800 es modo claro
      dayPeriod = "Atardecer";
      isDayMode = true;
    } else if (stableLightLevel < 3500) {
      dayPeriod = "Anochecer";
      isDayMode = false;
    } else {
      dayPeriod = "Noche";
      isDayMode = false;
    }
  } else {
    // Modo manual - mantener el modo actual
    dayPeriod = isDayMode ? "Día (Manual)" : "Noche (Manual)";
  }
}

// -------- FUNCIÓN PARA MOSTRAR PANTALLA DE INICIO --------
void displayStartupScreen() {
  display.clearDisplay();
  
  // Título grande
  display.setTextSize(2);
  display.setCursor(5, 10);
  display.println("ESTACION");
  display.setCursor(15, 30);
  display.println("METEO");
  display.setCursor(10, 50);
  display.println("ESP32");
  
  display.display();
  delay(3000); // Mostrar por 3 segundos
}

// -------- FUNCIÓN PARA MOSTRAR DATOS EN OLED --------
void displayOLEDData() {
  display.clearDisplay();
  
  if (!systemEnabled) {
    // Pantalla cuando el sistema está desactivado
    display.setTextSize(2);
    display.setCursor(15, 10);
    display.println("SISTEMA");
    display.setCursor(5, 30);
    display.println("PAUSADO");
    
    display.setTextSize(1);
    display.setCursor(20, 50);
    display.print("IP:");
    display.println(WiFi.localIP().toString());
  } else {
    // Encabezado con fecha y hora
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(currentDate);
    display.setCursor(85, 0);
    display.println(currentTime);
    
    // Línea separadora
    display.drawFastHLine(0, 9, 128, SSD1306_WHITE);
    
    // Temperatura grande en el centro
    display.setTextSize(2);
    display.setCursor(15, 15);
    display.print(temp, 1);
    display.println(" C");
    display.drawBitmap(0, 17, thermometer_icon, 8, 8, SSD1306_WHITE);
    
    // Humedad
    display.setTextSize(1);
    display.setCursor(0, 35);
    display.print("Hum:");
    display.setCursor(35, 35);
    display.print(hum, 1);
    display.println("%");
    display.drawBitmap(75, 35, humidity_icon, 8, 8, SSD1306_WHITE);
    
    // Presión
    display.setCursor(0, 45);
    display.print("Pres:");
    display.setCursor(35, 45);
    display.print(pressure, 1);
    display.println("hPa");
    display.drawBitmap(75, 45, pressure_icon, 8, 8, SSD1306_WHITE);
    
    // Estado del sistema y luz
    display.setCursor(0, 55);
    display.print(systemEnabled ? "ACTIVO" : "PAUSA");
    display.setCursor(50, 55);
    display.print("Luz:");
    display.print(stableLightLevel);
    display.drawBitmap(95, 55, light_icon, 8, 8, SSD1306_WHITE);
  }
  
  display.display();
}

// -------- FUNCIÓN PARA LEER SENSORES --------
void readSensors() {
  if (!systemEnabled) {
    return;
  }

  temp = dht.readTemperature();
  hum = dht.readHumidity();
  
  if (bmp.begin(BMP280_I2C_ADDRESS)) {
    pressure = bmp.readPressure() / 100.0;
  } else {
    pressure = 0;
  }
  
  if (millis() - lastLDRUpdate >= LDR_UPDATE_INTERVAL) {
    lightLevel = analogRead(LDR_PIN);
    stableLightLevel = readStableLDR();
    lastLDRUpdate = millis();
  }
  
  determineDayPeriod();

  if (!isnan(temp) && !isnan(hum)) {
    if (temp > tempMax) tempMax = temp;
    if (temp < tempMin) tempMin = temp;
    if (hum > humMax) humMax = hum;
    if (hum < humMin) humMin = hum;
  }
  
  if (!isnan(pressure)) {
    if (pressure > presMax) presMax = pressure;
    if (pressure < presMin) presMin = pressure;
  }

  if (rtc.now().isValid()) {
    DateTime now = rtc.now();
    currentTime = String(now.hour()) + ":" + 
                 (now.minute() < 10 ? "0" : "") + String(now.minute()) + ":" + 
                 (now.second() < 10 ? "0" : "") + String(now.second());
    currentDate = String(now.day()) + "/" + 
                  (now.month() < 10 ? "0" : "") + String(now.month()) + "/" + 
                  String(now.year());
  }
}

// -------- MANEJADORES DEL SERVIDOR WEB --------
void handleSensorData() {
  readSensors();

  String json = "{";
  json += "\"temp\":" + String(temp, 1) + ",";
  json += "\"tempMax\":" + String(tempMax, 1) + ",";
  json += "\"tempMin\":" + String(tempMin, 1) + ",";
  json += "\"hum\":" + String(hum, 1) + ",";
  json += "\"humMax\":" + String(humMax, 1) + ",";
  json += "\"humMin\":" + String(humMin, 1) + ",";
  json += "\"pressure\":" + String(pressure, 1) + ",";
  json += "\"presMax\":" + String(presMax, 1) + ",";
  json += "\"presMin\":" + String(presMin, 1) + ",";
  json += "\"date\":\"" + currentDate + "\",";
  json += "\"time\":\"" + currentTime + "\",";
  json += "\"systemEnabled\":" + String(systemEnabled ? "true" : "false") + ",";
  json += "\"lightLevel\":" + String(stableLightLevel) + ",";
  json += "\"dayPeriod\":\"" + dayPeriod + "\",";
  json += "\"autoMode\":" + String(autoMode ? "true" : "false") + ",";
  json += "\"isDayMode\":" + String(isDayMode ? "true" : "false");
  json += "}";

  server.send(200, "application/json", json);
}

void handleToggleSystem() {
  systemEnabled = !systemEnabled;
  String json = "{\"status\":\"ok\", \"enabled\":" + String(systemEnabled ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

void handleToggleMode() {
  autoMode = !autoMode;
  String json = "{\"status\":\"ok\", \"autoMode\":" + String(autoMode ? "true" : "false") + "}";
  Serial.println("🔧 Modo " + String(autoMode ? "automático" : "manual"));
  server.send(200, "application/json", json);
}

void handleSetMode() {
  if (server.hasArg("mode")) {
    String mode = server.arg("mode");
    isDayMode = (mode == "day");
    String json = "{\"status\":\"ok\", \"isDayMode\":" + String(isDayMode ? "true" : "false") + "}";
    Serial.println("🔧 Modo manual establecido: " + String(isDayMode ? "Día" : "Noche"));
    server.send(200, "application/json", json);
  } else {
    server.send(400, "application/json", "{\"error\":\"Parámetro mode faltante\"}");
  }
}

// -------- CONFIGURACIÓN INICIAL --------
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("\n🌤 Iniciando Estación Meteorológica ESP32...");

  // Inicializar I2C
  Wire.begin(I2C_SDA, I2C_SCL);
  
  // Inicializar OLED con verificación de error
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ Error al inicializar la pantalla OLED");
    while(1); // Detener ejecución si falla la pantalla
  } else {
    Serial.println("✅ Pantalla OLED inicializada correctamente");
  }

  // Mostrar pantalla de inicio
  displayStartupScreen();

  // Configurar fuente
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Inicializar sensores
  dht.begin();
  Serial.println("✅ Sensor DHT11 inicializado");

  if (!rtc.begin()) {
    Serial.println("❌ RTC no encontrado, ajustando hora del sistema...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("✅ RTC DS1307 inicializado");
  }

  if (bmp.begin(BMP280_I2C_ADDRESS)) {
    Serial.println("✅ Sensor BMP280 inicializado");
  } else {
    Serial.println("❌ Error al inicializar BMP280");
  }

  initLDRFilter();

  // Conectar a WiFi
  WiFi.begin(ssid, password);
  Serial.print("📡 Conectando a WiFi");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Conectando WiFi");
  display.display();
  
  int timeout = 0;
  while (WiFi.status() != WL_CONNECTED && timeout < 20) {
    delay(1000);
    Serial.print(".");
    timeout++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ Conectado a WiFi");
    Serial.print("📶 IP asignada: ");
    Serial.println(WiFi.localIP());
    
    // Mostrar información de conexión en pantalla OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("WiFi Conectado!");
    display.print("IP: ");
    display.println(WiFi.localIP().toString());
    display.display();
    delay(2000);
  } else {
    Serial.println("\n❌ Error de conexión WiFi");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Error WiFi!");
    display.display();
  }

  // Configurar servidor web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/sensors", HTTP_GET, handleSensorData);
  server.on("/toggle", HTTP_POST, handleToggleSystem);
  server.on("/toggleMode", HTTP_POST, handleToggleMode);
  server.on("/setMode", HTTP_POST, handleSetMode);

  server.begin();
  Serial.println("🌐 Servidor web iniciado");

  readSensors();
  Serial.println("🚀 Sistema listo");
  
  // Mostrar información final en monitor serial
  Serial.println("\n═══════════════════════════════════════");
  Serial.println("   ESTACIÓN METEOROLÓGICA ESP32");
  Serial.println("═══════════════════════════════════════");
  Serial.println("📊 Sensores disponibles:");
  Serial.println("   • DHT11 (Temperatura/Humedad)");
  Serial.println("   • BMP280 (Presión Atmosférica)");
  Serial.println("   • LDR (Sensor de Luz)");
  Serial.println("   • RTC DS1307 (Reloj Tiempo Real)");
  Serial.println("🌐 Servidor Web:");
  Serial.println("   http://" + WiFi.localIP().toString());
  Serial.println("💡 Modo Claro hasta: 2800 LDR");
  Serial.println("📊 Gráfico de presión: Medidor profesional");
  Serial.println("═══════════════════════════════════════\n");
}

// -------- BUCLE PRINCIPAL --------
void loop() {
  server.handleClient();
  
  static unsigned long lastOLEDUpdate = 0;
  if (millis() - lastOLEDUpdate >= 2000) {
    readSensors();
    displayOLEDData();
    lastOLEDUpdate = millis();
  }
  
  delay(10);
}