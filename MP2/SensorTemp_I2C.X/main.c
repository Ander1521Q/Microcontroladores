#include <xc.h>
#include "ssd1306.h"
#include <stdio.h>

// CONFIG (tu configuración original)
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

void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;

    I2C_Init();
    OLED_Init();
    
    // Mensaje de prueba con minúsculas
    OLED_PrintText(10, 0, "Test Minusculas");
    OLED_PrintText(10, 2, "abcdefghijklm");
    OLED_PrintText(10, 4, "nopqrstuvwxyz");
    
    __delay_ms(3000);
    OLED_Clear();

    BMP280_Init();

    while (1) {
        float temp = BMP280_ReadTemperature();
        float presion = BMP280_ReadPressure();
        
        // Usar método buffered para mejor rendimiento
        OLED_Clear();
        
        OLED_PrintTextBuffered(0, 0, "Sensor BMP280");
        
        // Línea divisoria
        for(uint8_t i = 0; i < 128; i++) {
            screen_buffer[1 * 128 + i] = 0xFF; // Línea sólida
        }
        
        OLED_PrintTextBuffered(0, 2, "Temperatura:");
        sprintf(buffer, "%.1f C", temp);
        OLED_PrintTextBuffered(70, 2, buffer);
        
        OLED_PrintTextBuffered(0, 4, "Presion:");
        sprintf(buffer, "%.1f hPa", presion);
        OLED_PrintTextBuffered(70, 4, buffer);
        
        // Actualizar pantalla completa de una vez
        OLED_Update();
        
        __delay_ms(2000);
    }
}