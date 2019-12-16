
#include "rtc.h"


/* 年月日时分秒是否需要重新配置  0：需要   1：不需要 */
uint8_t RTCNeedReConfig = 0;


/* 秒中断标志，进入秒中断时置1，当时间被刷新之后清0 */
__IO uint32_t TimeDisplay = 0;


/*
 * 函数名：NVIC_Configuration
 * 描述  ：配置RTC秒中断的主中断优先级为1，次优先级为0
 * 输入  ：无
 * 输出  ：无
 * 调用  ：外部调用
 */
static void RTC_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/*
 * 函数名：RTC_CheckAndConfig
 * 描述  ：检查并配置RTC
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用：外部调用
 */
void RTC_CheckAndConfig(struct rtc_time *tm)
{
    /*在启动时检查备份寄存器BKP_DR1，如果内容不是0xA5A5,
    则需重新配置时间并询问用户调整时间*/
    if (BKP_ReadBackupRegister(BKP_DR1) != 0xA5A5)
    {
        //printf("\r\n\r\n RTC not yet configured....");

        /* RTC Configuration */
        RTC_Configuration();

        //printf("\r\n\r\n RTC configured....");

        /* Get time entred by the user */
        //Time_Regulate(tm);
        tm->tm_year = 2017;
        tm->tm_mon = 6;
        tm->tm_mday = 28;
        tm->tm_hour = 14;
        tm->tm_min = 05;
        tm->tm_sec = 0;

        /* Adjust time by users typed on the hyperterminal */
        Time_Adjust(tm);

        BKP_WriteBackupRegister(BKP_DR1, 0xA5A5);
    }
    else  // 上电复位，按键复位
    {
        /*启动无需设置新时钟*/
        /*检查是否掉电重启*/
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
//		    printf("\r\n\r\n Power On Reset occurred....\r\n\r\n");
        }
        /*检查是否Reset复位*/
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
//			printf("\r\n\r\n External Reset occurred..\r\n");
        }
//		  printf("\r\n No need to configure RTC..\r\n");

        RTCNeedReConfig = 1;

        /*等待寄存器同步*/
        RTC_WaitForSynchro();

        /*允许RTC秒中断*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /*等待上次RTC寄存器写操作完成*/
        RTC_WaitForLastTask();

    }
    /*定义了时钟输出宏，则配置校正时钟输出到PC13*/

#ifdef RTCClockOutput_Enable
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Disable the Tamper Pin */
    BKP_TamperPinCmd(DISABLE); /* To output RTCCLK/64 on Tamper pin, the tamper
	                                 functionality must be disabled */

    /* Enable RTC Clock Output on Tamper Pin */
    BKP_RTCOutputConfig(BKP_RTCOutputSource_CalibClock);
#endif


    /* Clear reset flags */
    RCC_ClearFlag();

}

/*
 * 函数名：RTC_Configuration
 * 描述  ：配置RTC
 * 输入  ：无
 * 输出  ：无
 * 调用  ：内部调用
 */
void RTC_Configuration(void)
{
    /******************************************************************
    系统复位后，对后备寄存器和RTC的访问被禁止，这是为了防止对后备区域(BKP)的意外写操作。
    执行以下操作将使能对后备寄存器和RTC的访问：
    ● 设置寄存器RCC_APB1ENR的PWREN和BKPEN位，使能电源和后备接口时钟
    ● 设置寄存器PWR_CR的DBP位，使能对后备寄存器和RTC的访问。
    ******************************************************************/
    /* Enable PWR and BKP clocks */   //后备电源  后备电池打开
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);//  使能允许访问   PWR_CR的DBP=1（后备区域打开）

    /* Reset Backup Domain */
//	BKP_DeInit();// 要不要都可以  去掉可以修改保存时间

    /*确定时钟*/
    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);// 打开外部低速晶体振荡器
//	RCC_LSECmd(ENABLE);
    /* Wait till LSE is ready */  //外部起振
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization
     * 因为RTC时钟是低速的，内环时钟是高速的，所以要同步
     */
    RTC_WaitForSynchro();  //RTC 寄存器与APB同步

    /* Wait until last write operation on RTC registers has finished */
    /* 对RTC任何寄存器的写操作，都必须在前一次写操作结束后进行。 */
    RTC_WaitForLastTask();
    /* Enable the RTC Second Interrupt */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Set RTC prescaler: set RTC period to 1sec */
    RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) = 1HZ */
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


/*
 * 函数名：Time_Adjust
 * 描述  ：时间调节
 * 输入  ：用于读取RTC时间的结构体指针
 * 输出  ：无
 * 调用  ：内部调用
 */
void Time_Adjust(struct rtc_time *tm)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Get tm->wday */
    GregorianDay(tm);

    /* 修改当前RTC计数寄存器内容 */
    /* 本函数在工程中的输入参数为北京时间, 所以在转换成时间戳时最后要从北京时间转换为标准时间的时间戳 */
    RTC_SetCounter(mktimev(tm)); //更新RTC计数器的值

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}
//

/*
	将RTC counter寄存器值经转换写到时间结构体中
*/
void RTCTimeTransfer(struct rtc_time* tm)
{
    uint32_t TimeVar;
    uint32_t BJ_TimeVar;

    TimeVar = RTC_GetCounter();
    BJ_TimeVar = TimeVar + 8*60*60;  /* 把标准时间转换为北京时间 */

    to_tm(BJ_TimeVar, tm);	/* 把北京时间写入时间结构体中 */
}

///*
//	不用重新配置RTC时，在LCD显示出当前时间
//*/
//void RTCTimeShow(struct rtc_time* tm )
//{
//	Print_Value_I( 88, 32, tm->tm_year, BLACK, WHITE, 4);
//	Print_Value_I( 128, 32, tm->tm_mon, BLACK, WHITE, 2);
//	Print_Value_I( 152, 32, tm->tm_mday, BLACK, WHITE, 2);
//	Print_Value_I( 88, 48,  tm->tm_hour, BLACK, WHITE, 2);
//	Print_Value_I( 112, 48, tm->tm_min, BLACK, WHITE, 2);
//	Print_Value_I( 136, 48, tm->tm_sec, BLACK, WHITE, 2);
//}


void SysTimeBakeup(struct rtc_time* tm, struct rtc_time* tmbakeup)
{
    RTCTimeTransfer(tm);
    tmbakeup->tm_sec = tm->tm_sec;
    tmbakeup->tm_min = tm->tm_min;
    tmbakeup->tm_hour = tm->tm_hour;
    tmbakeup->tm_mday = tm->tm_mday;
    tmbakeup->tm_mon = tm->tm_mon;
    tmbakeup->tm_year = tm->tm_year;
    tmbakeup->tm_wday = tm->tm_wday;
}

void RTC_Init(struct rtc_time *tm)
{
    RTC_CheckAndConfig(tm);
    RTC_NVIC_Config();
}



/**
  * @brief  This function handles RTC interrupt request.
  * @param  None
  * @retval None
  */
void RTC_IRQHandler(void)
{
    if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
    {
        /* Clear the RTC Second interrupt */
        RTC_ClearITPendingBit(RTC_IT_SEC);

        /* Enable time update */
        TimeDisplay = 1;

        /* Wait until last write operation on RTC registers has finished */
        RTC_WaitForLastTask();
    }
}



/************************END OF FILE***************************************/
