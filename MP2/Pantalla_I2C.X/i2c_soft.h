#ifndef I2C_SOFT_H
#define I2C_SOFT_H

#include <xc.h>
#include <stdint.h>

#define _XTAL_FREQ 8000000UL

// Pines de I2C software
#define SDA LATBbits.LATB0
#define SCL LATBbits.LATB1
#define SDA_DIR TRISBbits.TRISB0
#define SCL_DIR TRISBbits.TRISB1

void I2C_Init(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Write(uint8_t data);

#endif
