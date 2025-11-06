#ifndef BMP280_H
#define BMP280_H

#include <xc.h>
#include <stdint.h>
#include "i2c_soft.h"

#define BMP280_ADDR 0xEC  // Dirección del sensor (0x76 << 1)

void BMP280_Init(void);
float BMP280_ReadTemperature(void);
float BMP280_ReadPressure(void);

#endif
