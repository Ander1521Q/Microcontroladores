#include <xc.h>
#include <stdio.h>
#include "Config.h"
#include "LCD.h"
#include "ds1307.h"

void main(void) {
    unsigned char h, m, s, d, mt, y;
    char linea1[17], linea2[17];

    OSCCONbits.IRCF = 0b111; // 8MHz
    ADCON1 = 0x0F;
    CMCON = 0x07;

    LCD_Init();
    LCD_Clear();
    DS1307_Init();

    // Si el reloj no está corriendo, configurarlo
    if (!DS1307_IsRunning()) {
        LCD_Clear();
        LCD_String_xy(0, 0, "RTC sin hora");
        LCD_String_xy(1, 0, "Ajustando...");

        __delay_ms(2000);

        // Ajusta la hora inicial (ejemplo: 14:30 + 2 minutos)
        unsigned char hora = 15;
        unsigned char minuto = 30 + 2;  // 2 minutos adelantado
        if (minuto >= 60) { hora++; minuto -= 60; }

        DS1307_SetTime(hora, minuto, 0);
        DS1307_SetDate(5, 11, 25); // 5 de noviembre de 2025

        LCD_Clear();
        LCD_String_xy(0, 0, "Hora establecida");
        __delay_ms(2000);
    }

    LCD_Clear();

    while (1) {
        DS1307_GetTime(&h, &m, &s);
        DS1307_GetDate(&d, &mt, &y);

        sprintf(linea1, "Fecha:%02d/%02d/20%02d", d, mt, y);
        sprintf(linea2, "Hora:%02d:%02d:%02d", h, m, s);

        LCD_Clear();
        LCD_String_xy(0, 0, linea1);
        LCD_String_xy(1, 0, linea2);

        __delay_ms(1000);
    }
}
