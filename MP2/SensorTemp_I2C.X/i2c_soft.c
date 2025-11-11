#include "i2c_soft.h"

void I2C_Delay(void) {
    __delay_us(5);
}

void I2C_Init(void) {
    SDA_DIR = 0;  // SDA como salida inicialmente
    SCL_DIR = 0;  // SCL como salida
    SDA = 1;      // Línea libre
    SCL = 1;      // Línea libre
}

void I2C_Start(void) {
    SDA = 1;
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
}

void I2C_Restart(void) {
    SDA = 1;
    SCL = 1;
    I2C_Delay();
    SDA = 0;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
}

void I2C_Stop(void) {
    SDA = 0;
    SCL = 1;
    I2C_Delay();
    SDA = 1;
    I2C_Delay();
}

uint8_t I2C_Write(uint8_t data) {
    uint8_t i;
    
    for (i = 0; i < 8; i++) {
        SDA = (data & 0x80) ? 1 : 0;
        SCL = 1;
        I2C_Delay();
        SCL = 0;
        I2C_Delay();
        data <<= 1;
    }
    
    // Leer ACK - CORREGIDO
    SDA_DIR = 1;  // SDA como entrada para leer ACK
    SCL = 1;
    I2C_Delay();
    uint8_t ack = SDA;  // 0 = ACK recibido, 1 = NACK (CORREGIDO)
    SCL = 0;
    I2C_Delay();
    SDA_DIR = 0;  // SDA vuelve a ser salida
    
    return !ack;  // Retorna 1 si ACK, 0 si NACK (CORREGIDO)
}

uint8_t I2C_Read(uint8_t ack) {
    uint8_t data = 0;
    uint8_t i;
    
    SDA_DIR = 1;  // SDA como entrada para leer datos
    
    for (i = 0; i < 8; i++) {
        data <<= 1;
        SCL = 1;
        I2C_Delay();
        if (SDA) {
            data |= 1;
        }
        SCL = 0;
        I2C_Delay();
    }
    
    // Enviar ACK/NACK
    SDA_DIR = 0;  // SDA como salida para enviar ACK
    SDA = !ack;   // 0 = ACK, 1 = NACK
    SCL = 1;
    I2C_Delay();
    SCL = 0;
    I2C_Delay();
    SDA_DIR = 1;  // SDA vuelve a ser entrada
    
    return data;
}