#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>
#include <Adafruit_BMP280.h>

//CONFIGURACIÓN WiFi 
const char* ssid = "Yo";
const char* password = "1234567890";

// CONFIGURACIÓN DE PINES 
#define DHTPIN 8
#define LDR_PIN 9
#define I2C_SDA 6
#define I2C_SCL 5

//  CONFIGURACIÓN DHT 
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//  CONFIGURACIÓN OLED 
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//  CONFIGURACIÓN RTC 
RTC_DS1307 rtc;

//  CONFIGURACIÓN BMP280 
Adafruit_BMP280 bmp;
#define BMP280_I2C_ADDRESS 0x76

//  SERVIDOR WEB 
WebServer server(80);

// VARIABLES GLOBALES 
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
bool autoMode = true;
String dayPeriod = "Día";

// Filtrado del LDR
const int LDR_SAMPLES = 10;
int ldrReadings[LDR_SAMPLES];
int ldrIndex = 0;
unsigned long lastLDRUpdate = 0;
const int LDR_UPDATE_INTERVAL = 100;

// Íconos para OLED
const uint8_t thermometer_icon[] PROGMEM = {0b00000100,0b00001010,0b00001010,0b00001010,0b00001110,0b00011111,0b00011111,0b00011111};
const uint8_t humidity_icon[] PROGMEM = {0b00000100,0b00000100,0b00001010,0b00001010,0b00010001,0b00010001,0b00001010,0b00000100};
const uint8_t pressure_icon[] PROGMEM = {0b00011111,0b00011111,0b00011111,0b00000000,0b00000000,0b00011111,0b00011111,0b00011111};
const uint8_t light_icon[] PROGMEM = {0b00000100,0b00010101,0b00001110,0b00011111,0b00001110,0b00010101,0b00000100,0b00000000};

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

// ===== DETERMINAR PERIODO DEL DÍA 
// Modo automático basado en niveles de luz
void determineDayPeriod() {

    if (autoMode) {
        // Límites calibrados: hasta 2800 es luz clara
        if (stableLightLevel < 100) {
            dayPeriod = "Luz Extrema";
            isDayMode = true;
        }
        else if (stableLightLevel < 500) {
            dayPeriod = "Día Brillante";
            isDayMode = true;
        }
        else if (stableLightLevel < 1500) {
            dayPeriod = "Día";
            isDayMode = true;
        }
        else if (stableLightLevel < 2800) {
            dayPeriod = "Atardecer";
            isDayMode = true;
        }
        else if (stableLightLevel < 3500) {
            dayPeriod = "Anochecer";
            isDayMode = false;
        }
        else {
            dayPeriod = "Noche";
            isDayMode = false;
        }
    }
    else {
        // Modo manual → solo respeta el estado elegido
        dayPeriod = isDayMode ? "Día (Manual)" : "Noche (Manual)";
    }
}

void readDHT11() {
    float t = dht.readTemperature();
    float h = dht.readHumidity();

    if (isnan(t) || isnan(h)) {
        Serial.println("⚠ Error leyendo DHT11");
        return;
    }

    dhtTemperature = t;   // °C
    dhtHumidity    = h;   // %
}

// ===== LECTURA DEL BMP280 =====
void readBMP280() {
    float p = bmp.readPressure();      // Pascales
    float tempBMP = bmp.readTemperature(); // °C

    if (isnan(p) || isnan(tempBMP)) {
        Serial.println("⚠ Error leyendo BMP280");
        return;
    }

    pressure     = p / 100.0;     // Convertir Pa → hPa
    bmpTemperature = tempBMP;
}

void updateOLED() {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    // ===== HORA ==========================
    display.setCursor(0, 0);
    display.print("Hora: ");
    if (rtcHour < 10) display.print("0");
    display.print(rtcHour);
    display.print(":");
    if (rtcMinute < 10) display.print("0");
    display.print(rtcMinute);

    // ===== TEMPERATURA ===================
    display.setCursor(0, 12);
    display.print("Temp: ");
    display.print(dhtTemperature);
    display.print(" C");

    // ===== HUMEDAD ========================
    display.setCursor(0, 24);
    display.print("Humed: ");
    display.print(dhtHumidity);
    display.print(" %");

    // ===== PRESIÓN ATMOSFÉRICA ============
    display.setCursor(0, 36);
    display.print("Pres: ");
    display.print(pressure);
    display.print(" hPa");

    // ===== LUZ AMBIENTAL ==================
    display.setCursor(0, 48);
    display.print("Luz: ");
    display.print(ldrValue);

    // Dibujar todo en pantalla
    display.display();
}

unsigned long previousMillis = 0;
const unsigned long interval = 1000;   // Leer todo cada 1 segundo

void loop() {

    unsigned long currentMillis = millis();

    // Leer sensores y actualizar display cada 1 segundo
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        // ===== LEER RTC =====
        getTimeDS1307();

        // ===== LEER DHT11 =====
        readDHT11();

        // ===== LEER BMP280 =====
        readBMP280();

        // ===== LEER LDR =====
        readLDR();

        // ===== ACTUALIZAR PANTALLA =====
        updateOLED();

        // ===== IMPRIMIR TODO POR SERIAL =====
        Serial.print("Hora: ");
        Serial.print(rtcHour); Serial.print(":");
        Serial.print(rtcMinute); Serial.print(":");
        Serial.println(rtcSecond);

        Serial.print("Temp: "); Serial.print(dhtTemperature); Serial.println(" C");
        Serial.print("Humedad: "); Serial.print(dhtHumidity); Serial.println(" %");
        Serial.print("Presion: "); Serial.print(pressure); Serial.println(" hPa");
        Serial.print("Luz LDR: "); Serial.println(ldrValue);
        Serial.println("----------------------------");
    }
}