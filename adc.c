#include "adc.h"

void ADC_Init(void){
    ADCON1 = 0x0D;   // AN0 analog - referencias VDD/VSS
    ADCON2 = 0xA9;   // Justificado a la derecha / Tacq / Fosc/8
    ADCON0 = 0x01;   // Habilitar ADC
}

uint16_t ADC_Read(uint8_t channel){
    ADCON0 = (channel << 2) | 0x01;
    __delay_us(10);
    GO_DONE = 1;
    while(GO_DONE);
    return (ADRESH << 8) | ADRESL;
}
