;==============================================================
;Secuencia_Led_A Codio para PIC18F4550
;3 Secuencias distintas ,25 leds Y 1 Boton en RE0
;Oscilador interno de 8MHz
;==============================================================
;configuracion de Matriz 5x5
;Fila 1: RA0, RA1, RA2, RA3, RA4
;Fila 2: RA5, RA6, RB0, RB1, RB2
;Fila 3: RB3, RB4, RB5, RB6, RB7
;Fila 4: RC0, RC1, RC2, RD4, RC5
;Fila 5: RC6, RC7, RD5, RD6, RD7 
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
	
    Letra_H:
	; --- FILA 1:01010
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BCF LATA, 2
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
	BCF LATD, 3
	
    Letra_H1:
	; --- FILA 1:10101
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10101
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:11101
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10101
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATC, 4
	BCF LATC, 5
	; --- FILA 5:10101
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 3
	
    Letra_H2:
	; --- FILA 1:01011
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:11010
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
	; --- FILA 5:01011
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 3
	
    Letra_H3:
	; --- FILA 1:10111
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10100
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10100
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10100
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATC, 4
	BCF LATC, 5
	; --- FILA 5:10111
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 3
	
    Letra_O:
	; --- FILA 1:01111
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01001
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01001
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01001
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATC, 4
	BCF LATC, 5
	; --- FILA 5:01111
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 3
	
    Letra_O1:
	; --- FILA 1:11110
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10010
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10010
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10010
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATC, 4
	BCF LATC, 5
	; --- FILA 5:11110
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 3
	
    Letra_L:
	; --- FILA 1:01000
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01000
	BCF LATA, 5
	BSF LATA, 6
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01000
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01000
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATC, 4
	BCF LATC, 5
	; --- FILA 5:01110
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 0
	BSF LATD, 1
	BCF LATD, 3
	
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
	BCF LATD, 3
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
	
;------------------------------------------------------
; Subrutina Retardo de 250ms segundo
;------------------------------------------------------
    Retardo_250ms:
	MOVLW 2
	MOVWF ContadorExterno

    LoopExterno250:
	MOVLW 200
	MOVWF ContadorMedio

    LoopMedio250:
	MOVLW 250
	MOVWF ContadorInterno

    LoopInterno250:
	;NOP
	NOP
	NOP
	DECFSZ ContadorInterno, F
	GOTO LoopInterno250
	DECFSZ ContadorMedio, F
	GOTO LoopMedio250
	DECFSZ ContadorExterno, F
	GOTO LoopExterno250

	RETURN  
	
	
    PSECT udata ;Deficnion de variables
      ContadorExterno: DS 1
      ContadorMedio:   DS 1
      ContadorInterno: DS 1

    END  