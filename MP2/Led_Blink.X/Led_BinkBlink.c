#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <pic18f4550.h>

#pragma config FOSC = INTOSC_EC   // Oscilador interno
#pragma config FCMEN = OFF
#pragma config PWRT = OFF
#pragma config BOR = ON
#pragma config WDT = OFF
#pragma config LVP = OFF
#pragma config CP0 = OFF
#pragma config WRT0 = OFF

#define _XTAL_FREQ 8000000  // Frecuencia de oscilador (8 MHz)

void main(void) {
    OSCCON = 0x72;       // Configura reloj interno a 8 MHz
    TRISBbits.TRISB0 = 0; // RB0 como salida
    LATBbits.LATB0 = 0;   // LED apagado al inicio

    while(1) {
        LATBbits.LATB0 = 1;   // LED ON
        __delay_ms(1000);     // 1000 ms encendido

        LATBbits.LATB0 = 0;   // LED OFF
        __delay_ms(2000);     // 2000 ms apagado
    }
}
