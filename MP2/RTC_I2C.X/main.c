/* 
 * File:   main.c
 * Author: ANDER
 *
 * Created on 20 de octubre de 2025, 07:08 PM
 */

#include <xc.h>
#include <stdio.h>
#include "LCD.h"
#include "Config.h"
#include "i2c.h"
#include "ds1307.h"

void main(void) {
    unsigned char h, m, s, d, mo, y;
    char buffer[17];

    OSCCONbits.IRCF = 0b111; // 8 MHz interno
    OSCCONbits.SCS = 0b10;
    ADCON1 = 0x0F;
    CMCON = 0x07; // Desactivar comparadores

    LCD_Init();
    LCD_Clear();
    I2C_Master_Init(100000); // I2C a 100kHz

    LCD_String_xy(0, 0, "Configurando RTC");
    RTC_Init();
    RTC_SetDateTime(14, 30, 0, 16, 10, 25); // Hora inicial
    LCD_Clear();

    while (1) {
        RTC_GetDateTime(&h, &m, &s, &d, &mo, &y);
        sprintf(buffer, "Fecha:%02d/%02d/20%02d", d, mo, y);
        LCD_String_xy(0, 0, buffer);
        sprintf(buffer, "Hora: %02d:%02d:%02d", h, m, s);
        LCD_String_xy(1, 0, buffer);
        __delay_ms(1000);
    }
}


