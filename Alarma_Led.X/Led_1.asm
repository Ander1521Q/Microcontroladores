
; PIC18F4550 Configuration Bit Settings

; Assembly source line config statements

  CONFIG  FOSC = INTOSC_EC      ; Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
  
// config statements should precede project file includes.
#include <xc.inc>
  PSECT  resetVec, class=CODE, reloc=2  ; Sección para el vector de reinicio
  
  ORG 0X00
  GOTO Inicio
  
  PSECT  main_code, class=CODE, reloc=2  ; Sección de código principal
  
  Inicio:
    CLRF    TRISB             ; Configura todo PORTB como salida
    CLRF    LATB              ; Limpia las salidas en 0v
    
  LOOP:
    BSF     LATB,0            ; Enciende LED en RB0
    GOTO    LOOP              ; Bucle infinito

    END