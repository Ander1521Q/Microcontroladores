#ifndef LCD_H
#define	LCD_H

#include <xc.h>
#include "Config.h"

// ================================
// ASIGNACIÓN DE PINES DEL LCD
// ================================

// Pines de control
#define RS LATBbits.LATB2   // RS -> RB2 (pin 35)
#define EN LATBbits.LATB3   // EN -> RB3 (pin 36)

// Pines de datos (modo 4 bits)
#define D4 LATDbits.LATD4   // D4 -> RD4 (pin 27)
#define D5 LATDbits.LATD5   // D5 -> RD5 (pin 28)
#define D6 LATDbits.LATD6   // D6 -> RD6 (pin 29)
#define D7 LATDbits.LATD7   // D7 -> RD7 (pin 30)

// Direcciones de TRIS (configuración como salida)
#define RS_DIR TRISBbits.TRISB2
#define EN_DIR TRISBbits.TRISB3
#define D4_DIR TRISDbits.TRISD4
#define D5_DIR TRISDbits.TRISD5
#define D6_DIR TRISDbits.TRISD6
#define D7_DIR TRISDbits.TRISD7

// ================================
// DECLARACIÓN DE FUNCIONES
// ================================

void LCD_Init(void);
void LCD_Command(unsigned char);
void LCD_Char(unsigned char);
void LCD_String(const char *);
void LCD_String_xy(char, char, const char *);
void LCD_Clear(void);

#endif
