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

  
;Incluir definiciones para el PIC18F4550
#include <xc.inc> 
  
  PSECT resetVec, class=CODE, reloc=2 ;Vector de reinicio
  
  ORG 0x00
  GOTO Inicio ;Va a Inicio
  
  PSECT main_code, class=CODE, reloc=2 ;Codigo principal
  
  Inicio:
    ;Configurar reloj interno a 8MHz
    MOVLW 0b01110110 ;valores binarios necesarios para el reloj de 8MHz
    MOVWF OSCCON ;Guarda en al variable OSCCON
    
    CLRF TRISB ;Configura los PORTB como salida
    CLRF LATB ;Configura PORTB en 0
    GOTO Encendido ;Va a Encendido
    
  Encendido:
    BSF LATB, 0 ;Coloca 5v en RB0
    MOVLW 5 ;Carga 5 en W
    MOVWF ContadorSegs ;Guarda 5 en la variable ContadorSegs
    
 EncendidoLoop:
    CALL Retardo_1s ;Llama a la subrutina retardo_1s
    DECFSZ ContadorSegs, F ;Decrementa ContadorSegs, si llega a 0 salta a la siguiente
    GOTO EncendidoLoop ;Si no llega a 0, repite
    GOTO Apagado ;Va a la rutina apagado, cuando llega a 0
    
 Apagado:
    BCF LATB, 0 ;Apaga el Led, RB0 en 0v
    MOVLW 2 ;Carga 2 en W
    MOVWF ContadorSegs ;Guarda el 2 en la variable ContadorSegs
    
 ApagadoLoop:
    CALL Retardo_1s ;llama al retardo_1s
    DECFSZ ContadorSegs, F ;Decrementa ContadorSegs, si llega a 0 salta
    GOTO ApagadoLoop ;Si no es 0, repite el bucle
    GOTO Encendido ;Cuando termina vuelve a encender el Led
; === Retardo 1s ====
  Retardo_1s:
    MOVLW 6 ;Carga 4 en w (contador externo)
    MOVWF ContadorExterno ;Guardaen la variable ContadorExterno
    
  LoopExterno:
    MOVLW 250 ;Carga 257 en W (contador medio)
    MOVWF ContadorInterno ; Guarda en la variable ContadorExterno 
    
  LoopMedio:
    MOVLW 250 ;Carga 300 en W (contador interno)
    MOVWF ContadorMedio
    
  LoopInterno:
    NOP ;Consume un ciclo
    NOP ;Consume un ciclo
    NOP ;Consume un ciclo
    DECFSZ ContadorInterno, F ;Decrementa el contador interno, si no es 0 repite
    GOTO LoopInterno ;Repite hasta llegar a 0
    DECFSZ ContadorMedio, F ;Decrementa el contador Medio, si no es 0 repite
    GOTO LoopMedio ;Repite hasta llegar a 0
    DECFSZ ContadorExterno, F ;Cuando Interno llega a 0, decrementa el externo
    GOTO LoopExterno ;Si no es 0, repite el externo
    
    RETURN ;Cuando ambos llegan a 0 se regresa
    
    PSECT udata
    ContadorExterno: DS 1 ;Reserva 1 byte para el contador externo
    ContadorMedio: DS 1 ;Reserva 1 byte para el contador medio
    ContadorInterno: DS 1 ;Reserva 1 byte para el contador interno
    ContadorSegs: DS 1 ;Reserva 1 byte para los segundos
 
  END ;Fin del codigo
   