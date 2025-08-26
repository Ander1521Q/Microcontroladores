;=======================================
;Codigo para PIC18F4550 en Assembler
;Led en RB0 encendido 5s y apagado 2s
;Oscilador interno a 8Mhz
;=======================================
; PIC18F4550 Configuracion de bits

  CONFIG  FOSC = INTOSC_EC      ;Utilizar oscilador interno de 8MHz
  CONFIG  WDT = OFF             ;Desactiva el Watchdog timer 
  CONFIG  PBADEN = OFF          ;Configura los PORTB como digitales
  CONFIG  LVP = OFF             ; Desactiva la programacion a bajo voltage

#include <xc.inc> ;Incluir definiciones para el PIC18F4550
  
  PSECT resetVec, class=CODE, reloc=2 ;Vector de reinicio
  
  ORG 0x00
  GOTO Inicio
  
  PSECT main_code, clss=CODE, reloc=2 ;Codigo principal
  
  Inicio
    CLRF TRISB
    CLRF LATB
    
  Loop
    BSF LATB, 0
    CALL Retardo_5s
    
    BCF LATB, 0
    CALL Retardo_2s
    
    GOTO Loop
    
;=====================================
;Subrutinas de retardos
;=====================================
; === Retardo 1s ====
  Retardo_1s:
    MOVLW 25
    MOVWF ContadorExterno
    
  LoopExterno:
    MOVLW 250
    MOVWF ContadorInterno
    
  LoopInterno:
    NOP
    NOP
    NOP
    DECFSZ ContadorInterno
    GOTO LoopInterno
    
    DECFSZ ContadorExterrno
    GOTO LoopExterno
    
    RETURN