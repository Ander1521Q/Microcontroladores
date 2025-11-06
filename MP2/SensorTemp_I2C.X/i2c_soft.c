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
    // ACK
    SDA_DIR = 1;
    SCL = 1; I2C_Delay();
    SCL = 0;
    SDA_DIR = 0;
}

uint8_t I2C_Read(uint8_t ack) {
    uint8_t data = 0;
    SDA_DIR = 1; // SDA como entrada
    
    for (uint8_t i = 0; i < 8; i++) {
        data <<= 1;
        SCL = 1;
        I2C_Delay();
        if(SDA) data |= 1;
        SCL = 0;
        I2C_Delay();
    }
    
    // Enviar ACK/NACK
    SDA_DIR = 0;
    SDA = !ack;
    SCL = 1;
    I2C_Delay();
    SCL = 0;
    SDA_DIR = 1;
    
    return data;
}