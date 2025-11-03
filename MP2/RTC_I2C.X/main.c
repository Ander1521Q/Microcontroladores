/* 
 * File:   main.c
 * Author: ANDER
 *
 * Created on 20 de octubre de 2025, 07:08 PM
 */

#include <xc.h>
#include <stdio.h>
#include "Config.h"
#include "LCD.h"
#include "ds1307.h"

void main(void) {
    unsigned char h, m, s, d, mo, y;
    
    OSCCONbits.IRCF = 0b111; // 8MHz
    ADCON1 = 0x0F;
    CMCON = 0x07;

    LCD_Init();
    LCD_Clear();
    DS1307_Init();

    // Solo se configura la hora una vez:
    // DS1307_SetTime(14, 30, 0);  // 14:30:00
    // DS1307_SetDate(17, 10, 25); // 17/10/2025

    while(1) {
        DS1307_GetTime(&h, &m, &s);
        DS1307_GetDate(&d, &mo, &y);

        char buffer[17];
        sprintf(buffer, "%02u:%02u:%02u", h, m, s);
        LCD_String_xy(0, 3, buffer);

        sprintf(buffer, "%02u/%02u/20%02u", d, mo, y);
        LCD_String_xy(1, 2, buffer);

        __delay_ms(500);
    }
}

