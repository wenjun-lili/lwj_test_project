#ifndef __EVALVE_H
#define __EVALVE_H

#include "stm32f10x.h"

#define CLS_VO1() GPIO_ResetBits( GPIOC, GPIO_Pin_7 ) //PC7
#define CLS_VO2() GPIO_ResetBits( GPIOC, GPIO_Pin_6 ) //PC6
 

#define OPEN_VO1() GPIO_SetBits( GPIOC, GPIO_Pin_7 )
#define OPEN_VO2() GPIO_SetBits( GPIOC, GPIO_Pin_6 )
 

extern void evalveInit(void);//初始化电磁阀
extern void evalveAllClose( void );//关闭所有电磁阀

void evalveClose(uint8_t evalveFlag);//关闭电磁阀
void evalveOpen(uint8_t evalveFlag);//打开电磁阀


#endif	/* __EVALVE_H */
