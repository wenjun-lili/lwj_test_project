
#ifndef __EVALVE_H
#define __EVALVE_H

#include "stm32f10x.h"

#define CLS_VO1() GPIO_ResetBits( GPIOB, GPIO_Pin_0 ) //PB0
#define CLS_VO2() GPIO_ResetBits( GPIOB, GPIO_Pin_1 ) //PB1
#define CLS_VO3() GPIO_ResetBits( GPIOE, GPIO_Pin_1 ) //PE1
#define CLS_VO4() GPIO_ResetBits( GPIOE, GPIO_Pin_0 ) //PE0
#define CLS_VO5() GPIO_ResetBits( GPIOB, GPIO_Pin_9 ) //PB9
#define CLS_VO6() GPIO_ResetBits( GPIOB, GPIO_Pin_8 ) //PB8
#define CLS_VO7() GPIO_ResetBits( GPIOA, GPIO_Pin_8 ) //PA8


#define OPEN_VO1() GPIO_SetBits( GPIOB, GPIO_Pin_0 )
#define OPEN_VO2() GPIO_SetBits( GPIOB, GPIO_Pin_1 )
#define OPEN_VO3() GPIO_SetBits( GPIOE, GPIO_Pin_1 )
#define OPEN_VO4() GPIO_SetBits( GPIOE, GPIO_Pin_0 )
#define OPEN_VO5() GPIO_SetBits( GPIOB, GPIO_Pin_9 )
#define OPEN_VO6() GPIO_SetBits( GPIOB, GPIO_Pin_8 )
#define OPEN_VO7() GPIO_SetBits( GPIOA, GPIO_Pin_8 )


extern void EValve_Init(void);
extern void EValve_All_Close(void);
extern void EValve_All_Open(void);
void EV_Close(uint8_t evalveflag);
void EV_Open(uint8_t evalveflag);


#endif	/* __EVALVE_H */