#include "bsp_exti.h"

/**
 * @brief  NVIC_Configuration
* @datail 配置嵌套向量中断控制器NVIC
 * @param  无
 * @retval 无
 */
static void KEY_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 配置中断源*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    /* 配置抢占优先级 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    /* 配置子优先级 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* 使能中断通道 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  配置 IO为EXTI中断口，并设置中断优先级
 * @detail KEY1:复位按键  KEY2：环境气采气按键  KEY3：肺泡气采气按键
 * @param  无
 * @retval 无
 */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*开启按键GPIO口的时钟*/
    RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK, ENABLE);

    /* 配置 NVIC 中断*/
    KEY_NVIC_Configuration();

    /*--------------------------KEY1配置-----------------------------*/
    GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY2配置-----------------------------*/
    GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY3配置-----------------------------*/
    GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);

    /* 选择EXTI的信号源 */
    GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;

    /* EXTI为中断模式 */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* 下降沿中断 */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* 使能中断 */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/**
  *@breif  外部中断服务程序
  *@param  none
  *@retval none
  */
void EXTI15_10_IRQHandler(void)
{
    uint16_t i = 0;
    SysTick_Delay_Ms(10);//消抖
    if(KEY1 == 0) //再次确认是否按下
    {
        for(i = 0; i < 290; i++)
        {
            SysTick_Delay_Ms(10);
            if(KEY1 == 1)
            {
                __set_FAULTMASK(1); // 关闭所有中断
                NVIC_SystemReset(); // 复位
            }
        }
        if(KEY1 == 0) //长按进行模式的切换
        {
            F_Key = 1;
            flagAgeMode = !flagAgeMode; //切换成人/儿童模式

            /*设置不同模式的参数值*/
            if(flagAgeMode)
            {
                printf("进入成人模式，占空比0、每次采集延时0.3s、采集后延时0.6s\r\n");
                PWMDuty = 0; //成人全速工作
                delayBegin = 300; //开始后延迟0.3s启动气泵
                delayAfter = 600; //结束后延时0.6s关闭气泵

            }
            else
            {
                printf("进入婴儿模式，占空比50、每次采集延时0.1s、采集后延时0.1s\r\n");
                PWMDuty = 50; //婴儿半速工作
                delayBegin = 100; //开始后延迟0.1s启动气泵
                delayAfter = 100; //结束后立即关闭气泵

            }

            /*不同模式的状态灯显示*/
            for(i = 1; i <= 10; i++)
            {
                runLedDoing(i % 2); //状态灯亮起或者熄灭
                SysTick_Delay_Ms(200 + 800 * flagAgeMode); //闪烁时间,成人模式是1s闪烁一次，幼儿模式是闪烁0.5s
            }

        }

    }

    if((KEY2 == 0) && (flagError == 0)&& (flagInit==1)) //再次确认是否KEY2是否按下,前提条件未发生故障
    {
        F_Key = 2;
    }

    if((KEY3 == 0) && (flagError == 0)&& (flagInit==1)) //再次确认是否KEY2是否按下，前提条件未发生故障
    {
        F_Key = 3;
    }

    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  //清除LINE12上的中断标志位
    EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);  //清除LINE13上的中断标志位
    EXTI_ClearITPendingBit(KEY3_INT_EXTI_LINE);  //清除LINE14上的中断标志位
}





/*********************************************END OF FILE**********************/
