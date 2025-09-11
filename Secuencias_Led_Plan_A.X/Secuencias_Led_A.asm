;==============================================================
;Secuencia_Led_A Codio para PIC18F4550
;3 Secuencias distintas ,25 leds Y 1 Boton en RE0
;Oscilador interno de 8MHz
;==============================================================
;configuracion de Matriz 5x5
;Fila 1: RA0, RA1, RA2, RA3, RA4
;Fila 2: RA5, RE1, RB0, RB1, RB2
;Fila 3: RB3, RB4, RB5, RB6, RB7
;Fila 4: RC0, RC1, RC2, RD4, RE2
;Fila 5: RC6, RC7, RD5, RD6, RD7 
;==============================================================
    
;Configuracion de bits para PIC18F4550
    
  CONFIG  FOSC = INTOSC_EC ; Configuracion para utilizar reloj interno    
  CONFIG  WDT = OFF ;Desactiva el Watchdog timer       
  CONFIG  PBADEN = OFF ;Configura PortB como digitales      
  CONFIG  LVP = OFF ;Desactiva programacion a bajo voltaje

;Incluir definiciones para el PIC18F4550
#include <xc.inc>

    PSECT resetVec, class=CODE, reloc=2 ;Vector de reinicio
    ORG 0X00 ;Vector reset
    GOTO Inicio ;Va a inicio
    
    PSECT main_code, class=CODE, reloc=2 ;Codigo principal
    
    Inicio:
	MOVLW 0b01110010 ;Configurar reloj a 8Mhz
	MOVWF OSCCON
	
	CLRF ADCON1 ;Desactivar entradas analogicas
	MOVLW 0x00001111 ;Configutacion de bits para configurar la salida de los puertos
	MOVWF ADCON1 ;Poner todos los pines como Digitales
	
	CLRF TRISA
	CLRF TRISB ;Configuta PORTB como salida
	CLRF TRISC
	CLRF TRISD
	
	BSF TRISE, 0 ; Configura RE0 como entrada
	BCF TRISE, 1 ; Configura RE1 como salida
	BCF TRISE, 2 ; Configura RE2 como salida
	
	CLRF LATA
	CLRF LATB ;PORTB en 0
	CLRF LATC
	CLRF LATD
	CLRF LATE
	
	
	
    Boton_Loop:
	
	GOTO  LoopHola ;Comienza ejecutando la primera secuencia de palabra
	
    LoopHola:
	;Secuencia de ejecucion de la palabra HOLA 
	
	CALL Letra_H ;Se llama a la letra indicada
	CALL Retardo_1s ;se hace un retardo con los leds encendidos
	CALL Apagar_Leds ; se apagan los leds
	CALL Letra_H1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_H2
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_H3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_O
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_O1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_O2
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_O3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_O4
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_L
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_L1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_L2
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_L3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_L4
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra_A
	CALL Retardo_1s
	CALL Apagar_Leds
	
	BTFSS PORTE, 0 ; Revisa el boton en RE0
	GOTO LoopHola ; Si no esta precionado sigue el bucle
	GOTO LoopLeo ; Si es precionado avanza al siguiente bucle
	
    LoopLeo:
	;parecido al anterior
	CALL Letra2_L
	CALL Retardo_1s
	CALL Apagar_Leds
	CALL Letra2_L1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_L3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_L4
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_E
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_E1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_E2
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_E3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_O
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra2_O1
	CALL Retardo_1s
	CALL Apagar_Leds
	
	BTFSS PORTE, 0
	GOTO LoopLeo
	GOTO LoopMau
	
    LoopMau:
	;parecido al anterior
	CALL Letra3_M
	CALL Retardo_1s
	CALL Apagar_Leds
	CALL Letra3_M1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_M2
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_M3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_M4
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_A
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_A1
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_A2
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_A3
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_U
	CALL Retardo_300ms
	CALL Apagar_Leds
	CALL Letra3_U1
	CALL Retardo_1s
	CALL Apagar_Leds
	
	BTFSS PORTE, 0
	GOTO LoopMau
	GOTO LoopHola
	
    Apagar_Leds:
	;apaga todos los leds
	CLRF LATA ;PORTA en 0
	CLRF LATB
	CLRF LATC
	CLRF LATD
	BCF LATE, 1 ;PORTE 1 en 0
	BCF LATE, 2 ;PORTE 2 en 0
	
	RETURN ;Regresa donde quedo la ejecucion
	
    Letra_H:
	; --- FILA 1:01010
	BCF LATA, 0 ;apagado RA0
	BSF LATA, 1 ;Encendido RA1
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATE, 1
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
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01010
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra_H1:
	; --- FILA 1:10101
	BSF LATA, 0
	BCF LATA, 1 
	BSF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:10101
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:11101
	BSF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:10101
	BSF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:10101
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_H2:
	; --- FILA 1:01011
	BCF LATA, 0
	BSF LATA, 1
	BCF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:11010
	BSF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01010
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01011
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_H3:
	; --- FILA 1:10111
	BSF LATA, 0
	BCF LATA, 1
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:10100
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10100
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10100
	BSF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:10111
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_O:
	; --- FILA 1:01111
	BCF LATA, 0
	BSF LATA, 1
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:01001
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:01001
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:01001
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:01111
	BCF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_O1:
	; --- FILA 1:11110
	BSF LATA, 0
	BSF LATA, 1
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10010
	BSF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10010
	BSF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10010
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11110
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra_O2:
	; --- FILA 1:11101
	BSF LATA, 0
	BSF LATA, 1
	BSF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00101
	BCF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:00101
	BCF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00101
	BCF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:11101
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_O3:
	; --- FILA 1:11010
	BSF LATA, 0 
	BSF LATA, 1
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01010
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01010
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11011
	BSF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_O4:
	; --- FILA 1:10100
	BSF LATA, 0
	BCF LATA, 1
	BSF LATA, 2
	BCF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10100
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10100
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10100
	BSF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:10111
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_L:
	; --- FILA 1:01000
	BCF LATA, 0
	BSF LATA, 1
	BCF LATA, 2
	BCF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01000
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01000
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01000
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01110
	BCF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra_L1:
	; --- FILA 1:10000
	BSF LATA, 0
	BCF LATA, 1
	BCF LATA, 2
	BCF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10000
	BSF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10000
	BSF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10000
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11100
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra_L2:
	; --- FILA 1:00001
	BCF LATA, 0
	BCF LATA, 1
	BCF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00001
	BCF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:00001
	BCF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00001
	BCF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:11001
	BSF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_L3:
	; --- FILA 1:00011
	BCF LATA, 0
	BCF LATA, 1
	BCF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00010
	BCF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:00011
	BCF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00010
	BCF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:10010
	BSF LATC, 6
	BCF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra_L4:
	; --- FILA 1:00111
	BCF LATA, 0
	BCF LATA, 1
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00101
	BCF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:00111
	BCF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00101
	BCF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:00101
	BCF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra_A:
	; --- FILA 1:01110
	BCF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATE, 1
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
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01010
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra2_L:
	; --- FILA 1:01000
	BCF LATA, 0
	BSF LATA, 1 
	BCF LATA, 2
	BCF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01000
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01000
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01000
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01110
	BCF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra2_L1:
	; --- FILA 1:10000
	BSF LATA, 0
	BCF LATA, 1 
	BCF LATA, 2
	BCF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10000
	BSF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10000
	BSF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10000
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11100
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra2_L2:
	; --- FILA 1:00001
	BCF LATA, 0
	BCF LATA, 1 
	BCF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00001
	BCF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:00001
	BCF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00001
	BCF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:11001
	BSF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra2_L3:
	; --- FILA 1:00011
	BCF LATA, 0
	BCF LATA, 1 
	BCF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00010
	BCF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:00011
	BCF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00010
	BCF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:10011
	BSF LATC, 6
	BCF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra2_L4:
	; --- FILA 1:00111
	BCF LATA, 0
	BCF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00100
	BCF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:00111
	BCF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00100
	BCF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:00111
	BCF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra2_E:
	; --- FILA 1:01110
	BCF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01000
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01110
	BCF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01000
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01110
	BCF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra2_E1:
	; --- FILA 1:11101
	BSF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:10001
	BSF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:11101
	BSF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:10001
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:11101
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra2_E2:
	; --- FILA 1:11011
	BSF LATA, 0
	BSF LATA, 1 
	BCF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00010
	BCF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:11010
	BSF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:00010
	BCF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11011
	BSF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra2_E3:
	; --- FILA 1:10111
	BSF LATA, 0
	BCF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:00100
	BCF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10100
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:00100
	BCF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:10111
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra2_O:
	; --- FILA 1:01111
	BCF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:01001
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:01001
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:01001
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:01111
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra2_O1:
	; --- FILA 1:11110
	BSF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10010
	BSF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10010
	BSF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10010
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11110
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra3_M:
	; --- FILA 1:10001
	BSF LATA, 0
	BCF LATA, 1 
	BCF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:11011
	BSF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BSF LATB, 2
	; --- FILA 3:10101
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:10001
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:10001
	BSF LATC, 6
	BCF LATC, 7
	BCF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_M1:
	; --- FILA 1:00010
	BCF LATA, 0
	BCF LATA, 1 
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10110
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01010
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:00010
	BCF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:00010
	BCF LATC, 6
	BCF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra3_M2:
	; --- FILA 1:00101
	BCF LATA, 0
	BCF LATA, 1 
	BSF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:01101
	BCF LATA, 5
	BSF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:10101
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:00101
	BCF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:00101
	BCF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_M3:
	; --- FILA 1:01011
	BCF LATA, 0
	BSF LATA, 1 
	BCF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:11010
	BSF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:01011
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BSF LATB, 7
	; --- FILA 4:01010
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01010
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra3_M4:
	; --- FILA 1:10111
	BSF LATA, 0
	BCF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BSF LATA, 4
	; --- FILA 2:10101
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:10111
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BSF LATB, 6
	BSF LATB, 7
	; --- FILA 4:10101
	BSF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:10101
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_A:
	; --- FILA 1:01110
	BCF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATE, 1
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
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01010
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
    Letra3_A1:
	; --- FILA 1:11101
	BSF LATA, 0
	BSF LATA, 1 
	BSF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:10101
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:11101
	BSF LATB, 3
	BSF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:10101
	BSF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:10101
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BCF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_A2:
	; --- FILA 1:11010
	BSF LATA, 0
	BSF LATA, 1 
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:01010
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:11010
	BSF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:01010
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:01011
	BCF LATC, 6
	BSF LATC, 7
	BCF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_A3:
	; --- FILA 1:10100
	BSF LATA, 0
	BCF LATA, 1 
	BSF LATA, 2
	BCF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10100
	BSF LATA, 5
	BCF LATE, 1
	BSF LATB, 0
	BCF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10100
	BSF LATB, 3
	BCF LATB, 4
	BSF LATB, 5
	BCF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10100
	BSF LATC, 0
	BCF LATC, 1
	BSF LATC, 2
	BCF LATD, 4
	BCF LATE, 2
	; --- FILA 5:10111
	BSF LATC, 6
	BCF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_U:
	; --- FILA 1:01001
	BCF LATA, 0
	BSF LATA, 1 
	BCF LATA, 2
	BCF LATA, 3
	BSF LATA, 4
	; --- FILA 2:01001
	BCF LATA, 5
	BSF LATE, 1
	BCF LATB, 0
	BCF LATB, 1
	BSF LATB, 2
	; --- FILA 3:01001
	BCF LATB, 3
	BSF LATB, 4
	BCF LATB, 5
	BCF LATB, 6
	BSF LATB, 7
	; --- FILA 4:01001
	BCF LATC, 0
	BSF LATC, 1
	BCF LATC, 2
	BCF LATD, 4
	BSF LATE, 2
	; --- FILA 5:01111
	BCF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BSF LATD, 7
	
	RETURN
	
    Letra3_U1:
	; --- FILA 1:10010
	BSF LATA, 0
	BCF LATA, 1 
	BCF LATA, 2
	BSF LATA, 3
	BCF LATA, 4
	; --- FILA 2:10010
	BSF LATA, 5
	BCF LATE, 1
	BCF LATB, 0
	BSF LATB, 1
	BCF LATB, 2
	; --- FILA 3:10010
	BSF LATB, 3
	BCF LATB, 4
	BCF LATB, 5
	BSF LATB, 6
	BCF LATB, 7
	; --- FILA 4:10010
	BSF LATC, 0
	BCF LATC, 1
	BCF LATC, 2
	BSF LATD, 4
	BCF LATE, 2
	; --- FILA 5:11110
	BSF LATC, 6
	BSF LATC, 7
	BSF LATD, 5
	BSF LATD, 6
	BCF LATD, 7
	
	RETURN
	
;------------------------------------------------------
; Subrutina Retardo de 1 segundo
;------------------------------------------------------
    Retardo_1s:
	MOVLW 8 ;Carga 8 en W
	MOVWF ContadorExterno ;La carga en ContadorExterno

    LoopExterno:
	MOVLW 200 ;Carga 200 en W
	MOVWF ContadorMedio ;La carga en ContadorMedio

    LoopMedio:
	MOVLW 250 ;Carga 250 en W
	MOVWF ContadorInterno ;La carga en ContadorInterno

    LoopInterno:
	NOP ;Consume un ciclo
	NOP
	NOP
	DECFSZ ContadorInterno, F ;Decrementa el ContadorInterno(si no es 0, repite)
	GOTO LoopInterno ;Va a LoopInterno y repite hasta llegar a 0
	DECFSZ ContadorMedio, F ;Decrementa el ContadorMedio(si no es 0, repite)
	GOTO LoopMedio ;Va a LoopMedio y repite hasta llegar a 0
	DECFSZ ContadorExterno, F ;Decrementa el ContadorExterno(si no es 0, repite)
	GOTO LoopExterno ;Va a LoopExterno y repite hasta llegar a 0

	RETURN ;Cuando termina, regresa donde quedo la ejecucion
	
;----------------------------------------------------------
; Subrutina Retardo de 250ms segundo (Parecido al de 1s)
;----------------------------------------------------------
    Retardo_300ms:
	MOVLW 3
	MOVWF ContadorExterno

    LoopExterno300:
	MOVLW 200
	MOVWF ContadorMedio

    LoopMedio300:
	MOVLW 200
	MOVWF ContadorInterno

    LoopInterno300:
	;NOP
	NOP ;Consume un ciclo
	NOP
	DECFSZ ContadorInterno, F
	GOTO LoopInterno300
	DECFSZ ContadorMedio, F
	GOTO LoopMedio300
	DECFSZ ContadorExterno, F
	GOTO LoopExterno300

	RETURN  
	
	
    PSECT udata ;Deficnion de variables
      ContadorExterno: DS 1 ;variable para bucle externo
      ContadorMedio:   DS 1 ;variable para bucle medio
      ContadorInterno: DS 1 ;variable para bucle interno

    END  