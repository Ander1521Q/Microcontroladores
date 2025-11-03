/* 
 * File:   i2c.c
 * Author: ANDER
 *
 * Created on 20 de octubre de 2025, 07:00 PM
 */

#include "i2c.h"

void I2C_Master_Init(const unsigned long c) {
    SSPCON = 0b00101000;
    SSPCON2 = 0x00;
    SSPADD = (_XTAL_FREQ / (4 * c)) - 1;
    SSPSTAT = 0x00;
    TRISBbits.TRISB0 = 1; // SDA
    TRISBbits.TRISB1 = 1; // SCL
}

void I2C_Master_Wait(void) {
    while ((SSPCON2 & 0x1F) || (SSPSTATbits.R_nW));
}

void I2C_Master_Start(void) {
    I2C_Master_Wait();
    SSPCON2bits.SEN = 1;
}

void I2C_Master_RepeatedStart(void) {
    I2C_Master_Wait();
    SSPCON2bits.RSEN = 1;
}

void I2C_Master_Stop(void) {
    I2C_Master_Wait();
    SSPCON2bits.PEN = 1;
}

void I2C_Master_Write(unsigned d) {
    I2C_Master_Wait();
    SSPBUF = d;
}

unsigned short I2C_Master_Read(unsigned short a) {
    unsigned short temp;
    I2C_Master_Wait();
    SSPCON2bits.RCEN = 1;
    I2C_Master_Wait();
    temp = SSPBUF;
    I2C_Master_Wait();
    SSPCON2bits.ACKDT = (a)?0:1;
    SSPCON2bits.ACKEN = 1;
    return temp;
}

