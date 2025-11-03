#include <xc.h>

#ifndef CONFIGGPS_H
#define CONFIGGPS_H

#define _XTAL_FREQ 8000000

#pragma config PLLDIV = 1
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 1
#pragma config FOSC = INTOSC_EC
#pragma config FCMEN = OFF
#pragma config IESO = OFF
#pragma config PWRT = OFF
#pragma config BOR = ON
#pragma config BORV = 3
#pragma config VREGEN = OFF
#pragma config WDT = OFF
#pragma config WDTPS = 32768
#pragma config CCP2MX = ON
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF
#pragma config MCLRE = OFF
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config ICPRT = OFF
#pragma config XINST = OFF

#endif
