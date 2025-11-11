#include <xc.h>
#include "ssd1306.h"
#include <stdio.h>

// CONFIG
#pragma config FOSC = INTOSCIO_EC
#pragma config PLLDIV = 1
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 1
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config VREGEN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#pragma config XINST = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 8000000UL

char buffer[20];

void I2C_Scan(void) {
    uint8_t devices[] = {0x76, 0x77, 0x78}; // BMP280 y OLED
    char buffer[20];
    
    OLED_Clear();
    OLED_PrintText(5, 0, "ESCANEO I2C COMPLETO");
    
    for(uint8_t i = 0; i < 3; i++) {
        I2C_Start();
        uint8_t ack = I2C_Write(devices[i]); // Dirección sin desplazar
        I2C_Stop();
        
        sprintf(buffer, "0x%02X: %s", devices[i], ack ? "OK" : "NO");
        OLED_PrintText(0, 2 + i, buffer);
        __delay_ms(500);
    }
    
    // Probar también direcciones desplazadas
    OLED_PrintText(0, 5, "Direc. desplazadas:");
    I2C_Start();
    uint8_t ack1 = I2C_Write(0xEC); // 0x76 << 1
    I2C_Stop();
    sprintf(buffer, "0xEC: %s", ack1 ? "OK" : "NO");
    OLED_PrintText(0, 6, buffer);
    
    I2C_Start();
    uint8_t ack2 = I2C_Write(0xEE); // 0x77 << 1
    I2C_Stop();
    sprintf(buffer, "0xEE: %s", ack2 ? "OK" : "NO");
    OLED_PrintText(0, 7, buffer);
    
    __delay_ms(4000);
}

void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;

    I2C_Init();
    OLED_Init();
    OLED_Clear();
    
    // Mensaje inicial
    OLED_PrintText(20, 3, "INICIANDO...");
    __delay_ms(1500);
    
    // Ejecutar escaneo I2C completo
    I2C_Scan();
    OLED_Clear();

    // Inicializar BMP280
    OLED_PrintText(10, 3, "INICIANDO BMP280");
    __delay_ms(1500);
    OLED_Clear();

    BMP280_Init();
    
    // Mostrar resultado detallado
    OLED_PrintText(5, 0, "RESULTADO BMP280");
    
    if(bmp280_ok) {
        OLED_PrintText(5, 2, "Estado: CONECTADO");
        OLED_PrintText(5, 3, "Sensor detectado OK");
        OLED_PrintText(5, 4, "Leyendo datos...");
    } else {
        OLED_PrintText(5, 2, "Estado: NO CONECTADO");
        OLED_PrintText(5, 3, "No detectado en:");
        OLED_PrintText(5, 4, "0x76 ni 0x77");
        OLED_PrintText(5, 5, "Verificar:");
        OLED_PrintText(5, 6, "- Cableado I2C");
        OLED_PrintText(5, 7, "- Voltaje 3.3V");
    }
    __delay_ms(4000);
    OLED_Clear();

    while (1) {
        OLED_PrintText(15, 0, "SENSOR BMP280");
        
        if(bmp280_ok) {
            float temp = BMP280_ReadTemperature();
            float presion = BMP280_ReadPressure();
            
            // Mostrar temperatura
            OLED_PrintText(5, 2, "Temperatura:");
            
            if(temp > -100.0 && temp < 100.0) {
                sprintf(buffer, "%.1f C", temp);
            } else {
                sprintf(buffer, "Error");
            }
            OLED_PrintText(70, 2, buffer);
            
            // Mostrar presión
            OLED_PrintText(5, 4, "Presion:");
            
            if(presion > 800.0 && presion < 1200.0) {
                sprintf(buffer, "%.1f hPa", presion);
            } else {
                sprintf(buffer, "Error");
            }
            OLED_PrintText(70, 4, buffer);
        } else {
            OLED_PrintText(10, 2, "SENSOR NO DETECTADO");
            OLED_PrintText(5, 4, "Revisar:");
            OLED_PrintText(5, 5, "- Conexiones I2C");
            OLED_PrintText(5, 6, "- Voltaje 3.3V");
            OLED_PrintText(5, 7, "- Resistencias pull-up");
        }
        
        __delay_ms(2000);
        
        // Limpiar áreas de datos
        for(uint8_t y = 2; y <= 7; y++) {
            for(uint8_t x = 0; x < 128; x++) {
                OLED_SetCursor(x, y);
                OLED_Data(0x00);
            }
        }
    }
}