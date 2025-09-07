;=================================================================
;Secuencia_Led_B Codio para PIC18F4550
;3 Secuencias distintas, 4 leds en RB0 - RB3 y 1 Boton en RB4
;Oscilador interno de 8MHz
;=================================================================
    
;Configuracion de bits para PIC18F4550
    
  CONFIG  FOSC = INTOSC_EC      ; Oscillator Selection bits (Internal oscillator, CLKO function on RA6, EC used by USB (INTCKO))
  CONFIG  WDT = OFF             ; Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit)
  CONFIG  PBADEN = OFF          ; PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
  CONFIG  LVP = OFF             ; Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)

;Incluir definiciones para el PIC18F4550
#include <xc.inc>

    PSECT resetVec, class=CODE, reloc=2 ;Vector de reinicio
    ORG 0X00 ;Vector reset
    GOTO Inicio ;Va a inicio
    
    PSECT main_code, class=CODE, reloc=2 ;Codigo principal
    
    Inicio:
    
	MOVLW 0b01110010 ;Configurar reloj a 8Mhz
	MOVWF OSCCON
	
	MOVLW 0B00010000
	MOVWF TRISB ;Configuta PORTB como salida
	CLRF LATB ;PORTB en 0
	
    Boton_Loop:
	
	BTFSC PORTB, 4
	GOTO Apagar_Leds
	;CALL Secuencia1
	CALL Secuencia2
	;CALL Secuencia3
	GOTO Boton_Loop ;Saltar a revisar si el boton esta presionado
	
    Apagar_Leds:
	CLRF LATB
	GOTO Boton_Loop
	
    ;Secuencia1:
	;MOVLW 0b00000001
	;MOVWF LATB
	;CALL Retardo_1s
	
	;MOVLW 0b00000011
	;MOVWF LATB
	;CALL Retardo_1s
	
	;MOVLW 0b00000111
	;MOVWF LATB
	;CALL Retardo_1s
	
	;MOVLW 0b00001111
	;MOVWF LATB
	;CALL Retardo_1s
	
	;CLRF LATB
	;CALL Retardo_1s
	;GOTO Secuencia1
	
    Secuencia2:
	MOVLW 0b00000001
	MOVWF LATB
	CALL Retardo_1s
	
	MOVLW 0b00000010
	MOVWF LATB
	CALL Retardo_1s
	
	MOVLW 0b00000100
	MOVWF LATB
	CALL Retardo_1s
	
	MOVLW 0b00001000
	MOVWF LATB
	CALL Retardo_1s
	
	MOVLW 0b00000100
	MOVWF LATB
	CALL Retardo_1s
	
	MOVLW 0b00000010
	MOVWF LATB
	CALL Retardo_1s
	
	CLRF LATB
	CALL Retardo_1s
	RETURN
	
    ;Secuencia3:
	;MOVLW 0b00000101
	;MOVWF LATB
	;CALL Retardo_1s
	
	;MOVLW 0b00001010
	;MOVWF LATB
	;CALL Retardo_1s
	
	;CLRF LATB
	;CALL Retardo_1s
	;GOTO Secuencia3

	
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