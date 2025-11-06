#include "ssd1306.h"
#define OLED_ADDR 0x78

void OLED_Command(uint8_t cmd){
    I2C_Start();
    I2C_Write(OLED_ADDR);
    I2C_Write(0x00);
    I2C_Write(cmd);
    I2C_Stop();
}

void OLED_Data(uint8_t data){
    I2C_Start();
    I2C_Write(OLED_ADDR);
    I2C_Write(0x40);
    I2C_Write(data);
    I2C_Stop();
}

void OLED_Init(void){
    __delay_ms(200);
    OLED_Command(0xAE);
    OLED_Command(0xD5);
    OLED_Command(0x80);
    OLED_Command(0xA8);
    OLED_Command(0x3F);
    OLED_Command(0xD3);
    OLED_Command(0x00);
    OLED_Command(0x40);
    OLED_Command(0x8D);
    OLED_Command(0x14);
    OLED_Command(0x20);
    OLED_Command(0x00);
    OLED_Command(0xA1);
    OLED_Command(0xC8);
    OLED_Command(0xDA);
    OLED_Command(0x12);
    OLED_Command(0x81);
    OLED_Command(0x8F);
    OLED_Command(0xD9);
    OLED_Command(0xF1);
    OLED_Command(0xDB);
    OLED_Command(0x40);
    OLED_Command(0xA4);
    OLED_Command(0xA6);
    OLED_Command(0xAF);
}

void OLED_Clear(void){
    for(uint8_t page=0; page<8; page++){
        OLED_Command(0xB0 + page);
        OLED_Command(0x00);
        OLED_Command(0x10);
        for(uint8_t i=0; i<128; i++)
            OLED_Data(0x00);
    }
}

void OLED_PrintText(uint8_t page, uint8_t col, const char *txt){
    OLED_Command(0xB0 + page);
    OLED_Command(0x00 + (col & 0x0F));
    OLED_Command(0x10 + (col >> 4));
    while(*txt){
        OLED_Data(*txt++);
    }
}
