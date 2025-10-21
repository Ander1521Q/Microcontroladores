/* 
 * File:   i2c.c
 * Author: ANDER
 *
 * Created on 20 de octubre de 2025, 07:00 PM
 */

#include <xc.h>
#define _XTAL_FREQ 8000000

void I2C_Master_Init(const unsigned long c) {
    TRISB0 = 1; // SDA
    TRISB1 = 1; // SCL
    SSPCON1 = 0b00101000; // Enable MSSP in I2C Master mode
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ / (4 * c)) - 1; // Clock speed
    SSPSTAT = 0;
}

void I2C_Master_Wait(void) {
    while ((SSPCON2 & 0x1F) || (SSPSTAT & 0x04));
}

void I2C_Master_Start(void) {
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart(void) {
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop(void) {
    I2C_Master_Wait();
    PEN = 1;
}

unsigned char I2C_Master_Write(unsigned d) {
    I2C_Master_Wait();
    SSPBUF = d;
    I2C_Master_Wait();
    return !ACKSTAT; // Devuelve 1 si hubo ACK
}

unsigned short I2C_Master_Read(unsigned short a) {
    unsigned short temp;
    I2C_Master_Wait();
    RCEN = 1;
    I2C_Master_Wait();
    temp = SSPBUF;
    I2C_Master_Wait();
    ACKDT = (a)?0:1;
    ACKEN = 1;
    return temp;
}

