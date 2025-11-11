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
void Display_Suspended(void);
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

uint8_t system_suspended = 0;
uint8_t last_button_state = 1; // Estado anterior del bot?n (pull-up)

void main(void) {
    System_Init();
    
    // Prueba simple de la OLED
    Test_OLED();
    __delay_ms(2000);
    
    // Leer sensores y mostrar datos iniciales
    Read_Sensors();
    Display_Time_Humidity();
    
    uint8_t screen_mode = 0; // 0 = Tiempo/Humedad, 1 = GPS
    uint16_t update_counter = 0;
    uint16_t display_counter = 0;
    uint16_t gps_read_counter = 0;
    uint8_t gps_updated = 0;
    
    while(1) {
        // ===== CONTROL DEL BOT?N TOUCH =====
        uint8_t current_button_state = PORTBbits.RB2; // Leer estado del bot?n
        
        // Detectar flanco descendente (bot?n presionado)
        if (last_button_state == 1 && current_button_state == 0) {
            __delay_ms(50); // Debounce
            if (PORTBbits.RB2 == 0) { // Confirmar que sigue presionado
                system_suspended = !system_suspended; // Cambiar estado
                
                if (system_suspended) {
                    Display_Suspended();
                } else {
                    // Al reactivar, mostrar pantalla actual
                    if (screen_mode == 0) {
                        Display_Time_Humidity();
                    } else {
                        Display_GPS();
                    }
                }
            }
            __delay_ms(300); // Evitar m?ltiples detecciones
        }
        last_button_state = current_button_state;
        
        // Si el sistema est? suspendido, solo verificar el bot?n
        if (system_suspended) {
            __delay_ms(10);
            continue;
        }
        
        // ===== LECTURA CONTINUA DEL GPS =====
        gps_read_counter++;
        if (gps_read_counter >= 5) { // Leer GPS cada 50ms
            if(UART_Available()) {
                char buffer[100];
                uint8_t i = 0;
                char c;
                
                // Leer una l?nea completa del GPS
                while(i < sizeof(buffer)-1) {
                    if(UART_Available()) {
                        c = UART_Read();
                        if(c == '\n') break;
                        if(c != '\r') {
                            buffer[i++] = c;
                        }
                    } else {
                        // Peque?a pausa para esperar m?s datos
                        __delay_us(100);
                    }
                }
                buffer[i] = '\0';
                
                // Procesar datos GPS si la l?nea no est? vac?a
                if(i > 6) { // M?nimo 6 caracteres para ser una trama v?lida
                    gps_updated = GPS_ParseData(buffer, gps_lat, gps_lon, gps_alt);
                }
            }
            gps_read_counter = 0;
        }
        
        // ===== ACTUALIZACI?N DE SENSORES =====
        update_counter++;
        if(update_counter >= 200) { // 200 * 10ms = 2 segundos
            Read_Sensors();
            update_counter = 0;
            
            // Si estamos en pantalla de tiempo/humedad, actualizar display
            if(screen_mode == 0) {
                Display_Time_Humidity();
            }
        }
        
        // ===== CAMBIO DE PANTALLA =====
        display_counter++;
        if(display_counter >= 800) { // 800 * 10ms = 8 segundos
            screen_mode = !screen_mode;
            display_counter = 0;
            
            if(screen_mode == 0) {
                Display_Time_Humidity();
            } else {
                Display_GPS();
            }
        }
        
        // ===== ACTUALIZACI?N DE PANTALLA GPS =====
        if(screen_mode == 1 && gps_updated) {
            Display_GPS();
            gps_updated = 0; // Reset flag
        }
        
        __delay_ms(10); // Delay principal de 10ms
    }
}

void System_Init(void) {
    // Oscilador interno a 8 MHz
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
    
    // Configurar pines
    ADCON1 = 0x0F;
    CMCON = 0x07;
    
    TRISAbits.TRISA0 = 1;    // RA0 anal?gico
    TRISCbits.TRISC7 = 1;    // RC7 RX GPS
    TRISCbits.TRISC6 = 0;    // RC6 TX
    TRISBbits.TRISB2 = 1;    // RB2 como entrada para bot?n touch
    
    // Configurar pull-up para el bot?n (si tu PIC lo soporta)
    // Si no, usa resistencia pull-up externa de 10k a VCC
    INTCON2bits.RBPU = 0;    // Habilitar pull-ups internos (si disponibles)
    
    // Inicializar perif?ricos
    ADC_Init();
    UART_Init(9600);
    I2C_Init();
    OLED_Init();
    DS1307_Init();
    
    // Configurar RTC - Solo si no est? funcionando
    if (!DS1307_IsRunning()) {
        DS1307_SetTime(12, 2, 0);  // 12:02:00
        DS1307_SetDate(15, 1, 25); // 15 de enero 2025
    }
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
    
    // Limpiar buffer de recepci?n
    while(RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
}

char UART_Read(void) {
    while(!PIR1bits.RCIF);
    return RCREG;
}

uint8_t UART_Available(void) {
    // Verificar si hay datos disponibles y si no hay error de overrun
    if(RCSTAbits.OERR) {
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
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
    
    // Convertir a porcentaje (f?rmula actual)
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
            case 2: // Latitud
                if(token != NULL && strlen(token) > 2) { // M?nimo 3 caracteres
                    strncpy(lat, token, 9);
                    lat[9] = '\0';
                } else {
                    strcpy(lat, "---");
                }
                break;
                
            case 3: // Direcci?n latitud (N/S)
                if(token != NULL && strlen(token) > 0 && lat[0] != '-') {
                    strncat(lat, token, 1);
                }
                break;
                
            case 4: // Longitud
                if(token != NULL && strlen(token) > 2) { // M?nimo 3 caracteres
                    strncpy(lon, token, 9);
                    lon[9] = '\0';
                } else {
                    strcpy(lon, "---");
                }
                break;
                
            case 5: // Direcci?n longitud (E/W)
                if(token != NULL && strlen(token) > 0 && lon[0] != '-') {
                    strncat(lon, token, 1);
                }
                break;
                
            case 6: // Indicador de calidad GPS (0=sin fix, 1=GPS fix, 2=DGPS fix)
                if(token != NULL && token[0] != '\0') {
                    gps_fix = (token[0] == '1' || token[0] == '2');
                } else {
                    gps_fix = 0;
                }
                break;
                
            case 7: // N?mero de sat?lites
                if(token != NULL && token[0] != '\0') {
                    gps_satellites = atoi(token);
                } else {
                    gps_satellites = 0;
                }
                break;
                
            case 9: // Altitud
                if(token != NULL && strlen(token) > 0) {
                    strncpy(alt, token, 6);
                    strcat(alt, "m");
                } else {
                    strcpy(alt, "---");
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
        OLED_PrintText(40, 4, "ACTIVO");
    } else {
        OLED_PrintText(25, 4, "NO");
        OLED_PrintText(40, 4, "BUSCANDO");
    }
    
    // Mostrar n?mero de sat?lites
    OLED_PrintText(0, 5, "SAT:");
    sprintf(buffer, "%d", gps_satellites);
    OLED_PrintText(25, 5, buffer);
    
    OLED_PrintText(0, 6, "ACTUALIZANDO...");
}

void Display_Suspended(void) {
    OLED_Clear();
    OLED_PrintText(25, 2, "SISTEMA");
    OLED_PrintText(15, 4, "SUSPENDIDO");
    OLED_PrintText(10, 6, "PRESIONE BOTON");
}

void Test_OLED(void) {
    OLED_Clear();
    OLED_PrintText(40, 2, "SISTEMA");
    OLED_PrintText(25, 4, "INICIANDO...");
}