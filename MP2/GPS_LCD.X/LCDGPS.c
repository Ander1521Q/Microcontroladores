#include "LCDGPS.h"

#define _XTAL_FREQ 8000000

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
    // Configurar pines como salidas
    RS_DIR = 0;
    EN_DIR = 0;
    D4_DIR = 0;
    D5_DIR = 0;
    D6_DIR = 0;
    D7_DIR = 0;

    __delay_ms(50);  // Espera inicial más larga
    
    // Secuencia de inicialización en 4 bits
    RS = 0;
    EN = 0;
    
    LCD_Send4Bits(0x03);
    __delay_ms(5);
    LCD_Send4Bits(0x03);
    __delay_us(150);
    LCD_Send4Bits(0x03);
    LCD_Send4Bits(0x02);  // Cambio a modo 4 bits
    
    // Configurar LCD
    LCD_Command(0x28); // 4 bits, 2 líneas, 5x8 puntos
    LCD_Command(0x0C); // Display ON, cursor OFF
    LCD_Command(0x06); // Incremento automático, no shift
    LCD_Command(0x01); // Clear display
    __delay_ms(5);
}

void LCD_String(const char *msg) {
    while (*msg) {
        LCD_Char(*msg++);
    }
}

void LCD_String_xy(char row, char pos, const char *msg) {
    char location;
    if (row == 0)
        location = 0x80 + pos;  // Primera línea
    else
        location = 0xC0 + pos;  // Segunda línea
    
    LCD_Command(location);
    LCD_String(msg);
}

void LCD_Clear(void) {
    LCD_Command(0x01);
    __delay_ms(5);
}