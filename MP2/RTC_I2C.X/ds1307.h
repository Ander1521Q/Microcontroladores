#ifndef DS1307_H
#define DS1307_H

void RTC_Init(void);
void RTC_SetDateTime(unsigned char h, unsigned char m, unsigned char s,
                     unsigned char d, unsigned char mo, unsigned char y);
void RTC_GetDateTime(unsigned char *hour, unsigned char *min, unsigned char *sec,
                     unsigned char *day, unsigned char *month, unsigned char *year);
unsigned char BCD2DEC(unsigned char val);
unsigned char DEC2BCD(unsigned char val);

#endif
