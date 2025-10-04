/* 
 * File:   Led_PPInter.c
 * Author: ANDER
 *
 * Created on 2 de octubre de 2025, 8:41
 */
#include <xc.h>
//#include <pic18f4550.h>

#pragma config FOSC = INTOSCIO_EC   // Oscilador interno
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config PBADEN = OFF
#pragma config MCLRE = OFF

#define _XTAL_FREQ 8000000

// Interrupción INT0
void __interrupt() ISR(void) {
    if (INTCONbits.INT0IF) {
        LATDbits.LATD0 = !LATDbits.LATD0;   // Toggle LED
        INTCONbits.INT0IF = 0;              // Limpiar bandera
    }
}

void main(void) {
    OSCCONbits.IRCF = 0b111;   // Reloj interno a 8MHz
    OSCCONbits.SCS = 0b10;     // Usar oscilador interno

    TRISBbits.TRISB0 = 1;      // RB0 como entrada (botón)
    TRISDbits.TRISD0 = 0;      // RD0 como salida (LED)
    LATDbits.LATD0 = 0;        // LED apagado al inicio
    ADCON1 = 0x0F;             // Todos los pines como digitales

    INTCON2bits.INTEDG0 = 0;   // Interrupción en flanco de bajada
    INTCONbits.INT0IF = 0;     // Limpiar bandera
    INTCONbits.INT0IE = 1;     // Habilitar INT0
    INTCONbits.GIE = 1;        // Habilitar interrupciones global

    while (1) {
        // Loop vacío: todo lo hace la ISR
    }
}


