/* 
 * File:   ds1307.c
 * Author: ANDER
 *
 * Created on 20 de octubre de 2025, 07:00 PM
 */

#include "ds1307.h"

unsigned char BCDtoDecimal(unsigned char bcd) {
    return ((bcd >> 4) * 10) + (bcd & 0x0F);
}

unsigned char DecimaltoBCD(unsigned char dec) {
    return ((dec / 10) << 4) | (dec % 10);
}

void DS1307_Init(void) {
    I2C_Master_Init(100000); // 100kHz
}

void DS1307_SetTime(unsigned char h, unsigned char m, unsigned char s) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDRESS);
    I2C_Master_Write(0x00);
    I2C_Master_Write(DecimaltoBCD(s));
    I2C_Master_Write(DecimaltoBCD(m));
    I2C_Master_Write(DecimaltoBCD(h));
    I2C_Master_Stop();
}

void DS1307_SetDate(unsigned char d, unsigned char mth, unsigned char y) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDRESS);
    I2C_Master_Write(0x04);
    I2C_Master_Write(DecimaltoBCD(d));
    I2C_Master_Write(DecimaltoBCD(mth));
    I2C_Master_Write(DecimaltoBCD(y));
    I2C_Master_Stop();
}

void DS1307_GetTime(unsigned char *h, unsigned char *m, unsigned char *s) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDRESS);
    I2C_Master_Write(0x00);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS1307_ADDRESS + 1);
    *s = BCDtoDecimal(I2C_Master_Read(1));
    *m = BCDtoDecimal(I2C_Master_Read(1));
    *h = BCDtoDecimal(I2C_Master_Read(0));
    I2C_Master_Stop();
}

void DS1307_GetDate(unsigned char *d, unsigned char *mth, unsigned char *y) {
    I2C_Master_Start();
    I2C_Master_Write(DS1307_ADDRESS);
    I2C_Master_Write(0x04);
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS1307_ADDRESS + 1);
    *d = BCDtoDecimal(I2C_Master_Read(1));
    *mth = BCDtoDecimal(I2C_Master_Read(1));
    *y = BCDtoDecimal(I2C_Master_Read(0));
    I2C_Master_Stop();
}
