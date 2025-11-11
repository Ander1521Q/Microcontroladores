#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>

#define _XTAL_FREQ 8000000UL

// CONFIGURACIÓN DEL PIC18F4550
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

#endif