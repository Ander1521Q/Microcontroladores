#include "ssd1306.h"

// Registros BMP280
#define BMP280_REG_ID          0xD0

uint8_t bmp280_ok = 0;
uint8_t bmp280_address = 0;

uint8_t BMP280_TestAddress(uint8_t addr) {
    I2C_Start();
    uint8_t ack = I2C_Write(addr);
    I2C_Stop();
    return ack;
}

uint8_t BMP280_ReadID(uint8_t addr) {
    uint8_t id = 0;
    
    I2C_Start();
    if(I2C_Write(addr)) {
        I2C_Write(BMP280_REG_ID);
        I2C_Stop();
        
        __delay_us(100);
        
        I2C_Start();
        if(I2C_Write(addr | 0x01)) {
            id = I2C_Read(0);
        }
        I2C_Stop();
    } else {
        I2C_Stop();
    }
    
    return id;
}

void BMP280_Init(void) {
    __delay_ms(100);
    
    // Probar ambas direcciones posibles
    uint8_t id_76 = BMP280_ReadID(0x76);
    uint8_t id_77 = BMP280_ReadID(0x77);
    
    if(id_76 == 0x58) {
        bmp280_ok = 1;
        bmp280_address = 0x76;
    } else if(id_77 == 0x58) {
        bmp280_ok = 1;
        bmp280_address = 0x77;
    } else {
        bmp280_ok = 0;
        bmp280_address = 0;
    }
}

float BMP280_ReadTemperature(void) {
    if(!bmp280_ok) return -999.0;
    
    // Por ahora retornar valor de prueba
    return 25.5; // Valor de ejemplo
}

float BMP280_ReadPressure(void) {
    if(!bmp280_ok) return -999.0;
    
    // Por ahora retornar valor de prueba  
    return 1013.2; // Valor de ejemplo
}