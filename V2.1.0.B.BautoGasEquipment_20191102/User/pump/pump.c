#include "pump.h"

static void ADVANCE_TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(ADVANCE_TIM_CH3_GPIO_CLK, ENABLE);
    RCC_APB2PeriphClockCmd(ADVANCE_TIM_CLK,ENABLE);

    /*肺泡气采集泵引脚初始化,该泵采用PWM形式控制*/
    GPIO_InitStructure.GPIO_Pin =  ADVANCE_TIM_CH3_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(ADVANCE_TIM_CH3_PORT, &GPIO_InitStructure);
    
	/*环境本底气采集泵引脚初始化，该泵直接采用高低电平控制*/
    GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC,&GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_9);//关闭气泵
}

//

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *	TIM_CounterMode			     TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */

/* ----------------   PWM信号 周期和占空比的计算--------------- */
// ARR ：自动重装载寄存器的值
// CLK_cnt：计数器的时钟，等于 Fck_int / (psc+1) = 72M/(psc+1)
// PWM 信号的周期 T = (ARR+1) * (1/CLK_cnt) = (ARR+1)*(PSC+1) / 72M
// 占空比P=CCR/(ARR+1)

static void ADVANCE_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    /*--------------------时基结构体初始化-------------------------*/

    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=ADVANCE_TIM_PERIOD;
    // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler= ADVANCE_TIM_PSC;
    // 时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // 初始化定时器
    TIM_TimeBaseInit(ADVANCE_TIM, &TIM_TimeBaseStructure);

    /*--------------------输出比较结构体初始化-------------------*/

    // 配置为PWM模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // 输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	// 互补输出使能
//	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
    // 设置占空比大小
    TIM_OCInitStructure.TIM_Pulse = ADVANCE_TIM_PULSE;
    // 输出通道电平极性配置，当计数值小于CCR时，为高电平
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
		
		TIM_OCInitStructure.TIM_OCNPolarity=TIM_OCNPolarity_Low;//设置互补输出极性

    TIM_OCInitStructure.TIM_OCIdleState=TIM_OCIdleState_Set;//选择空闲状态下得非工作状态

    TIM_OCInitStructure.TIM_OCNIdleState=TIM_OCNIdleState_Reset;//选择互补空闲状态下得非工作状态

    TIM_OC3Init(ADVANCE_TIM, &TIM_OCInitStructure);
    TIM_OC3PreloadConfig(ADVANCE_TIM, TIM_OCPreload_Enable);

    //使能TIM8重载寄存器ARR
    TIM_ARRPreloadConfig(ADVANCE_TIM, ENABLE);
    // 主输出使能，当使用的是通用定时器时，这句不需要
    TIM_CtrlPWMOutputs(ADVANCE_TIM, ENABLE);
    // 使能计数器
    TIM_Cmd(ADVANCE_TIM, ENABLE);
}

void ADVANCE_TIM_Init(void)
{
    ADVANCE_TIM_GPIO_Config();
    ADVANCE_TIM_Mode_Config();
}

/*********************************************END OF FILE**********************/
