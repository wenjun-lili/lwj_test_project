#ifndef __EVALVE_H
#define __EVALVE_H

#include "stm32f10x.h"

#define CLS_VO1() GPIO_ResetBits( GPIOC, GPIO_Pin_7 ) //PC7
#define CLS_VO2() GPIO_ResetBits( GPIOC, GPIO_Pin_6 ) //PC6
 

#define OPEN_VO1() GPIO_SetBits( GPIOC, GPIO_Pin_7 )
#define OPEN_VO2() GPIO_SetBits( GPIOC, GPIO_Pin_6 )
 

extern void evalveInit(void);//��ʼ����ŷ�
extern void evalveAllClose( void );//�ر����е�ŷ�

void evalveClose(uint8_t evalveFlag);//�رյ�ŷ�
void evalveOpen(uint8_t evalveFlag);//�򿪵�ŷ�


#endif	/* __EVALVE_H */
