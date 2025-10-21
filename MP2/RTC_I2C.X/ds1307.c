/* 
 * File:   ds1307.c
 * Author: ANDER
 *
 * Created on 20 de octubre de 2025, 07:00 PM
 */

#include "i2c.h"
#include "LCD.h"
#include <stdio.h>
#define DS1307_ADDR 0b11010000

unsigned char BCD2DEC(unsigned char val) {
    return ((val >> 4) * 10) + (val & 0x0F);
}

unsigned char DEC2BCD(unsigned char val) {
    return ((val / 10) << 4) | (val % 10);
}

void RTC_Init(void) {
    unsigned char sec;

    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR);
    I2C_Master_Write(0x00);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS1307_ADDR | 1);
    sec = I2C_Master_Read(0);
    I2C_Master_Stop();

    if (sec & 0x80) {
        LCD_Clear();
        LCD_String_xy(0, 0, "RTC detenido");
        __delay_ms(1000);

        I2C_Master_Start();
        I2C_Master_Write(DS1307_ADDR);
        I2C_Master_Write(0x00);
        I2C_Master_Write(sec & 0x7F); // Activar CH=0
        I2C_Master_Stop();
    }
}

void RTC_SetDateTime(unsigned char h, unsigned char m, unsigned char s,
                     unsigned char d, unsigned char mo, unsigned char y) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR);
    I2C_Master_Write(0x00);
    I2C_Master_Write(DEC2BCD(s & 0x7F));
    I2C_Master_Write(DEC2BCD(m));
    I2C_Master_Write(DEC2BCD(h));
    I2C_Master_Write(DEC2BCD(1)); // Día de semana
    I2C_Master_Write(DEC2BCD(d));
    I2C_Master_Write(DEC2BCD(mo));
    I2C_Master_Write(DEC2BCD(y));
    I2C_Master_Stop();
}

void RTC_GetDateTime(unsigned char *hour, unsigned char *min, unsigned char *sec,
                     unsigned char *day, unsigned char *month, unsigned char *year) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDR);
    I2C_Master_Write(0x00);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS1307_ADDR | 1);

    *sec   = BCD2DEC(I2C_Master_Read(1));
    *min   = BCD2DEC(I2C_Master_Read(1));
    *hour  = BCD2DEC(I2C_Master_Read(1));
    I2C_Master_Read(1);
    *day   = BCD2DEC(I2C_Master_Read(1));
    *month = BCD2DEC(I2C_Master_Read(1));
    *year  = BCD2DEC(I2C_Master_Read(0));
    I2C_Master_Stop();
}

