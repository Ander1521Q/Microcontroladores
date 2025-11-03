#ifndef DS1307_H
#define DS1307_H

#include <xc.h>
#include "i2c.h"
#include "Config.h"

#define DS1307_ADDRESS 0xD0

void DS1307_Init(void);
void DS1307_SetTime(unsigned char h, unsigned char m, unsigned char s);
void DS1307_SetDate(unsigned char d, unsigned char mth, unsigned char y);
void DS1307_GetTime(unsigned char *h, unsigned char *m, unsigned char *s);
void DS1307_GetDate(unsigned char *d, unsigned char *mth, unsigned char *y);

#endif

