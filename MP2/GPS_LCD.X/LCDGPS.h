#ifndef LCDGPS_H
#define	LCDGPS_H

#include <xc.h>
#include "configGPS.h"

#define RS LATBbits.LATB2
#define EN LATBbits.LATB3
#define D4 LATDbits.LATD4
#define D5 LATDbits.LATD5
#define D6 LATDbits.LATD6
#define D7 LATDbits.LATD7

#define RS_DIR TRISBbits.TRISB2
#define EN_DIR TRISBbits.TRISB3
#define D4_DIR TRISDbits.TRISD4
#define D5_DIR TRISDbits.TRISD5
#define D6_DIR TRISDbits.TRISD6
#define D7_DIR TRISDbits.TRISD7

void LCD_Init(void);
void LCD_Command(unsigned char);
void LCD_Char(unsigned char);
void LCD_String(const char *);
void LCD_String_xy(char, char, const char *);
void LCD_Clear(void);

#endif
