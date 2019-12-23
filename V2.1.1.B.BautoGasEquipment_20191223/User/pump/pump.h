#ifndef _PUMP_H
#define _PUMP_H

#include "stm32f10x.h"

/************�߼���ʱ��TIM�������壬ֻ��TIM1��TIM8************/
// ��ʹ�ò�ͬ�Ķ�ʱ����ʱ�򣬶�Ӧ��GPIO�ǲ�һ���ģ����Ҫע��
// ��������ʹ�ø߼����ƶ�ʱ��TIM8

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8
// PWM �źŵ�Ƶ�� F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (100-1)
#define            ADVANCE_TIM_PSC               (48-1)
#define            ADVANCE_TIM_PULSE             100

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM8 ���ͨ��
#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH3_PORT          GPIOC
#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_8
#define            ADVANCE_TIM_CH4_PIN           GPIO_Pin_9   //ʵ����û�õ�ch4


/**************************��������********************************/

void ADVANCE_TIM_Init(void);

#endif	/* __BSP_ADVANCETIME_H */


