
#ifndef __NUM2STR_H
#define __NUM2STR_H

#include <stdio.h>


extern char Num2StrTmp[16];

char *Num2StrI(int val, const char *format);
char *Num2StrF(float val, const char *format);


#endif
