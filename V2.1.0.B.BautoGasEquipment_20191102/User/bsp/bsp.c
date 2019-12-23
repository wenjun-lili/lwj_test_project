#include "bsp.h"

#define runLED   PDout(9)// PD9
#define errorLED PDout(10)// PD10
#define co2LED   PDout(8) //PD8

static void TIM7_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  bsp_init
  * @datail 底层初始化
  * @param  none
  * @return none
*/
void bspInit(void)
{
    NVIC_Config();//NVIC配置

    delay_init();//延时函数配置初始化

    USART_Config();//初始化USART

    I2C_BME280_Init();//三合一传感器初始化

    beepInit();//蜂鸣器初始化

    ledInit();//led灯初始化

    evalveInit();//电磁阀初始化

    EXTI_Key_Config();//按键初始化

    USART3_Config();//CO2串口配置  CO2传感器预热周期大概为50s，实测

    ADVANCE_TIM_Init();//tim8初始化，即气泵初始化

    TIM7_Config();//基本定时器初始化
     
		TIM6_Config();//基本定时器初始化

    breathSensorInit();//呼吸测量传感器初始化

}
/**
  *@breif  TIM7定时器配置
	*@detail 定时器设置中断周期为200ms
	*@param  none
	*@retval none
	*/
void TIM7_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 设置TIM6CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = 1999;  //200ms

    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM7, DISABLE);

    TIM7_NVIC_Config();
}

/**
  *@breif 自检流程
  *@detail V01和V02打开，全速启动气泵，清洗10s，清洗后读取气压值，
           如不在[500,1200]hPa范围内，则点亮故障灯
  *@param  none
  *@retval none
  */
void selfInspection(void)
{
    evalveOpen(1);//打开电磁阀1，即为2和1通

    evalveClose(2);//关闭电磁阀2

    timeCnt=10;//清洗时间10s

    TIM_Cmd(TIM7,ENABLE);//打开定时器

    TIM_SetCompare3(TIM8,0);//设置占空比0，则气泵1低电平全速

    while(timeCnt!=0);//查看10s是否完成

    TIM_Cmd(TIM7,DISABLE);//关闭定时器

    TIM_SetCompare3(TIM8,100);//设置占空比100%，气泵1停止工作

    BME280_ReadPressure(&BME_Pressure);//读取三合一传感器中气压值
    printf("自检气压值为：%f\r\n",BME_Pressure);

    if((BME_Pressure>1200)||(BME_Pressure<500))///气压故障判断
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPressure(&BME_Pressure);
        if((BME_Pressure>1200)||(BME_Pressure<500))
        {
            errorLedDoing(1);//故障灯亮起来
            while(1);//待机
        }
    }
    evalveAllClose();//关闭所有电磁阀
		
		beepMode(1);
    

    
}
/**
  *@breif  参数设置
  *@detail 设置每次采气前延时和气泵运行占空比
  *@param  none
  *@retval none
  */
void parametersSet(void)
{
    uint16_t num=0;

    printf("是否联机设置参数？请回复(Y/N)-请输入大写\r\n");
    while(Rec1FinishF!=1)
    {
        Delay10ms(1);
        num++;
        if(num>400)//如果未答复，则直接跳过
        {
            printf("未设置参数\r\n");
            break;
        }
    }

    if(Com1RBuf[0]==0x59)
    {
        Rec1FinishF=0;//接收标志归零
        onLineFlag=1;//联机标志位置1

        printf("请输入采气前延时时间，单位：ms\r\n");
        while(Rec1FinishF!=1);//等待接收完毕
        Rec1FinishF=0;
        delayBegin=inputTemp;

        printf("请输入采气后延时时间，单位：ms\r\n");
        while(Rec1FinishF!=1);//等待接收完毕
        Rec1FinishF=0;
        delayAfter=inputTemp;

        printf("请输入气泵运行占空比(高电平比例)，单位：百分比\r\n");
        while(Rec1FinishF!=1);//等待接收完毕
        Rec1FinishF=0;
        PWMDuty=inputTemp;
    }
    printf("采气前延时=%d\r\n",delayBegin);
    printf("采气后延时=%d\r\n",delayAfter);
    printf("占空比（高电平比例）=%d\r\n",PWMDuty);
    Rec1FinishF=0;//接收标志归零

}

/**
  *@breif 定时器3初始化
  *@param
         @arr 自动重装值
         @psc 时钟预分频值
  *@retval none

  */
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能

    //定时器TIM3初始化
    TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据指定的参数初始化TIMx的时间基数单位

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //使能指定的TIM3中断,允许更新中断

    //中断优先级NVIC设置
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //初始化NVIC寄存器


    TIM_Cmd(TIM3, ENABLE);  //使能TIMx
}
/**
  *@breif 定时器3中断服务程序
  *@param none
  *@retval none
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
        co2LED=!co2LED;
    }
}

/**
  *@breif  TIM6定时器中断优先级配置
	*@param  none
	*@retval none
	*/
static void TIM6_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  *@breif  TIM6定时器配置
	*@detail 定时器设置中断周期为50ms
	*@param  none
	*@retval none
	*/
void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* 设置TIM6CLK 为 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* 自动重装载寄存器周期的值(计数值) */
    TIM_TimeBaseStructure.TIM_Period = 4999;  //50ms

    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    /* 时钟预分频数为72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    /* 对外部时钟进行采样的时钟分频,这里没有用到 */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM6, DISABLE);

    TIM6_NVIC_Config();
}
