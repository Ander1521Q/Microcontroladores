#ifndef _BMP280_H
#define _BMP280_H

#include <xc.h>
#include <stdint.h>
#include "i2c_soft.h"

void BMP280_Init(void);
float BMP280_ReadTemperature(void);

#endif
