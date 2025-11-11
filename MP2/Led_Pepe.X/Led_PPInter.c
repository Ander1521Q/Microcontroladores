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

// Variables globales
volatile unsigned char secuencia = 0;       // Secuencia actual
volatile unsigned char modoVelocidad = 0;   // Modo de velocidad
volatile unsigned int velocidad = 300;      // Velocidad inicial

// --- Función de retardo variable ---
void delay_ms_variable(unsigned int tiempo) {
    while (tiempo--) {
        __delay_ms(1);
    }
}

// Interrupción INT0
void __interrupt() ISR(void) {
    if (INTCONbits.INT0IF) {
        secuencia++;
        if (secuencia > 3) secuencia = 0;  //Volver a la secuencia iniial
        INTCONbits.INT0IF = 0;              // Limpiar bandera
    }
    if (INTCON3bits.INT1IF) {
        modoVelocidad++;
        if (modoVelocidad > 5) modoVelocidad = 0;  // Reiniciar ciclo

        switch (modoVelocidad) {
            case 0: velocidad = 1500; break; // Muy lento
            case 1: velocidad = 1200;  break; // Lento
            case 2: velocidad = 900;  break; // Rápido
            case 3: velocidad = 600;  break; // Turbo
            case 4: velocidad = 300;  break; // Turbo
            case 5: velocidad = 150;  break; // Turbo
        }

        INTCON3bits.INT1IF = 0; // Limpiar bandera
    }
    
}

void main(void) {
    OSCCONbits.IRCF = 0b111;   // Reloj interno a 8MHz
    OSCCONbits.SCS = 0b10;     // Usar oscilador interno
    
    TRISD = 0x0F;
    LATD = 0x00;
            
    TRISBbits.TRISB0 = 1;      // RB0 como entrada (botón)
    TRISBbits.TRISB1 = 1;      // RB1 como entrada (botón)
    ADCON1 = 0x0F;             // Todos los pines como digitales

    //INT0
    INTCON2bits.INTEDG0 = 0;   // Interrupción en flanco de bajada
    INTCONbits.INT0IF = 0;     // Limpiar bandera
    INTCONbits.INT0IE = 1;     // Habilitar INT0
    INTCONbits.GIE = 1;        // Habilitar interrupciones global

    //INT1
    INTCON2bits.INTEDG1 = 0;
    INTCON3bits.INT1IF = 0;
    INTCON3bits.INT1IE = 1;
    
    while (1) {
        switch (secuencia) {
            case 0: // Secuencia 1: correr de izquierda a derecha
                LATD = 0b0001; delay_ms_variable(velocidad);
                LATD = 0b0010; delay_ms_variable(velocidad);
                LATD = 0b0100; delay_ms_variable(velocidad);
                LATD = 0b1000; delay_ms_variable(velocidad);
                break;

            case 1: // Secuencia 2: ida y vuelta
                LATD = 0b0001; delay_ms_variable(velocidad);
                LATD = 0b0010; delay_ms_variable(velocidad);
                LATD = 0b0100; delay_ms_variable(velocidad);
                LATD = 0b1000; delay_ms_variable(velocidad);
                LATD = 0b0100; delay_ms_variable(velocidad);
                LATD = 0b0010; delay_ms_variable(velocidad);
                break;

            case 2: // Secuencia 3: efecto Knight Rider
                LATD = 0b0011; delay_ms_variable(velocidad);
                LATD = 0b0110; delay_ms_variable(velocidad);
                LATD = 0b1100; delay_ms_variable(velocidad);
                LATD = 0b0110; delay_ms_variable(velocidad);
                break;

            case 3: // Secuencia 4: encendido progresivo
                LATD = 0b0001; delay_ms_variable(velocidad);
                LATD = 0b0011; delay_ms_variable(velocidad);
                LATD = 0b0111; delay_ms_variable(velocidad);
                LATD = 0b1111; delay_ms_variable(velocidad);
                LATD = 0b0111; delay_ms_variable(velocidad);
                LATD = 0b0011; delay_ms_variable(velocidad);
                LATD = 0b0001; delay_ms_variable(velocidad);
                LATD = 0b0000; delay_ms_variable(velocidad);
                break;
        }
        
    }
}


