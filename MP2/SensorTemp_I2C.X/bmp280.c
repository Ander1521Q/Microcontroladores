#include "ssd1306.h"

// Registros BMP280
#define BMP280_REG_TEMP_XLSB   0xFC
#define BMP280_REG_TEMP_LSB    0xFB
#define BMP280_REG_TEMP_MSB    0xFA
#define BMP280_REG_PRESS_XLSB  0xF9
#define BMP280_REG_PRESS_LSB   0xF8
#define BMP280_REG_PRESS_MSB   0xF7
#define BMP280_REG_CONFIG      0xF5
#define BMP280_REG_CTRL_MEAS   0xF4
#define BMP280_REG_STATUS      0xF3
#define BMP280_REG_RESET       0xE0
#define BMP280_REG_ID          0xD0
#define BMP280_REG_CALIB       0x88

// Parámetros de calibración
uint16_t dig_T1;
int16_t dig_T2, dig_T3;
uint16_t dig_P1;
int16_t dig_P2, dig_P3, dig_P4, dig_P5, dig_P6, dig_P7, dig_P8, dig_P9;

int32_t t_fine;

void BMP280_ReadCalibration(void) {
    uint8_t calib_data[24];
    
    // Leer 24 bytes de calibración (0x88-0x9F)
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(BMP280_REG_CALIB);
    I2C_Stop();
    
    __delay_us(10);
    
    I2C_Start();
    I2C_Write(BMP280_ADDR | 0x01);
    for(uint8_t i = 0; i < 24; i++) {
        calib_data[i] = I2C_Read(i < 23 ? 1 : 0);
    }
    I2C_Stop();
    
    // Convertir datos de calibración
    dig_T1 = (calib_data[1] << 8) | calib_data[0];
    dig_T2 = (calib_data[3] << 8) | calib_data[2];
    dig_T3 = (calib_data[5] << 8) | calib_data[4];
    dig_P1 = (calib_data[7] << 8) | calib_data[6];
    dig_P2 = (calib_data[9] << 8) | calib_data[8];
    dig_P3 = (calib_data[11] << 8) | calib_data[10];
    dig_P4 = (calib_data[13] << 8) | calib_data[12];
    dig_P5 = (calib_data[15] << 8) | calib_data[14];
    dig_P6 = (calib_data[17] << 8) | calib_data[16];
    dig_P7 = (calib_data[19] << 8) | calib_data[18];
    dig_P8 = (calib_data[21] << 8) | calib_data[20];
    dig_P9 = (calib_data[23] << 8) | calib_data[22];
}

void BMP280_Init(void) {
    __delay_ms(100);
    
    // Verificar ID del dispositivo (debe ser 0x58)
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(BMP280_REG_ID);
    I2C_Stop();
    
    __delay_us(10);
    
    I2C_Start();
    I2C_Write(BMP280_ADDR | 0x01);
    uint8_t id = I2C_Read(0);
    I2C_Stop();
    
    // Leer calibración
    BMP280_ReadCalibration();
    
    // Configurar BMP280 - Modo normal, oversampling x1
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(BMP280_REG_CTRL_MEAS);
    I2C_Write(0x3F); // Temperatura y presión oversampling x1, modo normal
    I2C_Stop();
    
    __delay_ms(100);
}

int32_t BMP280_Compensate_T(int32_t adc_T) {
    int32_t var1, var2, T;
    var1 = ((((adc_T >> 3) - ((int32_t)dig_T1 << 1))) * ((int32_t)dig_T2)) >> 11;
    var2 = (((((adc_T >> 4) - ((int32_t)dig_T1)) * ((adc_T >> 4) - ((int32_t)dig_T1))) >> 12) * ((int32_t)dig_T3)) >> 14;
    t_fine = var1 + var2;
    T = (t_fine * 5 + 128) >> 8;
    return T;
}

uint32_t BMP280_Compensate_P(int32_t adc_P) {
    int64_t var1, var2, p;
    var1 = ((int64_t)t_fine) - 128000;
    var2 = var1 * var1 * (int64_t)dig_P6;
    var2 = var2 + ((var1 * (int64_t)dig_P5) << 17);
    var2 = var2 + (((int64_t)dig_P4) << 35);
    var1 = ((var1 * var1 * (int64_t)dig_P3) >> 8) + ((var1 * (int64_t)dig_P2) << 12);
    var1 = (((((int64_t)1) << 47) + var1)) * ((int64_t)dig_P1) >> 33;
    if (var1 == 0) return 0;
    p = 1048576 - adc_P;
    p = (((p << 31) - var2) * 3125) / var1;
    var1 = (((int64_t)dig_P9) * (p >> 13) * (p >> 13)) >> 25;
    var2 = (((int64_t)dig_P8) * p) >> 19;
    p = ((p + var1 + var2) >> 8) + (((int64_t)dig_P7) << 4);
    return (uint32_t)p;
}

float BMP280_ReadTemperature(void) {
    uint8_t data[3];
    
    // Leer temperatura (3 bytes)
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(BMP280_REG_TEMP_MSB);
    I2C_Stop();
    
    __delay_us(10);
    
    I2C_Start();
    I2C_Write(BMP280_ADDR | 0x01);
    data[0] = I2C_Read(1); // MSB
    data[1] = I2C_Read(1); // LSB
    data[2] = I2C_Read(0); // XLSB
    I2C_Stop();
    
    int32_t adc_T = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    int32_t T = BMP280_Compensate_T(adc_T);
    return (float)T / 100.0;
}

float BMP280_ReadPressure(void) {
    uint8_t data[6];
    
    // Leer presión y temperatura (6 bytes)
    I2C_Start();
    I2C_Write(BMP280_ADDR);
    I2C_Write(BMP280_REG_PRESS_MSB);
    I2C_Stop();
    
    __delay_us(10);
    
    I2C_Start();
    I2C_Write(BMP280_ADDR | 0x01);
    for(uint8_t i = 0; i < 6; i++) {
        data[i] = I2C_Read(i < 5 ? 1 : 0);
    }
    I2C_Stop();
    
    // Primero compensar temperatura para obtener t_fine
    int32_t adc_T = (data[3] << 12) | (data[4] << 4) | (data[5] >> 4);
    BMP280_Compensate_T(adc_T);
    
    // Luego compensar presión
    int32_t adc_P = (data[0] << 12) | (data[1] << 4) | (data[2] >> 4);
    uint32_t p = BMP280_Compensate_P(adc_P);
    return (float)p / 25600.0; // Convertir a hPa
}