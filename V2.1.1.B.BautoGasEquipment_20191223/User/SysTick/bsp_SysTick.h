#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Delay_Us(u32 nus);
void SysTick_Delay_Ms(u16 nms);
void delay_init(void);

#endif /* __SYSTICK_H */
