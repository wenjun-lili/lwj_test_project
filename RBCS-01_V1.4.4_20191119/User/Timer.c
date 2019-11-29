#include "timer.h"


static void TIM7_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

static void TIM6_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 设置TIM6CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
    //TIM_DeInit(TIM6);

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = 999; // 1ms
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM6, TIM_FLAG_Update);
#if 0
    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);  /* 这条语句和上面那条作用是相同的 */
#endif

    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM6, DISABLE);

    TIM6_NVIC_Config();
}


/*
	功能：用定时器来实现精确延时
	参数：ms --- 延时时间	ms = 0 和 ms = 1效果相同
	NVIC_Config();
	TIM6_Config();
	for(;;)
	{
		LED1( ON );
		TIM6Delay_f(100);
		LED1( OFF );

		LED2( ON );
		TIM6Delay_f(100);
		LED2( OFF );

		LED3( ON );
		TIM6Delay_f(200);
		LED3( OFF );
	}
*/
TIM_Delay TIM6Delay_v = {0, 0};
void TIM6Delay_f(uint32_t ms)
{
    TIM6Delay_v.cnt = ms;
    TIM6Delay_v.flag = 0;
    TIMx_Enable(TIM6);
    while(!TIM6Delay_v.flag);
}


/* TIM7的定时周期10ms */
void TIM7_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 设置TIM6CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    //TIM_DeInit(TIM6);

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = 4999;  //500ms
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
#if 0
    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);  /* 这条语句和上面那条作用是相同的 */
#endif

    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM7, DISABLE);
// TIM_Cmd(TIM7, ENABLE);

    TIM7_NVIC_Config();
}


void TIMx_Enable(TIM_TypeDef* TIMx)
{
    TIMx->CNT = 0x0000;
    TIM_Cmd(TIMx, ENABLE);
}

void TIMx_Disable(TIM_TypeDef* TIMx)
{
    TIM_Cmd(TIMx, DISABLE);
}
