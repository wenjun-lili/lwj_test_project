#include "steppermotor.h"
#include "timer.h"
// 配置TIME2_CH3

static void Motor_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 设置TIM2CLK 为 72MHZ */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* GPIOA clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG, ENABLE);

    /* 配置motor1的GPIO PA2时钟引脚  PG8方向引脚  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // K_L
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  // K_R
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // 复用推挽输出
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
//static void Motor_NVIC_Config(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;


//	/* 配置TIM2_IRQ中断为中断源 */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}


static void TIM2_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    /* -----------------------------------------------------------------------
    TIM2 Channel3 duty cycle = (TIM3_CCR/ TIM3_ARR+1)* 100% = 50%（占空比）
    PWM周期为 = 1/(72MHZ /72) * (TIM3_ARR+1)
    ----------------------------------------------------------------------- */

    /* Time base configuration */

    TIM_TimeBaseStructure.TIM_Period = 499;    // 500次 *1us=0.5ms
    TIM_TimeBaseStructure.TIM_Prescaler = 71;	    /* 设置预分频：72分频，即为1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	/* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    /* 配置为PWM模式1 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 249;	   /* 设置跳变值，当计数器计数到这个值时，电平发生跳变 */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low ;  /* 当定时器计数值小于CCR_Val时为高电平 */
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 /* 使能通道3 */
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);			 /* 使能TIM2重载寄存器ARR */

//	TIM_ITConfig(TIM2,TIM_IT_CC3, ENABLE);		 /* 使能比较中断 */

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, DISABLE);                   /* 使能定时器2 */
//TIM_Cmd(TIM2, ENABLE);

}


void MotorInit(void)
{
    Motor_GPIO_Config();
//Motor_NVIC_Config();
    TIM2_Mode_Config();

//	GPIO_ResetBits(GPIOG, GPIO_Pin_5);// 关闭使能
//
////	GPIO_SetBits(GPIOG, GPIO_Pin_3);// 全/半步进控制
//	GPIO_ResetBits(GPIOG, GPIO_Pin_3);


}


void Motor1Start(uint8_t dir, uint16_t speed_us)  // 电机开启
{

// 	GPIO_SetBits(GPIOG, GPIO_Pin_5); //MOTOR1使能
//  GPIO_ResetBits(GPIOG, GPIO_Pin_5); //MOTOR1使能

    if(dir == 0)  // 正转
    {
        CW_M1();
    }
    else if(dir == 1) // 反转
    {
        CCW_M1();
    }

    TIM2->ARR = speed_us;

    TIM2->CCR3= speed_us/2;
//	TIM2->CNT = 0x0000;
    TIM_Cmd(TIM2, ENABLE);
}

void Motor1Stop(void)
{
    TIM_Cmd(TIM2, DISABLE); // TIMER2_CH3 开启
//  GPIO_ResetBits(GPIOG, GPIO_Pin_5); //失能
    //2017.6.20 电机停下后延时
    TIM6Delay_f(1500);// 等待1S

}
