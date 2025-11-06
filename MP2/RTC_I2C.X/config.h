/*!
\file   Config.h
\brief  Configuration bits for PIC18F4550.
*/
#ifndef CONFIG_H
#define CONFIG_H

#include <xc.h>
#define _XTAL_FREQ 8000000

#pragma config FOSC = INTOSC_EC
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = OFF

#endif
