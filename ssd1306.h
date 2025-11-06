#ifndef _SSD1306_H
#define _SSD1306_H

#include <xc.h>
#include <stdint.h>
#include "i2c_soft.h"

void OLED_Init(void);
void OLED_Clear(void);
void OLED_PrintText(uint8_t page, uint8_t col, const char *txt);
void OLED_Command(uint8_t cmd);
void OLED_Data(uint8_t data);

#endif
