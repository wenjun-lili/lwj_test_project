#ifndef _PUMP_H
#define _PUMP_H

#include "stm32f10x.h"

/************高级定时器TIM参数定义，只限TIM1和TIM8************/
// 当使用不同的定时器的时候，对应的GPIO是不一样的，这点要注意
// 这里我们使用高级控制定时器TIM8

#define            ADVANCE_TIM                   TIM8
#define            ADVANCE_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            ADVANCE_TIM_CLK               RCC_APB2Periph_TIM8
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            ADVANCE_TIM_PERIOD            (100-1)
#define            ADVANCE_TIM_PSC               (48-1)
#define            ADVANCE_TIM_PULSE             100

#define            ADVANCE_TIM_IRQ               TIM1_UP_IRQn
#define            ADVANCE_TIM_IRQHandler        TIM1_UP_IRQHandler

// TIM8 输出通道
#define            ADVANCE_TIM_CH3_GPIO_CLK      RCC_APB2Periph_GPIOC
#define            ADVANCE_TIM_CH3_PORT          GPIOC
#define            ADVANCE_TIM_CH3_PIN           GPIO_Pin_8
#define            ADVANCE_TIM_CH4_PIN           GPIO_Pin_9   //实际上没用到ch4


/**************************函数声明********************************/

void ADVANCE_TIM_Init(void);

#endif	/* __BSP_ADVANCETIME_H */


