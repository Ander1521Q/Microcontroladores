#ifndef _I2C_SOFT_H
#define _I2C_SOFT_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

#define SDA     RB0
#define SCL     RB1
#define SDA_DIR TRISB0
#define SCL_DIR TRISB1

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
bool I2C_Write(uint8_t data);
uint8_t I2C_Read(bool ack);

#endif
