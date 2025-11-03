#include "LCDGPS.h"

static void LCD_Pulse(void) {
    EN = 1;
    __delay_us(10);
    EN = 0;
    __delay_us(50);
}

static void LCD_Send4Bits(unsigned char data) {
    D4 = (data >> 0) & 1;
    D5 = (data >> 1) & 1;
    D6 = (data >> 2) & 1;
    D7 = (data >> 3) & 1;
    LCD_Pulse();
}

void LCD_Command(unsigned char cmd) {
    RS = 0;
    LCD_Send4Bits(cmd >> 4);
    LCD_Send4Bits(cmd & 0x0F);
    __delay_ms(2);
}

void LCD_Char(unsigned char data) {
    RS = 1;
    LCD_Send4Bits(data >> 4);
    LCD_Send4Bits(data & 0x0F);
    __delay_us(50);
}

void LCD_Init(void) {
    RS_DIR = EN_DIR = 0;
    D4_DIR = D5_DIR = D6_DIR = D7_DIR = 0;

    __delay_ms(20);
    RS = 0;
    EN = 0;

    LCD_Send4Bits(0x03); __delay_ms(5);
    LCD_Send4Bits(0x03); __delay_us(100);
    LCD_Send4Bits(0x03);
    LCD_Send4Bits(0x02);

    LCD_Command(0x28); // 4 bits, 2 líneas
    LCD_Command(0x0C); // display ON
    LCD_Command(0x06); // auto incremento
    LCD_Command(0x01); // clear
    __delay_ms(2);
}

void LCD_String(const char *msg) {
    while (*msg) LCD_Char(*msg++);
}

void LCD_String_xy(char row, char pos, const char *msg) {
    char location = (row == 0) ? 0x80 : 0xC0;
    location += pos;
    LCD_Command(location);
    LCD_String(msg);
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    __delay_ms(2);
}
