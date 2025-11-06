#ifndef _ADC_H
#define _ADC_H

#include <xc.h>
#include <stdint.h>

void ADC_Init(void);
uint16_t ADC_Read(uint8_t channel);

#endif
