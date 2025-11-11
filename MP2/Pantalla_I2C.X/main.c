/*
 * File:   main.c
 * Author: ANDER
 *
 * Created on 5 de noviembre de 2025, 10:41 AM
 */


#include <xc.h>
#include "ssd1306.h"

// CONFIG
#pragma config FOSC = INTOSCIO_EC
#pragma config PLLDIV = 1
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 1
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOR = OFF
#pragma config VREGEN = OFF
#pragma config WDT = OFF
#pragma config MCLRE = ON
#pragma config LVP = OFF
#pragma config XINST = OFF
#pragma config PBADEN = OFF

#define _XTAL_FREQ 8000000UL

void main(void) {
    // Oscilador interno a 8 MHz
    OSCCONbits.IRCF = 0b111;
    OSCCONbits.SCS = 0b10;

    I2C_Init();
    OLED_Init();
    OLED_Clear();

    OLED_PrintText(32, 3, "HELLO WORLD");

    while (1);
}
