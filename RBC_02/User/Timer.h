
#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"

typedef struct {
    uint32_t cnt;	/* ��ʱ���ڸ��� �����ʱ�Ķ�ʱ������1ms, ��Ҫ��ʱ1s����cnt = 1000 */
    uint8_t flag;	/* ��ʱ��ɱ�־λ ��ʱʱ�䵽Ϊ1 */
} TIM_Delay;


//void TIM2_Config(void);


void TIM6_Config(void);
void TIM6Delay_f(uint32_t ms);
void TIM7_Config(void);


void TIMx_Enable(TIM_TypeDef* TIMx);
void TIMx_Disable(TIM_TypeDef* TIMx);

#endif	/* __TIMER_H */


