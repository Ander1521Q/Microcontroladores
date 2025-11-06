#include <xc.h>
#include <stdio.h>
#include "i2c.h"
#include "ssd1306.h"
#include "bmp280.h"

// CONFIGURACIÓN DEL PIC
#pragma config FOSC = INTOSCIO_EC
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF

#define _XTAL_FREQ 8000000UL

void main(void) {
    float temperatura = 0.0;
    float presion = 0.0;
    char buffer[20];

    // Configurar oscilador interno a 8 MHz
    OSCCONbits.IRCF = 0b111;  // 8MHz
    OSCCONbits.SCS = 0b10;    // Oscilador interno

    // Inicializar periféricos
    I2C_Init();
    OLED_Init();
    BMP280_Init();

    OLED_Clear();
    OLED_PrintText(15, 0, "ESTACION METEO");
    OLED_PrintText(0, 2, "Leyendo datos...");
    __delay_ms(1500);

    while (1) {
        // Leer temperatura y presión
        temperatura = BMP280_ReadTemperature();
        presion = BMP280_ReadPressure() / 100.0; // Pa → hPa

        // Limpiar pantalla y mostrar resultados
        OLED_Clear();

        sprintf(buffer, "Temp: %.2f C", temperatura);
        OLED_PrintText(0, 1, buffer);

        sprintf(buffer, "Pres: %.2f hPa", presion);
        OLED_PrintText(0, 3, buffer);

        __delay_ms(1000);
    }
}
