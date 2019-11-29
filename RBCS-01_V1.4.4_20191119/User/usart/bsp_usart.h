#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>


extern void USART1_Config(void);
extern void USART1_Timeout_UserCallback(void);

#if 1

extern int fputc(int ch, FILE *f);
extern int fgetc(FILE *f);

#endif
#endif /* __USART1_H */


 
 
