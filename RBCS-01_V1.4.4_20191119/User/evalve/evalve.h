
#ifndef __EVALVE_H
#define __EVALVE_H

#include "stm32f10x.h"

#define CLS_VO1() GPIO_ResetBits( GPIOF, GPIO_Pin_15 )//PF15
#define CLS_VO2() GPIO_ResetBits( GPIOG, GPIO_Pin_0 ) //PG0
#define CLS_VO3() GPIO_ResetBits( GPIOG, GPIO_Pin_1 ) //PG1
#define CLS_VO4() GPIO_ResetBits( GPIOE, GPIO_Pin_11 )//PE11
#define CLS_VO5() GPIO_ResetBits( GPIOE, GPIO_Pin_12 )//pE12
#define CLS_VO6() GPIO_ResetBits( GPIOE, GPIO_Pin_13 )//PE13
#define CLS_VO7() GPIO_ResetBits( GPIOE, GPIO_Pin_14 )//PE14
 

#define OPEN_VO1() GPIO_SetBits( GPIOF, GPIO_Pin_15 )
#define OPEN_VO2() GPIO_SetBits( GPIOG, GPIO_Pin_0 )
#define OPEN_VO3() GPIO_SetBits( GPIOG, GPIO_Pin_1 )
#define OPEN_VO4() GPIO_SetBits( GPIOE, GPIO_Pin_11 )
#define OPEN_VO5() GPIO_SetBits( GPIOE, GPIO_Pin_12 )
#define OPEN_VO6() GPIO_SetBits( GPIOE, GPIO_Pin_13 )
#define OPEN_VO7() GPIO_SetBits( GPIOE, GPIO_Pin_14 )
 

extern void EValve_Init(void);
extern void EValve_All_Close( void );

void EV_Close(uint8_t evalveflag);
void EV_Open(uint8_t evalveflag);


#endif	/* __EVALVE_H */
