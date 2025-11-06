#include "i2c_soft.h"
#define _XTAL_FREQ 8000000

void I2C_Init(void){
    SDA_DIR = 1;
    SCL_DIR = 1;
}

void I2C_Start(void){
    SDA_DIR = 1; SCL_DIR = 1;
    __delay_us(5);
    SDA_DIR = 0;
    __delay_us(5);
    SCL_DIR = 0;
}

void I2C_Stop(void){
    SDA_DIR = 0;
    SCL_DIR = 1;
    __delay_us(5);
    SDA_DIR = 1;
}

bool I2C_Write(uint8_t data){
    for (uint8_t mask = 0x80; mask; mask >>= 1){
        SDA_DIR = (data & mask) ? 1 : 0;
        SCL_DIR = 1;
        __delay_us(5);
        SCL_DIR = 0;
    }
    SDA_DIR = 1;
    SCL_DIR = 1;
    __delay_us(5);
    bool ack = SDA;
    SCL_DIR = 0;
    return ack;
}

uint8_t I2C_Read(bool ack){
    uint8_t data = 0;
    SDA_DIR = 1;

    for(uint8_t i=0; i<8; i++){
        SCL_DIR = 1;
        __delay_us(5);
        data = (data << 1) | SDA;
        SCL_DIR = 0;
    }

    SDA_DIR = ack ? 0 : 1;
    SCL_DIR = 1;
    __delay_us(5);
    SCL_DIR = 0;
    SDA_DIR = 1;

    return data;
}
