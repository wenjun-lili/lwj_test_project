


#ifndef __UTILITY_H
#define __UTILITY_H
#include "stm32f10x.h"
#ifndef  OS_CPU_H
/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (数据类型定义与UCOS兼容)
*********************************************************************************************************
*/

typedef unsigned char  BOOLEAN;
typedef unsigned char  INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char  INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int   INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int   INT32S;                   /* Signed   32 bit quantity                           */
typedef float          FP32;                     /* Single precision floating point                    */
typedef double         FP64;                     /* Double precision floating point                    */

#endif	/* OS_CPU_H */


typedef unsigned char uchar;
typedef unsigned int uint;


/* 延时程序，纯软延时，不使用中断 -------------------------------------*/
void Delay(unsigned int nCount);
void Delay1us(unsigned int Counter);
void Delay100us(unsigned int Counter);
void Delay1ms(unsigned int Counter);
void Delay10ms(unsigned int Counter);
void Delay100ms(unsigned int Counter);

#endif /*  UTILITY_H */
