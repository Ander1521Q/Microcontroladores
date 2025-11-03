#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "configGPS.h"
#include "LCDGPS.h"

#define _XTAL_FREQ 8000000

// ---------- UART ----------
void UART_Init(long baudrate) {
    TRISCbits.TRISC6 = 1;  // TX
    TRISCbits.TRISC7 = 1;  // RX
    SPBRG = (_XTAL_FREQ / (64UL * baudrate)) - 1;
    TXSTAbits.BRGH = 0;
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

unsigned char UART_Available(void) {
    return PIR1bits.RCIF;
}

char UART_Read(void) {
    if (RCSTAbits.OERR) {     
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    return RCREG;
}

// ---------- Conversión ddmm.mmmm -> dd.ddddd ----------
float ConvertToDecimal(const char *value) {
    if (strlen(value) < 3) return 0.0;
    float val = atof(value);
    int grados = (int)(val / 100);
    float minutos = val - (grados * 100);
    return grados + (minutos / 60.0);
}

// ---------- Variables ----------
char nmea[120];
char lat_raw[20], lon_raw[20], alt_raw[20];
char ns, ew;
float lat, lon;

// ---------- Parseo de $GPGGA ----------
unsigned char ParseGPGGA(char *sentence) {
    if (strstr(sentence, "$GPGGA") == NULL) return 0;

    char *token;
    int field = 0;
    token = strtok(sentence, ",");

    while (token != NULL) {
        field++;
        switch (field) {
            case 3: strcpy(lat_raw, token); break;
            case 4: ns = token[0]; break;
            case 5: strcpy(lon_raw, token); break;
            case 6: ew = token[0]; break;
            case 10: strcpy(alt_raw, token); break;
        }
        token = strtok(NULL, ",");
    }

    if (strlen(lat_raw) < 4 || strlen(lon_raw) < 4) return 0;
    lat = ConvertToDecimal(lat_raw);
    lon = ConvertToDecimal(lon_raw);
    if (ns == 'S') lat = -lat;
    if (ew == 'W') lon = -lon;
    return 1;
}

// ---------- MAIN ----------
void main(void) {
    OSCCONbits.IRCF = 0b111; // 8 MHz interno
    ADCON1 = 0x0F;
    CMCON = 0x07;

    LCD_Init();
    LCD_Clear();
    UART_Init(9600);

    LCD_String_xy(0, 0, "Esperando GPS...");
    LCD_String_xy(1, 0, "Sin senal...");
    
    int i = 0;
    char c;
    unsigned char gotData = 0;
    unsigned long noDataCounter = 0;
    unsigned char showingWait = 1; // flag para saber si ya mostramos coordenadas

    while (1) {
        if (UART_Available()) {
            c = UART_Read();

            if (c == '$') {
                i = 0;
                nmea[i++] = c;
            } 
            else if (c == '\n' && i > 6) {
                nmea[i] = '\0';
                gotData = ParseGPGGA(nmea);

                if (gotData) {
                    char l1[17], l2[17];
                    static unsigned char toggle = 0;
                    showingWait = 0; // ya tenemos datos válidos

                    if (!toggle) {
                        sprintf(l1, "Lat:%2.5f", lat);
                        sprintf(l2, "Lon:%2.5f", lon);
                        LCD_Clear();
                        LCD_String_xy(0, 0, l1);
                        LCD_String_xy(1, 0, l2);
                        toggle = 1;
                    } else {
                        LCD_Clear();
                        LCD_String_xy(0, 0, "Altitud:");
                        LCD_String_xy(1, 0, alt_raw);
                        LCD_String_xy(1, strlen(alt_raw), "m");
                        toggle = 0;
                    }
                }
                i = 0;
                noDataCounter = 0;
            } 
            else if (i < sizeof(nmea) - 1) {
                nmea[i++] = c;
            }
        } else {
            __delay_ms(10);
            noDataCounter++;

            // Si pasan 10 segundos sin datos válidos, mostrar mensaje de espera
            if (noDataCounter > 1000 && !showingWait) {
                LCD_Clear();
                LCD_String_xy(0, 0, "Esperando GPS...");
                LCD_String_xy(1, 0, "Sin senal...");
                showingWait = 1;
                noDataCounter = 0;
            }
        }
    }
}
