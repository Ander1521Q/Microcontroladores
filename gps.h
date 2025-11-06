#ifndef _GPS_H
#define _GPS_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>

void GPS_Init(void);
bool GPS_Read(char *latitude, char *longitude);

#endif
