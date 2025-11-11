#ifndef SSD1306_H
#define SSD1306_H

#include <xc.h>
#include <stdint.h>
#include "i2c.h"

#define SSD1306_ADDR 0x78  // (0x3C << 1)

void OLED_Init(void);
void OLED_Command(uint8_t cmd);
void OLED_Data(uint8_t data);
void OLED_Clear(void);
void OLED_SetCursor(uint8_t x, uint8_t y);
void OLED_PrintChar(char c);
void OLED_PrintText(uint8_t x, uint8_t y, const char *str);

#endif