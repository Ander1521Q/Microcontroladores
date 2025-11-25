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

// -------- CONFIGURACIÓN DEL SENSOR DHT --------
#define DHTPIN 4
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

// -------- CONFIGURACIÓN SENSOR DE LUZ (LDR) --------
#define LDR_PIN 5

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
bool isDayMode = true;

// -------- HTML CON MODO DÍA/NOCHE --------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html><head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Estación Meteorológica ESP32</title>
  <style>
    html { 
      font-family: Arial; 
      text-align: center; 
      transition: background-color 0.5s, color 0.5s;
    }
    
    .day-mode {
      background-color: #f0f8ff;
      color: #333;
    }
    
    .night-mode {
      background-color: #1a1a2e;
      color: #e6e6e6;
    }
    
    h2 { font-size: 2.2rem; margin-bottom: 20px; }
    
    .datetime {
      font-size: 1.2rem;
      margin-bottom: 20px;
      padding: 10px;
      background: rgba(0,0,0,0.1);
      border-radius: 10px;
    }
    
    .container { 
      display: flex; 
      justify-content: center; 
      gap: 30px; 
      flex-wrap: wrap; 
      margin-bottom: 20px;
    }
    
    .sensor-block { 
      display: flex; 
      flex-direction: column; 
      align-items: center; 
      margin-bottom: 25px;
      padding: 15px;
      border-radius: 15px;
      transition: background-color 0.5s;
    }
    
    .day-mode .sensor-block {
      background: linear-gradient(135deg, #ffffff, #e6f3ff);
      box-shadow: 0 4px 15px rgba(0,0,0,0.1);
    }
    
    .night-mode .sensor-block {
      background: linear-gradient(135deg, #2d3047, #1b262c);
      box-shadow: 0 4px 15px rgba(255,255,255,0.1);
    }
    
    .bar-wrapper { 
      display: flex; 
      flex-direction: row; 
      align-items: flex-end; 
      gap: 8px; 
    }
    
    .bar-container {
      position: relative; 
      width: 50px; 
      height: 200px;
      border-radius: 25px; 
      overflow: hidden; 
      border: 2px solid;
      transition: border-color 0.5s;
    }
    
    .day-mode .bar-container {
      border-color: #999;
    }
    
    .night-mode .bar-container {
      border-color: #666;
    }
    
    .temp-bar {
      background: linear-gradient(to top, #0000ff, #00ffff, #00ff00, #ffff00, #ff0000);
    }
    
    .hum-bar {
      background: linear-gradient(to top, #8B4513, #FFD700, #87CEEB);
    }
    
    .pres-bar {
      background: linear-gradient(to top, #4B0082, #0000FF, #00FFFF);
    }
    
    .bar-mask {
      position: absolute; 
      top: 0; 
      width: 100%;
      background: rgba(0, 0, 0, 0.6);
      transition: height 0.5s; 
      z-index: 1;
    }
    
    .scale {
      display: flex; 
      flex-direction: column; 
      justify-content: space-between;
      height: 200px; 
      font-size: 0.8rem;
    }
    
    .label { 
      margin-top: 10px; 
      font-size: 1.1rem; 
      font-weight: bold; 
      margin-bottom: 5px;
    }
    
    .value { 
      font-size: 1.3rem; 
      margin-top: 5px; 
      font-weight: bold;
    }
    
    .minmax { 
      font-size: 0.9rem; 
      margin-top: 4px; 
      color: #666;
    }
    
    .night-mode .minmax {
      color: #ccc;
    }
    
    .control-panel {
      margin: 20px 0;
      padding: 20px;
      border-radius: 15px;
      transition: background-color 0.5s;
    }
    
    .day-mode .control-panel {
      background: linear-gradient(135deg, #e6f3ff, #ffffff);
    }
    
    .night-mode .control-panel {
      background: linear-gradient(135deg, #1b262c, #2d3047);
    }
    
    .btn {
      padding: 12px 30px;
      margin: 0 10px;
      border: none;
      border-radius: 25px;
      font-size: 1rem;
      font-weight: bold;
      cursor: pointer;
      transition: all 0.3s;
    }
    
    .btn-enable {
      background: linear-gradient(135deg, #4CAF50, #45a049);
      color: white;
    }
    
    .btn-disable {
      background: linear-gradient(135deg, #f44336, #da190b);
      color: white;
    }
    
    .btn:hover {
      transform: translateY(-2px);
      box-shadow: 0 5px 15px rgba(0,0,0,0.2);
    }
    
    .status {
      font-size: 1.1rem;
      margin: 15px 0;
      padding: 10px;
      border-radius: 10px;
      font-weight: bold;
    }
    
    .status-enabled {
      background-color: #d4edda;
      color: #155724;
    }
    
    .status-disabled {
      background-color: #f8d7da;
      color: #721c24;
    }
    
    .light-info {
      font-size: 1rem;
      margin-top: 10px;
      padding: 8px;
      border-radius: 8px;
      background: rgba(0,0,0,0.1);
    }
  </style>
</head>
<body class="day-mode" id="body">
  <h2>🌤 Estación Meteorológica ESP32</h2>
  
  <div class="datetime">
    <div id="datetime">--/--/-- --:--:--</div>
  </div>
  
  <div class="control-panel">
    <div class="status status-enabled" id="systemStatus">SISTEMA ACTIVADO</div>
    <button class="btn btn-disable" onclick="toggleSystem()" id="toggleBtn">DESACTIVAR SISTEMA</button>
    <div class="light-info">Nivel de luz: <span id="lightLevel">--</span> | Modo: <span id="lightMode">Día</span></div>
  </div>

  <div class="container">
    <!-- Termómetro de Temperatura -->
    <div class="sensor-block">
      <div class="bar-wrapper">
        <div class="bar-container temp-bar">
          <div class="bar-mask" id="temperature-mask" style="height: 100%;"></div>
        </div>
        <div class="scale">
          <div>50</div><div>40</div><div>30</div><div>20</div><div>10</div><div>0</div>
        </div>
      </div>
      <div class="label">🌡 Temperatura</div>
      <div class="value"><span id="temperature">--</span> °C</div>
      <div class="minmax">Máx: <span id="temperature-max">--</span> °C | Mín: <span id="temperature-min">--</span> °C</div>
    </div>

    <!-- Termómetro de Humedad -->
    <div class="sensor-block">
      <div class="bar-wrapper">
        <div class="bar-container hum-bar">
          <div class="bar-mask" id="humidity-mask" style="height: 100%;"></div>
        </div>
        <div class="scale">
          <div>100</div><div>80</div><div>60</div><div>40</div><div>20</div><div>0</div>
        </div>
      </div>
      <div class="label">💧 Humedad</div>
      <div class="value"><span id="humidity">--</span> %</div>
      <div class="minmax">Máx: <span id="humidity-max">--</span> % | Mín: <span id="humidity-min">--</span> %</div>
    </div>

    <!-- Barómetro de Presión -->
    <div class="sensor-block">
      <div class="bar-wrapper">
        <div class="bar-container pres-bar">
          <div class="bar-mask" id="pressure-mask" style="height: 100%;"></div>
        </div>
        <div class="scale">
          <div>1100</div><div>1050</div><div>1000</div><div>950</div><div>900</div><div>850</div>
        </div>
      </div>
      <div class="label">📊 Presión</div>
      <div class="value"><span id="pressure">--</span> hPa</div>
      <div class="minmax">Máx: <span id="pressure-max">--</span> | Mín: <span id="pressure-min">--</span></div>
    </div>
  </div>

  <script>
    function updateData() {
      fetch('/sensors')
        .then(response => response.json())
        .then(data => {
          // Actualizar datos de sensores
          document.getElementById('temperature').textContent = data.temp.toFixed(1);
          document.getElementById('temperature-max').textContent = data.tempMax.toFixed(1);
          document.getElementById('temperature-min').textContent = data.tempMin.toFixed(1);
          
          document.getElementById('humidity').textContent = data.hum.toFixed(1);
          document.getElementById('humidity-max').textContent = data.humMax.toFixed(1);
          document.getElementById('humidity-min').textContent = data.humMin.toFixed(1);
          
          document.getElementById('pressure').textContent = data.pressure.toFixed(1);
          document.getElementById('pressure-max').textContent = data.presMax.toFixed(1);
          document.getElementById('pressure-min').textContent = data.presMin.toFixed(1);

          // Actualizar barras
          document.getElementById('temperature-mask').style.height = (100 - Math.min(data.temp * 2, 100)) + '%';
          document.getElementById('humidity-mask').style.height = (100 - Math.min(data.hum, 100)) + '%';
          document.getElementById('pressure-mask').style.height = (100 - ((data.pressure - 850) / 2.5)) + '%';

          // Actualizar fecha y hora
          document.getElementById('datetime').textContent = data.date + ' ' + data.time;
          
          // Actualizar estado del sistema
          document.getElementById('systemStatus').textContent = data.systemEnabled ? 'SISTEMA ACTIVADO' : 'SISTEMA DESACTIVADO';
          document.getElementById('systemStatus').className = data.systemEnabled ? 'status status-enabled' : 'status status-disabled';
          document.getElementById('toggleBtn').textContent = data.systemEnabled ? 'DESACTIVAR SISTEMA' : 'ACTIVAR SISTEMA';
          document.getElementById('toggleBtn').className = data.systemEnabled ? 'btn btn-disable' : 'btn btn-enable';
          
          // Actualizar modo día/noche
          document.getElementById('lightLevel').textContent = data.lightLevel;
          document.getElementById('lightMode').textContent = data.isDayMode ? 'Día' : 'Noche';
          document.getElementById('body').className = data.isDayMode ? 'day-mode' : 'night-mode';
        })
        .catch(error => console.error('Error:', error));
    }

    function toggleSystem() {
      fetch('/toggle', { method: 'POST' })
        .then(response => response.json())
        .then(data => {
          updateData();
        });
    }

    setInterval(updateData, 2000);
    window.onload = updateData;
  </script>
</body></html>
)rawliteral";

// -------- FUNCIÓN PARA LEER SENSORES --------
void readSensors() {
  if (!systemEnabled) return;

  // Leer DHT11
  temp = dht.readTemperature();
  hum = dht.readHumidity();
  
  // Leer BMP280
  if (bmp.begin(BMP280_I2C_ADDRESS)) {
    pressure = bmp.readPressure() / 100.0; // Convertir a hPa
  }
  
  // Leer sensor de luz
  lightLevel = analogRead(LDR_PIN);
  isDayMode = (lightLevel > 2000); // Ajustar este valor según tu LDR

  // Actualizar máximos y mínimos
  if (!isnan(temp) && !isnan(hum) && !isnan(pressure)) {
    if (temp > tempMax) tempMax = temp;
    if (temp < tempMin) tempMin = temp;
    if (hum > humMax) humMax = hum;
    if (hum < humMin) humMin = hum;
    if (pressure > presMax) presMax = pressure;
    if (pressure < presMin) presMin = pressure;
  }
}

// -------- FUNCIÓN PARA ACTUALIZAR RTC --------
void updateRTC() {
  if (rtc.now().isValid()) {
    DateTime now = rtc.now();
    currentTime = String(now.hour()) + ":" + 
                 String(now.minute()) + ":" + 
                 String(now.second());
    currentDate = String(now.day()) + "/" + 
                  String(now.month()) + "/" + 
                  String(now.year());
  }
}

// -------- FUNCIÓN PARA ACTUALIZAR OLED --------
void updateOLED() {
  if (!systemEnabled) {
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("SISTEMA");
    display.println("DESACTIVADO");
    display.display();
    return;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  
  display.println("Est Meteorologica");
  display.println("-----------------");
  
  display.print("Temp: ");
  display.print(temp, 1);
  display.println(" C");
  
  display.print("Hum:  ");
  display.print(hum, 1);
  display.println(" %");
  
  display.print("Pres: ");
  display.print(pressure, 1);
  display.println(" hPa");
  
  display.print("Hora: ");
  display.println(currentTime);
  
  display.print("Modo: ");
  display.println(isDayMode ? "Dia" : "Noche");
  
  display.display();
}

// -------- MANEJADORES DEL SERVIDOR WEB --------
void handleSensorData() {
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
  json += "\"lightLevel\":" + String(lightLevel) + ",";
  json += "\"isDayMode\":" + String(isDayMode ? "true" : "false");
  json += "}";
  
  server.send(200, "application/json", json);
}

void handleToggleSystem() {
  systemEnabled = !systemEnabled;
  String json = "{\"status\":\"ok\", \"enabled\":" + String(systemEnabled ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

// -------- CONFIGURACIÓN INICIAL --------
void setup() {
  delay(1000);
  Serial.begin(115200);
  Serial.println("\nIniciando Estación Meteorológica ESP32...");

  // Inicializar I2C
  Wire.begin();

  // Inicializar DHT11
  dht.begin();

  // Inicializar OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Error al inicializar OLED");
  } else {
    Serial.println("OLED inicializado correctamente");
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
  }

  // Inicializar RTC
  if (!rtc.begin()) {
    Serial.println("Error al inicializar RTC");
  } else {
    Serial.println("RTC inicializado correctamente");
    if (!rtc.isrunning()) {
      Serial.println("RTC no está funcionando, estableciendo hora...");
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
  }

  // Inicializar BMP280
  if (!bmp.begin(BMP280_I2C_ADDRESS)) {
    Serial.println("Error al inicializar BMP280");
  } else {
    Serial.println("BMP280 inicializado correctamente");
    bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,
                    Adafruit_BMP280::SAMPLING_X2,
                    Adafruit_BMP280::SAMPLING_X16,
                    Adafruit_BMP280::FILTER_X16,
                    Adafruit_BMP280::STANDBY_MS_500);
  }

  // Configurar pin LDR
  pinMode(LDR_PIN, INPUT);

  // Conectar WiFi
  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectado a WiFi");
  Serial.print("📶 IP asignada: ");
  Serial.println(WiFi.localIP());

  // Configurar rutas del servidor web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/sensors", HTTP_GET, handleSensorData);
  server.on("/toggle", HTTP_POST, handleToggleSystem);

  server.begin();
  Serial.println("🌐 Servidor web iniciado");

  // Mostrar IP en OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Conectado WiFi:");
  display.println(WiFi.localIP().toString());
  display.display();
  delay(2000);
}

// -------- BUCLE PRINCIPAL --------
void loop() {
  server.handleClient();
  
  readSensors();
  updateRTC();
  updateOLED();
  
  delay(1000);
}