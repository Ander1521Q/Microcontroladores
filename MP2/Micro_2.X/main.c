#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "i2c.h"
#include "ssd1306.h"
#include "ds1307.h"

// Prototipos de funciones
void System_Init(void);
void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);
void UART_Init(long baud);
char UART_Read(void);
uint8_t UART_Available(void);
void Read_Sensors(void);
void Display_Time_Humidity(void);
void Display_GPS(void);
void Test_OLED(void);
uint8_t GPS_ParseData(char *buffer, char *lat, char *lon, char *alt);

// Variables globales
uint8_t hours = 12, minutes = 0, seconds = 0;
uint8_t day = 5, month = 11, year = 25;
uint16_t humidity_raw = 0;
float humidity_percent = 0.0;

char gps_lat[12] = "---";
char gps_lon[12] = "---";
char gps_alt[8] = "---";
uint8_t gps_fix = 0;
uint8_t gps_satellites = 0;

void main(void) {
    System_Init();
    
    // Prueba simple de la OLED
    Test_OLED();
    __delay_ms(2000);
    
    // Leer sensores y mostrar datos
    Read_Sensors();
    Display_Time_Humidity();
    
    uint8_t screen_mode = 0;
    uint32_t counter = 0;
    uint32_t gps_read_counter = 0;
    
    while(1) {
        counter++;
        gps_read_counter++;
        
        // Leer sensores cada 2 segundos
        if(counter % 200 == 0) {
            Read_Sensors();
        }
        
        // Leer GPS más frecuentemente (cada 500ms)
        if(gps_read_counter % 50 == 0) {
            // Leer datos GPS si están disponibles
            if(UART_Available()) {
                char buffer[100];
                uint8_t i = 0;
                char c;
                
                // Leer una línea completa del GPS
                while(i < sizeof(buffer)-1) {
                    if(UART_Available()) {
                        c = UART_Read();
                        if(c == '\n') break;
                        if(c != '\r') {
                            buffer[i++] = c;
                        }
                    }
                }
                buffer[i] = '\0';
                
                // Procesar datos GPS
                if(strlen(buffer) > 0) {
                    GPS_ParseData(buffer, gps_lat, gps_lon, gps_alt);
                }
            }
        }
        
        // Cambiar pantalla cada 9 segundos
        if(counter % 900 == 0) {
            screen_mode = !screen_mode;
            if(screen_mode == 0) {
                Display_Time_Humidity();
            } else {
                Display_GPS();
            }
        }
        
        __delay_ms(10);
        
        // Reset counter para evitar overflow
        if(counter >= 60000) counter = 0;
        if(gps_read_counter >= 60000) gps_read_counter = 0;
    }
}

void System_Init(void) {
    // Oscilador interno a 8 MHz
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
    
    // Configurar pines
    ADCON1 = 0x0F;
    CMCON = 0x07;
    
    TRISAbits.TRISA0 = 1;    // RA0 analógico
    TRISCbits.TRISC7 = 1;    // RC7 RX GPS
    TRISCbits.TRISC6 = 0;    // RC6 TX
    
    // Inicializar periféricos
    ADC_Init();
    UART_Init(9600);
    I2C_Init();
    OLED_Init();
    DS1307_Init();
    
    // Configurar RTC - Solo si no está funcionando
    if (!DS1307_IsRunning()) {
        // Aquí puedes ajustar la hora inicial con 2 minutos de adelanto
        // Por ejemplo, si quieres que empiece en 12:02:00
        DS1307_SetTime(19, 45, 0);  // 12:02:00
        DS1307_SetDate(10, 11, 25); // 15 de enero 2025
    }
    // Si el RTC ya está funcionando, usará la hora que ya tiene
}

void ADC_Init(void) {
    ADCON0 = 0x01;
    ADCON1 = 0x0E;
    ADCON2 = 0xBE;
}

uint16_t ADC_Read(uint8_t channel) {
    ADCON0bits.CHS = channel;
    __delay_us(50);
    ADCON0bits.GO = 1;
    while(ADCON0bits.GO);
    return ((uint16_t)(ADRESH << 8) | ADRESL);
}

void UART_Init(long baud) {
    TRISCbits.TRISC7 = 1;
    TRISCbits.TRISC6 = 0;
    
    SPBRG = (uint8_t)((_XTAL_FREQ / (64UL * (unsigned long)baud)) - 1);
    TXSTAbits.BRGH = 0;
    TXSTAbits.SYNC = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

char UART_Read(void) {
    while(!PIR1bits.RCIF);
    return RCREG;
}

uint8_t UART_Available(void) {
    return PIR1bits.RCIF;
}

void Read_Sensors(void) {
    // Leer humedad con promediado
    static uint16_t adc_samples[3] = {0};
    static uint8_t sample_index = 0;
    uint32_t adc_sum = 0;
    
    adc_samples[sample_index] = ADC_Read(0);
    sample_index = (sample_index + 1) % 3;
    
    for(uint8_t i = 0; i < 3; i++) {
        adc_sum += adc_samples[i];
    }
    humidity_raw = (uint16_t)(adc_sum / 3);
    
    // Convertir a porcentaje
    humidity_percent = (1023.0 - (float)humidity_raw) / 10.23;
    if(humidity_percent < 0) humidity_percent = 0;
    if(humidity_percent > 100) humidity_percent = 100;
    
    // Leer RTC - siempre usa la hora actual del RTC
    DS1307_GetTime(&hours, &minutes, &seconds);
    DS1307_GetDate(&day, &month, &year);
}

uint8_t GPS_ParseData(char *buffer, char *lat, char *lon, char *alt) {
    char *token;
    uint8_t field = 0;
    uint8_t valid_fix = 0;
    
    // Verificar si es trama GGA
    if(strstr(buffer, "$GPGGA") == NULL) {
        return 0;
    }
    
    // Copiar buffer para no modificar el original
    char gps_buffer[100];
    strncpy(gps_buffer, buffer, sizeof(gps_buffer)-1);
    gps_buffer[sizeof(gps_buffer)-1] = '\0';
    
    token = strtok(gps_buffer, ",");
    
    while(token != NULL && field < 15) {
        field++;
        token = strtok(NULL, ",");
        
        switch(field) {
            case 2: // Hora UTC
                // Podríamos usar esto para sincronizar el RTC si queremos
                break;
                
            case 3: // Latitud
                if(token != NULL && strlen(token) > 0) {
                    strncpy(lat, token, 10);
                    lat[10] = '\0';
                }
                break;
                
            case 4: // Dirección latitud (N/S)
                if(token != NULL && strlen(token) > 0 && lat[0] != '\0') {
                    strncat(lat, token, 1);
                }
                break;
                
            case 5: // Longitud
                if(token != NULL && strlen(token) > 0) {
                    strncpy(lon, token, 10);
                    lon[10] = '\0';
                }
                break;
                
            case 6: // Dirección longitud (E/W)
                if(token != NULL && strlen(token) > 0 && lon[0] != '\0') {
                    strncat(lon, token, 1);
                }
                break;
                
            case 7: // Indicador de calidad GPS
                if(token != NULL && token[0] != '\0') {
                    gps_fix = (token[0] >= '1'); // 0=sin fix, 1=GPS fix, 2=DGPS fix
                }
                break;
                
            case 8: // Número de satélites
                if(token != NULL && token[0] != '\0') {
                    gps_satellites = atoi(token);
                }
                break;
                
            case 9: // Altitud
                if(token != NULL && strlen(token) > 0) {
                    strncpy(alt, token, 6);
                    strcat(alt, "m");
                }
                break;
        }
    }
    
    return gps_fix;
}

void Display_Time_Humidity(void) {
    char buffer[20];
    
    OLED_Clear();
    
    // Mostrar fecha
    OLED_PrintText(10, 0, "FECHA:");
    sprintf(buffer, "%02d/%02d/20%02d", day, month, year);
    OLED_PrintText(50, 0, buffer);
    
    // Mostrar hora
    OLED_PrintText(10, 2, "HORA:");
    sprintf(buffer, "%02d:%02d:%02d", hours, minutes, seconds);
    OLED_PrintText(50, 2, buffer);
    
    // Mostrar humedad
    OLED_PrintText(10, 4, "HUMEDAD:");
    sprintf(buffer, "%.1f%%", humidity_percent);
    OLED_PrintText(70, 4, buffer);
    
    // Mostrar valor RAW
    OLED_PrintText(10, 6, "ADC:");
    sprintf(buffer, "%d", humidity_raw);
    OLED_PrintText(50, 6, buffer);
}

void Display_GPS(void) {
    char buffer[20];
    
    OLED_Clear();
    
    OLED_PrintText(10, 0, "DATOS GPS");
    
    // Mostrar latitud
    OLED_PrintText(0, 1, "LAT:");
    OLED_PrintText(25, 1, gps_lat);
    
    // Mostrar longitud
    OLED_PrintText(0, 2, "LON:");
    OLED_PrintText(25, 2, gps_lon);
    
    // Mostrar altitud
    OLED_PrintText(0, 3, "ALT:");
    OLED_PrintText(25, 3, gps_alt);
    
    // Mostrar estado del GPS
    OLED_PrintText(0, 4, "FIX:");
    if(gps_fix) {
        OLED_PrintText(25, 4, "SI");
    } else {
        OLED_PrintText(25, 4, "NO");
    }
    
    // Mostrar número de satélites
    OLED_PrintText(0, 5, "SAT:");
    sprintf(buffer, "%d", gps_satellites);
    OLED_PrintText(25, 5, buffer);
    
    OLED_PrintText(0, 6, "ACTUALIZANDO...");
}

void Test_OLED(void) {
    OLED_Clear();
    OLED_PrintText(40, 2, "SISTEMA");
    OLED_PrintText(25, 4, "INICIANDO...");
}