#include "bmp280.h"
#define BMP_ADDR 0xEC  // Escribe / lectura cambia con bit 0

void BMP280_Init(void){
    I2C_Start();
    I2C_Write(BMP_ADDR);
    I2C_Write(0xF4);
    I2C_Write(0x27);
    I2C_Stop();
}

float BMP280_ReadTemperature(void){
    uint32_t raw;

    I2C_Start();
    I2C_Write(BMP_ADDR);
    I2C_Write(0xFA);
    I2C_Stop();

    I2C_Start();
    I2C_Write(BMP_ADDR | 1);

    raw = (I2C_Read(1) << 12) | (I2C_Read(1) << 4) | (I2C_Read(0) >> 4);

    I2C_Stop();
    return raw / 100.0;
}
