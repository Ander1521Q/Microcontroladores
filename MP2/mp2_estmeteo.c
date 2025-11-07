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

// =======================================================
//               BMP280 (TEMPERATURA Y PRESIÓN)
// =======================================================
void BMP280_Write8(unsigned char reg, unsigned char val) {
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(reg);
    I2C_Write(val);
    I2C_Stop();
}

unsigned long BMP280_Read24(unsigned char reg) {
    unsigned long value;
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(reg);
    I2C_Start();
    I2C_Write(BMP280_ADDR | 1);
    value = ((unsigned long)I2C_Read(1) << 16) | ((unsigned long)I2C_Read(1) << 8) | I2C_Read(0);
    I2C_Stop();
    return value;
}

float BMP280_ReadTemperature(void) {
    return 25.0; // Simulado
}

float BMP280_ReadPressure(void) {
    return 101325.0; // Simulado
}



// =======================================================
//               SENSOR DE HUMEDAD (ADC)
// =======================================================
void ADC_Init(void) {
    ADCON1 = 0x0E;
    ADCON2 = 0b10101010;
    ADCON0 = 0x01;
}

unsigned int ADC_Read(unsigned char channel) {
    ADCON0 = (channel << 2) | 0x01;
    __delay_ms(2);
    GO_DONE = 1;
    while (GO_DONE);
    return ((ADRESH << 8) + ADRESL);
}




void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;
    I2C_Init();
    OLED_Init();
    OLED_Clear();
    ADC_Init();

    


    while (1) {
        if (!BTN) { // Botón presionado
            pantalla++;
            if (pantalla > 2) pantalla = 0;
            __delay_ms(300);
        }
        OLED_Clear();

        if (pantalla == 0) {
            float temp = BMP280_ReadTemperature();
            float pres = BMP280_ReadPressure() / 100;
            sprintf(buffer, "Temp: %.2f C", temp);
            OLED_PrintText(0, 1, buffer);
            sprintf(buffer, "Pres: %.2f hPa", pres);
            OLED_PrintText(0, 3, buffer);
        }


        sprintf(buffer, "Temp: %.2f C", temperatura);
        OLED_PrintText(0, 1, buffer);

        sprintf(buffer, "Pres: %.2f hPa", presion);
        OLED_PrintText(0, 3, buffer);

        __delay_ms(1000);
    }
}
