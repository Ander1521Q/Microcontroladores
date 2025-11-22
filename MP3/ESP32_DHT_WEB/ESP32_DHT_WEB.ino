// Incluye la biblioteca para manejar WiFi en el ESP32
#include <WiFi.h>

// Incluye la biblioteca para crear un servidor web
#include <WebServer.h>

// Incluye la biblioteca para manejar el sensor DHT
#include <DHT.h>

// -------- CONFIGURACIÓN DE LA RED WiFi --------

// Nombre de la red WiFi (SSID)
const char* ssid = "Yo";

// Contraseña de la red WiFi
const char* password = "1234567890";

// -------- CONFIGURACIÓN DEL SENSOR DHT --------

// Define el pin digital al que está conectado el sensor DHT
#define DHTPIN 4

// Define el tipo de sensor DHT usado (DHT11)
#define DHTTYPE DHT11

// Crea un objeto "dht" para interactuar con el sensor
DHT dht(DHTPIN, DHTTYPE);

// -------- CREACIÓN DEL SERVIDOR WEB EN PUERTO 80 --------
WebServer server(80);

// -------- VARIABLES PARA GUARDAR MÁXIMOS Y MÍNIMOS --------
float tempMax = -1000, tempMin = 1000;
float humMax  = -1000, humMin  = 1000;
float temp = 0, hum = 0;

// -------- CONTENIDO HTML --------
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    html { font-family: Arial; text-align: center; background-color: #f2f2f2; }
    h2 { font-size: 2.2rem; margin-bottom: 30px; }
    .container { display: flex; justify-content: center; gap: 40px; flex-wrap: wrap; }
    .thermo-block { display: flex; flex-direction: column; align-items: center; margin-bottom: 30px; }
    .bar-wrapper { display: flex; flex-direction: row; align-items: flex-end; gap: 8px; }
    .bar-container {
      position: relative; width: 60px; height: 250px;
      background: linear-gradient(to top, #0000ff, #00ffff, #00ff00, #ffff00, #ff0000);
      border-radius: 30px; overflow: hidden; border: 2px solid #999;
    }
    .bar-mask {
      position: absolute; top: 0; width: 100%;
      background: rgba(0, 0, 0, 0.5);
      transition: height 0.5s; z-index: 1;
    }
    .scale {
      display: flex; flex-direction: column; justify-content: space-between;
      height: 250px; font-size: 0.9rem;
    }
    .label { margin-top: 10px; font-size: 1.2rem; font-weight: bold; }
    .value { font-size: 1.4rem; margin-top: 5px; }
    .minmax { font-size: 1rem; margin-top: 4px; color: #333; }
  </style>
</head>
<body>
  <h2>ESP32 Monitor de Temperatura y Humedad</h2>

  <div class="container">
    <!-- Termómetro de Temperatura -->
    <div class="thermo-block">
      <div class="bar-wrapper">
        <div class="bar-container"><div class="bar-mask" id="temperature-mask" style="height: 100%;"></div></div>
        <div class="scale"><div>100</div><div>80</div><div>60</div><div>40</div><div>20</div><div>0</div></div>
      </div>
      <div class="label">🌡 Temperatura</div>
      <div class="value"><span id="temperature">--</span> °C</div>
      <div class="minmax">Máx: <span id="temperature-max">--</span> °C | Mín: <span id="temperature-min">--</span> °C</div>
    </div>

    <!-- Termómetro de Humedad -->
    <div class="thermo-block">
      <div class="bar-wrapper">
        <div class="bar-container"><div class="bar-mask" id="humidity-mask" style="height: 100%;"></div></div>
        <div class="scale"><div>100</div><div>80</div><div>60</div><div>40</div><div>20</div><div>0</div></div>
      </div>
      <div class="label">💧 Humedad</div>
      <div class="value"><span id="humidity">--</span> %</div>
      <div class="minmax">Máx: <span id="humidity-max">--</span> % | Mín: <span id="humidity-min">--</span> %</div>
    </div>
  </div>

<script>
function updateData() {
  fetch('/sensors')
    .then(response => response.json())
    .then(data => {
      document.getElementById('temperature').textContent = data.temp.toFixed(1);
      document.getElementById('temperature-max').textContent = data.tempMax.toFixed(1);
      document.getElementById('temperature-min').textContent = data.tempMin.toFixed(1);
      document.getElementById('humidity').textContent = data.hum.toFixed(1);
      document.getElementById('humidity-max').textContent = data.humMax.toFixed(1);
      document.getElementById('humidity-min').textContent = data.humMin.toFixed(1);

      document.getElementById('temperature-mask').style.height = (100 - Math.min(data.temp, 100)) + '%';
      document.getElementById('humidity-mask').style.height = (100 - Math.min(data.hum, 100)) + '%';
    });
}
setInterval(updateData, 2000);
window.onload = updateData;
</script>
</body>
</html>
)rawliteral";

// -------- FUNCIÓN PARA ENVIAR DATOS JSON --------
void handleSensorData() {
  temp = dht.readTemperature();
  hum = dht.readHumidity();

  if (!isnan(temp) && !isnan(hum)) {
    if (temp > tempMax) tempMax = temp;
    if (temp < tempMin) tempMin = temp;
    if (hum > humMax) humMax = hum;
    if (hum < humMin) humMin = hum;
  }

  String json = "{";
  json += "\"temp\":" + String(temp, 1) + ",";
  json += "\"tempMax\":" + String(tempMax, 1) + ",";
  json += "\"tempMin\":" + String(tempMin, 1) + ",";
  json += "\"hum\":" + String(hum, 1) + ",";
  json += "\"humMax\":" + String(humMax, 1) + ",";
  json += "\"humMin\":" + String(humMin, 1);
  json += "}";

  server.send(200, "application/json", json);
}

// -------- CONFIGURACIÓN INICIAL --------
void setup() {
  delay(1000); // Espera un poco antes de iniciar
  Serial.begin(115200);
  Serial.println("\nIniciando ESP32...");

  dht.begin();

  WiFi.begin(ssid, password);
  Serial.print("Conectando a WiFi");

  // Espera hasta conectar
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\n✅ Conectado a WiFi");
  Serial.print("📶 IP asignada: ");
  Serial.println(WiFi.localIP());

  // Rutas del servidor web
  server.on("/", HTTP_GET, []() {
    server.send_P(200, "text/html", index_html);
  });
  server.on("/sensors", HTTP_GET, handleSensorData);

  server.begin();
  Serial.println("🌐 Servidor web iniciado");
}

// -------- BUCLE PRINCIPAL --------
void loop() {
  server.handleClient();
}
