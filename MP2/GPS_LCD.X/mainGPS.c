#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configGPS.h"
#include "LCDGPS.h"

#define _XTAL_FREQ 8000000

// ---------- UART CONFIGURACIÓN MEJORADA ----------
void UART_Init(long baudrate) {
    // CONFIGURACIÓN CORRECTA DE PINES
    TRISCbits.TRISC6 = 1;  // TX pin como entrada (¡IMPORTANTE!)
    TRISCbits.TRISC7 = 1;  // RX pin como entrada
    
    // Cálculo mejorado del baud rate
    SPBRG = (unsigned char)((_XTAL_FREQ / (64UL * baudrate)) - 1);
    
    TXSTAbits.SYNC = 0;    // Modo asíncrono
    TXSTAbits.BRGH = 0;    // Low speed
    RCSTAbits.SPEN = 1;    // Habilitar puerto serial
    TXSTAbits.TXEN = 0;    // Solo recepción
    RCSTAbits.CREN = 1;    // Habilitar recepción continua
    
    // Limpiar flags
    PIR1bits.RCIF = 0;
}

unsigned char UART_Available(void) {
    return PIR1bits.RCIF;
}

char UART_Read(void) {
    if (RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        __delay_us(10);
        RCSTAbits.CREN = 1;
    }
    return RCREG;
}

// ---------- CONVERSIÓN MEJORADA ----------
float ConvertToDecimal(const char *value) {
    if (value == NULL || strlen(value) < 4) return 0.0;
    
    // Verificar que el string contenga solo números y punto
    int valid = 1;
    int dot_count = 0;
    for(int i = 0; value[i] != '\0'; i++) {
        if(value[i] == '.') dot_count++;
        else if(value[i] < '0' || value[i] > '9') valid = 0;
    }
    if(!valid || dot_count > 1) return 0.0;
    
    float val = atof(value);
    int grados = (int)(val / 100);
    float minutos = val - (grados * 100.0);
    return grados + (minutos / 60.0);
}

// ---------- VARIABLES GLOBALES ----------
char nmea_buffer[256];
char lat_raw[16] = "", lon_raw[16] = "", alt_raw[16] = "", time_raw[16] = "";
char ns = 'N', ew = 'E';
float lat = 0.0, lon = 0.0, alt = 0.0;
unsigned long time_utc = 0;

// ---------- PARSING MEJORADO DE GPGGA ----------
unsigned char ParseGPGGA(char *sentence) {
    // Verificar que es una sentencia GPGGA
    if (strstr(sentence, "$GPGGA") == NULL) {
        return 0;
    }
    
    // Reiniciar variables
    strcpy(lat_raw, "");
    strcpy(lon_raw, "");
    strcpy(alt_raw, "");
    strcpy(time_raw, "");
    ns = 'N';
    ew = 'E';
    
    // Hacer copia para no dañar el original
    char temp[256];
    strcpy(temp, sentence);
    
    char *token;
    int field = 0;
    
    token = strtok(temp, ",");
    
    while (token != NULL && field < 15) {
        field++;
        
        switch (field) {
            case 1:  // $GPGGA
                break;
            case 2:  // Tiempo UTC
                if(strlen(token) >= 6) strcpy(time_raw, token);
                break;
            case 3:  // Latitud
                if(strlen(token) >= 4) strcpy(lat_raw, token);
                break;
            case 4:  // N/S
                if(strlen(token) == 1) ns = token[0];
                break;
            case 5:  // Longitud
                if(strlen(token) >= 4) strcpy(lon_raw, token);
                break;
            case 6:  // E/W
                if(strlen(token) == 1) ew = token[0];
                break;
            case 7:  // Fix quality
                break;
            case 8:  // Número de satélites
                break;
            case 9:  // HDOP
                break;
            case 10: // Altitud
                if(strlen(token) > 0) strcpy(alt_raw, token);
                break;
        }
        
        token = strtok(NULL, ",");
    }
    
    // Verificar que tenemos datos mínimos
    if (strlen(lat_raw) < 4 || strlen(lon_raw) < 4) {
        return 0;
    }
    
    // Convertir coordenadas
    lat = ConvertToDecimal(lat_raw);
    lon = ConvertToDecimal(lon_raw);
    alt = atof(alt_raw);
    time_utc = atol(time_raw);
    
    // Aplicar dirección
    if (ns == 'S') lat = -lat;
    if (ew == 'W') lon = -lon;
    
    return 1;
}

// ---------- FUNCIÓN PARA MOSTRAR DATOS EN LCD ----------
void DisplayGPSData(void) {
    char buffer[17];
    static unsigned char screen = 0;
    
    if(screen == 0) {
        // Pantalla 1: Coordenadas
        LCD_Clear();
        
        // Línea 1: Latitud
        if(lat >= 0)
            sprintf(buffer, "Lat:%9.5fN", lat);
        else
            sprintf(buffer, "Lat:%9.5fS", -lat);
        LCD_String_xy(0, 0, buffer);
        
        // Línea 2: Longitud
        if(lon >= 0)
            sprintf(buffer, "Lon:%9.5fE", lon);
        else
            sprintf(buffer, "Lon:%9.5fW", -lon);
        LCD_String_xy(1, 0, buffer);
        
        screen = 1;
    } else {
        // Pantalla 2: Altitud y Tiempo
        LCD_Clear();
        
        // Línea 1: Altitud
        sprintf(buffer, "Alt:%8.1f m", alt);
        LCD_String_xy(0, 0, buffer);
        
        // Línea 2: Tiempo UTC (si está disponible)
        if(time_utc > 0) {
            unsigned int hours = time_utc / 10000;
            unsigned int minutes = (time_utc % 10000) / 100;
            unsigned int seconds = time_utc % 100;
            sprintf(buffer, "UTC %02d:%02d:%02d", hours, minutes, seconds);
        } else {
            sprintf(buffer, "UTC --:--:--");
        }
        LCD_String_xy(1, 0, buffer);
        
        screen = 0;
    }
}

// ---------- FUNCIÓN PRINCIPAL ----------
void main(void) {
    // Configurar oscilador interno a 8MHz
    OSCCONbits.IRCF = 0b111;
    
    // Configurar todos los pines como digitales
    ADCON1 = 0x0F;
    CMCON = 0x07;
    
    // Inicializar periféricos
    LCD_Init();
    UART_Init(9600);
    
    // Mensaje inicial
    LCD_Clear();
    LCD_String_xy(0, 0, "  GPS NEO-6M");
    LCD_String_xy(1, 0, "  Inicializando");
    __delay_ms(2000);
    
    LCD_Clear();
    LCD_String_xy(0, 0, "Buscando");
    LCD_String_xy(1, 0, "senal GPS...");
    
    // Variables para recepción
    unsigned int index = 0;
    char received_char;
    unsigned char sentence_started = 0;
    unsigned char valid_data = 0;
    unsigned long timeout_counter = 0;
    unsigned long display_timer = 0;
    
    while(1) {
        if(UART_Available()) {
            received_char = UART_Read();
            
            if(received_char == '$') {
                // Inicio de nueva sentencia NMEA
                index = 0;
                sentence_started = 1;
                nmea_buffer[index++] = received_char;
            }
            else if(sentence_started) {
                if(received_char == '\n') {
                    // Fin de sentencia
                    nmea_buffer[index] = '\0';
                    
                    // Procesar sentencia GPGGA
                    if(strstr(nmea_buffer, "$GPGGA") != NULL) {
                        valid_data = ParseGPGGA(nmea_buffer);
                        
                        if(valid_data) {
                            // Mostrar datos en LCD
                            DisplayGPSData();
                            display_timer = 0;
                        }
                    }
                    
                    sentence_started = 0;
                    index = 0;
                    timeout_counter = 0;
                }
                else if(index < sizeof(nmea_buffer) - 1) {
                    // Almacenar carácter en buffer
                    nmea_buffer[index++] = received_char;
                }
            }
        }
        else {
            __delay_ms(1);
            timeout_counter++;
            display_timer++;
            
            // Timeout después de 10 segundos sin datos
            if(timeout_counter > 10000) {
                LCD_Clear();
                LCD_String_xy(0, 0, "  SIN SEÑAL");
                LCD_String_xy(1, 0, "Verificar GPS");
                timeout_counter = 0;
            }
            
            // Cambiar pantalla cada 5 segundos si hay datos
            if(valid_data && display_timer > 5000) {
                DisplayGPSData();
                display_timer = 0;
            }
        }
    }
}