#include "steppermotor.h"
#include "timer.h"

/**
  *@breif  步进电机引脚配置
	*@param  none
	*@retval none
	*/
static void Motor_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* 设置TIM3CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE);

    /* 配置方向引脚PG8*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /*配置左到位引脚PD5*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*配置右到位引脚PG7*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /*配置时钟引脚PA7*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  *@breif  TIM2定时器的配置
	*@param  none
	*@retval none
	*/
static void TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* -----------------------------------------------------------------------
    TIM3 Channel2 duty cycle = (TIM3_CCR/ TIM3_ARR+1)* 100% = 50%（占空比）
    PWM周期为 = 1/(72MHZ /72) * (TIM3_ARR+1)
    ----------------------------------------------------------------------- */

    TIM_TimeBaseStructure.TIM_Period = 499;    // 500次 *1us=0.5ms
    TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //设置预分频：72分频，即为1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	// 对外部时钟进行采样的时钟分频,这里没有用到
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	// 配置为PWM模式1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 249;	   //设置跳变值，当计数器计数到这个值时，电平发生跳变
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low ;  // 当定时器计数值小于CCR_Val时为高电平
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 // 使能通道2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);			 // 使能TIM2重载寄存器ARR

    TIM_Cmd(TIM3, DISABLE);                 // 失能定时器2

}

/**
  *@breif  步进电机初始化
	*@param  none
	*@retval none
	*/
void MotorInit(void)
{
    Motor_GPIO_Config();

    TIM3_Mode_Config();
}

/**
  *@breif  步进电机运行
	*@param  none
	*@retval none
	*/
void Motor1Start(uint8_t dir, uint16_t speed_us)  // 电机开启
{
    if(dir == 0)  // 正转
    {
        CW_M1();
    }
    else if(dir == 1) // 反转
    {
        CCW_M1();
    }

    TIM3->ARR = speed_us;

    TIM3->CCR2= speed_us/2;

    TIM_Cmd(TIM3, ENABLE);
}

/**
  *@breif  步进电机停止
	*@param  none
	*@retval none
	*/
void Motor1Stop(void)
{
    TIM_Cmd(TIM3, DISABLE); // TIMER2_CH3 开启

    TIM6Delay_f(1500);// 等待1S

}
