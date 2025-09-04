;==============================================================
;Secuencia_Led_A Codio para PIC18F4550
;3 Secuencias distintas y 25 leds casi todos los puertos XD
;Oscilador interno de 8MHz
;==============================================================
;configuracion de Matriz 5x5
;Fila 1: RA0, RA1, RA2, RA3, RA4
;Fila 2: RA5, RA6, RB0, RB1, RB2
;Fila 3: RB3, RB4, RB5, RB6, RB7
;Fila 4: RC0, RC1, RC2, RC4, RC5
;Fila 5: RC6, RC7, RD0, RD1, RD2
;==============================================================
    
;Configuracion de bits para PIC18F4550
    
  CONFIG  FOSC = INTOSC_EC     
  CONFIG  WDT = OFF             
  CONFIG  PBADEN = OFF          
  CONFIG  LVP = OFF             

;Incluir definiciones para el PIC18F4550
#include <xc.inc>

    PSECT resetVec, class=CODE, reloc=2 ;Vector de reinicio
    ORG 0X00 ;Vector reset
    GOTO Inicio ;Va a inicio
    
    PSECT main_code, class=CODE, reloc=2 ;Codigo principal
    
    Inicio:
	MOVLW 0b01110010 ;Configurar reloj a 8Mhz
	MOVWF OSCCON
	
	CLRF TRISA
	CLRF TRISB ;Configuta PORTB como salida
	CLRF TRISC
	CLRF TRISD
	
	CLRF LATA
	CLRF LATB ;PORTB en 0
	CLRF LATC
	CLRF LATD
	
    LoopPrincipal:
	CALL Letra_A
	CALL Retardo_1s
	
	CLRF LATA
	CLRF LATB
	CLRF LATC
	CLRF LATD
	CALL Retardo_1s
	
	GOTO LoopPrincipal
	
    Letra_A:
	; --- FILA 1:01110
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01110
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01010
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATC, 4
	BCF LATC, 5
	; --- FILA 5:01010
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 2
;------------------------------------------------------
; Subrutina Retardo de 1 segundo
;------------------------------------------------------
    Retardo_1s:
	MOVLW 8
	MOVWF ContadorExterno

    LoopExterno:
	MOVLW 200
	MOVWF ContadorMedio

    LoopMedio:
	MOVLW 250
	MOVWF ContadorInterno

    LoopInterno:
	NOP
	NOP
	NOP
	DECFSZ ContadorInterno, F
	GOTO LoopInterno
	DECFSZ ContadorMedio, F
	GOTO LoopMedio
	DECFSZ ContadorExterno, F
	GOTO LoopExterno

	RETURN
	
	
    PSECT udata ;Deficnion de variables
      ContadorExterno: DS 1
      ContadorMedio:   DS 1
      ContadorInterno: DS 1

    END