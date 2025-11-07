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


// =============== PINES =========================
#define SDA LATBbits.LATB0
#define SCL LATBbits.LATB1
#define SDA_DIR TRISBbits.TRISB0
#define SCL_DIR TRISBbits.TRISB1

#define BTN PORTBbits.RB2
#define BTN_DIR TRISBbits.TRISB2

#define OLED_ADDR 0x78
#define BMP280_ADDR 0xEC


// =======================================================
//                 I2C SOFTWARE
// =======================================================
void I2C_Delay(void) { __delay_us(5); }

void I2C_Init(void) {
    SDA_DIR = 0; SCL_DIR = 0;
    SDA = 1; SCL = 1;
}

void I2C_Start(void) {
    SDA = 1; SCL = 1; I2C_Delay();
    SDA = 0; I2C_Delay();
    SCL = 0;
}

void I2C_Stop(void) {
    SDA = 0; SCL = 1; I2C_Delay();
    SDA = 1; I2C_Delay();
}

void I2C_Write(unsigned char data) {
    for (unsigned char i = 0; i < 8; i++) {
        SDA = (data & 0x80) ? 1 : 0;
        SCL = 1; I2C_Delay();
        SCL = 0; I2C_Delay();
        data <<= 1;
    }
    SDA_DIR = 1;
    SCL = 1; I2C_Delay();
    SCL = 0;
    SDA_DIR = 0;
}

unsigned char I2C_Read(unsigned char ack) {
    unsigned char data = 0;
    SDA_DIR = 1;
    for (unsigned char i = 0; i < 8; i++) {
        SCL = 1;
        if (PORTBbits.RB0) data |= (1 << (7 - i));
        I2C_Delay();
        SCL = 0; I2C_Delay();
    }
    SDA_DIR = 0;
    SDA = (ack) ? 0 : 1;
    SCL = 1; I2C_Delay();
    SCL = 0;
    SDA = 1;
    return data;
}



// =======================================================
//              OLED SSD1306 (I2C SOFTWARE)
// =======================================================
void OLED_Command(unsigned char cmd) {
    I2C_Start();
    I2C_Write(OLED_ADDR);
    I2C_Write(0x00);
    I2C_Write(cmd);
    I2C_Stop();
}

void OLED_Data(unsigned char data) {
    I2C_Start();
    I2C_Write(OLED_ADDR);
    I2C_Write(0x40);
    I2C_Write(data);
    I2C_Stop();
}

void OLED_Init(void) {
    __delay_ms(100);
    OLED_Command(0xAE); // OFF
    OLED_Command(0x20); OLED_Command(0x00); // Horizontal
    OLED_Command(0x81); OLED_Command(0x7F);
    OLED_Command(0xA1);
    OLED_Command(0xA6);
    OLED_Command(0xA8); OLED_Command(0x3F);
    OLED_Command(0xC8);
    OLED_Command(0xD3); OLED_Command(0x00);
    OLED_Command(0xD5); OLED_Command(0x80);
    OLED_Command(0xD9); OLED_Command(0xF1);
    OLED_Command(0xDA); OLED_Command(0x12);
    OLED_Command(0xDB); OLED_Command(0x40);
    OLED_Command(0x8D); OLED_Command(0x14);
    OLED_Command(0xAF); // ON
}

void OLED_Clear(void) {
    for (unsigned char page = 0; page < 8; page++) {
        OLED_Command(0xB0 + page);
        OLED_Command(0x00);
        OLED_Command(0x10);
        for (unsigned char col = 0; col < 128; col++) {
            OLED_Data(0x00);
        }
    }
}

void OLED_SetCursor(unsigned char x, unsigned char y) {
    OLED_Command(0xB0 + y);
    OLED_Command(((x & 0xF0) >> 4) | 0x10);
    OLED_Command((x & 0x0F) | 0x01);
}

// Simulación texto básico (versión simple)
void OLED_PrintText(unsigned char x, unsigned char y, const char *text) {
    OLED_SetCursor(x, y);
    while (*text) {
        for (unsigned char i = 0; i < 5; i++) OLED_Data(0xFF);
        text++;
    }
}



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
