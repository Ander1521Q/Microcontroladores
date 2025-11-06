#include "gps.h"
#include <string.h>

void GPS_Init(void){
    TXSTA = 0b00100100;
    RCSTA = 0b10010000;
    SPBRG = 51;   // 9600 baudios @ 8MHz
}

bool GPS_Read(char *latitude, char *longitude){
    char buffer[80];
    uint8_t i = 0;

    while (RCIF == 0);
    while (RCREG != '$');

    while (i < 79){
        while (RCIF == 0);
        buffer[i] = RCREG;
        if(buffer[i] == '\n') break;
        i++;
    }
    buffer[i] = '\0';

    if(strstr(buffer, "GPGGA") == 0)
        return false;

    char *token = strtok(buffer, ",");
    uint8_t field = 0;

    while(token != NULL){
        field++;
        if(field == 3) strcpy(latitude, token);
        if(field == 5) strcpy(longitude, token);
        token = strtok(NULL, ",");
    }
    return true;
}
