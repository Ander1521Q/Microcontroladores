/*
 * File:   i2c_soft.c
 * Author: ANDER
 *
 * Created on 5 de noviembre de 2025, 10:42 AM
 */


#include "i2c_soft.h"

void I2C_Delay(void) {
    __delay_us(5);
}

void I2C_Init(void) {
    SDA_DIR = 0;
    SCL_DIR = 0;
    SDA = 1;
    SCL = 1;
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

void I2C_Write(uint8_t data) {
    for (uint8_t i = 0; i < 8; i++) {
        SDA = (data & 0x80) ? 1 : 0;
        SCL = 1; I2C_Delay();
        SCL = 0; I2C_Delay();
        data <<= 1;
    }
    // ACK bit
    SDA_DIR = 1;
    SCL = 1; I2C_Delay();
    SCL = 0;
    SDA_DIR = 0;
}
