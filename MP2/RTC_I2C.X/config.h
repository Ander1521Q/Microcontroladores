/*!
\file   Config.h
\brief  Configuration bits for PIC18F4550.
*/

#include <xc.h>

#ifndef CONFIG_H
#define	CONFIG_H

#define _XTAL_FREQ 8000000

// CONFIG1L
#pragma config PLLDIV = 1       // No prescale (4 MHz oscillator input drives PLL directly)
#pragma config CPUDIV = OSC1_PLL2
#pragma config USBDIV = 1

// CONFIG1H
#pragma config FOSC = INTOSC_EC // Internal oscillator
#pragma config FCMEN = OFF
#pragma config IESO = OFF

// CONFIG2L
#pragma config PWRT = OFF
#pragma config BOR = ON
#pragma config BORV = 3
#pragma config VREGEN = OFF

// CONFIG2H
#pragma config WDT = OFF
#pragma config WDTPS = 32768

// CONFIG3H
#pragma config CCP2MX = ON
#pragma config PBADEN = OFF
#pragma config LPT1OSC = OFF
#pragma config MCLRE = OFF

// CONFIG4L
#pragma config STVREN = ON
#pragma config LVP = OFF
#pragma config ICPRT = OFF
#pragma config XINST = OFF

// CONFIG5L-7H (Protection bits)
#pragma config CP0 = OFF, CP1 = OFF, CP2 = OFF, CP3 = OFF
#pragma config CPB = OFF, CPD = OFF
#pragma config WRT0 = OFF, WRT1 = OFF, WRT2 = OFF, WRT3 = OFF
#pragma config WRTC = OFF, WRTB = OFF, WRTD = OFF
#pragma config EBTR0 = OFF, EBTR1 = OFF, EBTR2 = OFF, EBTR3 = OFF
#pragma config EBTRB = OFF

#endif
