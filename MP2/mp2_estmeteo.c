
#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// ================= CONFIGURACIÓN ==========================
#pragma config FOSC = INTOSCIO_EC
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = ON
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#define _XTAL_FREQ 8000000UL

//  PINES 
#define SDA LATBbits.LATB0
#define SCL LATBbits.LATB1
#define SDA_DIR TRISBbits.TRISB0
#define SCL_DIR TRISBbits.TRISB1

#define BTN PORTBbits.RB2
#define BTN_DIR TRISBbits.TRISB2

#define OLED_ADDR 0x78
#define BMP280_ADDR 0xEC

//  VARIABLES GLOBALES 
float t_fine;

// ==========================================================
//                  I2C SOFTWARE
// ==========================================================
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

// ==========================================================
//                    OLED SSD1306
// ==========================================================
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
    OLED_Command(0xAE);
    OLED_Command(0x20); OLED_Command(0x00);
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
    OLED_Command(0xAF);
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

void OLED_PrintText(unsigned char x, unsigned char y, const char *text) {
    OLED_SetCursor(x, y);
    while (*text) {
        for (unsigned char i = 0; i < 6; i++) OLED_Data(0xFF);
        text++;
    }
}

// ==========================================================
//                   BMP280 
// ==========================================================
unsigned short dig_T1, dig_T2, dig_T3;
unsigned short dig_P1; short dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

unsigned short read16_LE(unsigned char reg) {
    unsigned short val;
    I2C_Start(); I2C_Write(BMP280_ADDR); I2C_Write(reg);
    I2C_Start(); I2C_Write(BMP280_ADDR | 1);
    val = I2C_Read(1);
    val |= (I2C_Read(0) << 8);
    I2C_Stop();
    return val;
}

void BMP280_Init(void) {
    unsigned char id;
    I2C_Start(); I2C_Write(BMP280_ADDR); I2C_Write(0xD0);
    I2C_Start(); I2C_Write(BMP280_ADDR | 1);
    id = I2C_Read(0);
    I2C_Stop();

    if (id == 0x58) {
        dig_T1 = read16_LE(0x88);
        dig_T2 = read16_LE(0x8A);
        dig_T3 = read16_LE(0x8C);
        dig_P1 = read16_LE(0x8E);
        dig_P2 = read16_LE(0x90);
        dig_P3 = read16_LE(0x92);
        dig_P4 = read16_LE(0x94);
        dig_P5 = read16_LE(0x96);
        dig_P6 = read16_LE(0x98);
        dig_P7 = read16_LE(0x9A);
        dig_P8 = read16_LE(0x9C);
        dig_P9 = read16_LE(0x9E);

        I2C_Start();
        I2C_Write(BMP280_ADDR);
        I2C_Write(0xF4);
        I2C_Write(0x27);  // Normal mode
        I2C_Stop();

        I2C_Start();
        I2C_Write(BMP280_ADDR);
        I2C_Write(0xF5);
        I2C_Write(0xA0);
        I2C_Stop();
    }
}

float BMP280_ReadTemperature(void) {
    long adc_T;
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(0xFA);
    I2C_Start();
    I2C_Write(BMP280_ADDR | 1);
    adc_T = ((long)I2C_Read(1) << 12) | ((long)I2C_Read(1) << 4) | (I2C_Read(0) >> 4);
    I2C_Stop();

    float var1 = (((float)adc_T / 16384.0) - ((float)dig_T1 / 1024.0)) * (float)dig_T2;
    float var2 = ((((float)adc_T / 131072.0) - ((float)dig_T1 / 8192.0)) *
                 (((float)adc_T / 131072.0) - ((float)dig_T1 / 8192.0))) * (float)dig_T3;
    t_fine = (long)(var1 + var2);
    float T = (var1 + var2) / 5120.0;
    return T;
}

float BMP280_ReadPressure(void) {
    long adc_P;
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(0xF7);
    I2C_Start();
    I2C_Write(BMP280_ADDR | 1);
    adc_P = ((long)I2C_Read(1) << 12) | ((long)I2C_Read(1) << 4) | (I2C_Read(0) >> 4);
    I2C_Stop();

    float var1 = ((float)t_fine / 2.0) - 64000.0;
    float var2 = var1 * var1 * ((float)dig_P6) / 32768.0;
    var2 = var2 + var1 * ((float)dig_P5) * 2.0;
    var2 = (var2 / 4.0) + (((float)dig_P4) * 65536.0);
    var1 = (((float)dig_P3) * var1 * var1 / 524288.0 + ((float)dig_P2) * var1) / 524288.0;
    var1 = (1.0 + var1 / 32768.0) * ((float)dig_P1);
    if (var1 == 0) return 0;
    float p = 1048576.0 - (float)adc_P;
    p = (p - (var2 / 4096.0)) * 6250.0 / var1;
    var1 = ((float)dig_P9) * p * p / 2147483648.0;
    var2 = p * ((float)dig_P8) / 32768.0;
    p = p + (var1 + var2 + ((float)dig_P7)) / 16.0;
    return p;
}

// ==========================================================
//                SENSOR HUMEDAD (ADC) + GPS
// ==========================================================
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

void UART_Init(void) {
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCONbits.BRG16 = 0;
    SPBRG = ((_XTAL_FREQ / (64UL * 9600)) - 1);
    RCSTAbits.SPEN = 1;
    RCSTAbits.CREN = 1;
    TXSTAbits.TXEN = 1;
}

char UART_Read(void) {
    while (!PIR1bits.RCIF);
    return RCREG;
}

// ==========================================================
//                   MAIN PROGRAM
// ==========================================================
void main(void) {
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;

    I2C_Init();
    OLED_Init();
    OLED_Clear();
    BMP280_Init();
    ADC_Init();
    UART_Init();

    BTN_DIR = 1;

    unsigned char pantalla = 0;
    char buffer[22];

    while (1) {
        if (!BTN) {
            pantalla++;
            if (pantalla > 2) pantalla = 0;
            __delay_ms(250);
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
        else if (pantalla == 1) {
            unsigned int hum = ADC_Read(0);
            float humedad = ((float)hum / 1023.0) * 100.0;
            sprintf(buffer, "Humedad: %.1f%%", humedad);
            OLED_PrintText(0, 2, buffer);
        }
        else if (pantalla == 2) {
            sprintf(buffer, "GPS: 03.437S");
            OLED_PrintText(0, 1, buffer);
            sprintf(buffer, "     76.520W");
            OLED_PrintText(0, 3, buffer);
        }

        __delay_ms(800);
    }
}
