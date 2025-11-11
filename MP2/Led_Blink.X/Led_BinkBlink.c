#include <xc.h>
//#include <pic18f4550.h>

#pragma config FOSC = INTOSC_EC   // Oscilador interno
#pragma config PBADEN = OFF
#pragma config WDT = OFF
#pragma config LVP = OFF

#define _XTAL_FREQ 8000000  // Frecuencia de oscilador (8 MHz)

void delay_ms_variable(unsigned int tiempo){
    while (tiempo--){
        __delay_ms(1);
    }
}

void blink(unsigned char veces, unsigned int tiempo_on, unsigned int tiempo_off){
    for (unsigned char i = 0; i < veces; i++){
        LATBbits.LATB0 = 1;
        delay_ms_variable(tiempo_on);
        LATBbits.LATB0 = 0;
        delay_ms_variable(tiempo_off);
        
    }
}
void main(void) {
    OSCCON = 0x72;       // Configura reloj interno a 8 MHz
    TRISBbits.TRISB0 = 0; // RB0 como salida
    LATBbits.LATB0 = 0;   // LED apagado al inicio
    
    unsigned long duracion_total = 20000;
    unsigned long duracion_ciclo = 5 * (500 + 500);
    
    while (duracion_total >= duracion_ciclo) {
        blink(5, 500, 500);         
        duracion_total -= duracion_ciclo;
    }

    blink(2, 1000, 1000);
    LATBbits.LATB0 = 0;

    while(1) {
        
    }
}
