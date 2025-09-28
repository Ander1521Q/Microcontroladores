;=========================================
;Encender Led con PIC18F4550
;=========================================

; PIC18F4550 Configuracion de bits

  CONFIG  WDT = OFF             ; Desactiva el Watchdog timer
  CONFIG  PBADEN = OFF          ; configura los PORTB como digitales
  CONFIG  LVP = OFF             ; Desactiva la programacion a bajo voltaje
  
#include <xc.inc> ;Incluir definiciones para el PIC18F4550
  
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
