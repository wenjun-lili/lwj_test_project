
#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

typedef struct {
    uint32_t cnt;	/* 定时周期个数 如果定时的定时周期是1ms, 若要定时1s，则cnt = 1000 */
    uint8_t flag;	/* 定时完成标志位 定时时间到为1 */
} TIM_Delay;


//void TIM2_Config(void);


void TIM6_Config(void);
void TIM6Delay_f(uint32_t ms);
void TIM7_Config(void);


void TIMx_Enable(TIM_TypeDef* TIMx);
void TIMx_Disable(TIM_TypeDef* TIMx);

#endif	/* __TIMER_H */



