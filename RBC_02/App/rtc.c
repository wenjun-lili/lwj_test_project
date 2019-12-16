
#include "rtc.h"


/* ������ʱ�����Ƿ���Ҫ��������  0����Ҫ   1������Ҫ */
uint8_t RTCNeedReConfig = 0;


/* ���жϱ�־���������ж�ʱ��1����ʱ�䱻ˢ��֮����0 */
__IO uint32_t TimeDisplay = 0;


/*
 * ��������NVIC_Configuration
 * ����  ������RTC���жϵ����ж����ȼ�Ϊ1�������ȼ�Ϊ0
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
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
 * ��������RTC_CheckAndConfig
 * ����  ����鲢����RTC
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ���ã��ⲿ����
 */
void RTC_CheckAndConfig(struct rtc_time *tm)
{
    /*������ʱ��鱸�ݼĴ���BKP_DR1��������ݲ���0xA5A5,
    ������������ʱ�䲢ѯ���û�����ʱ��*/
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
    else  // �ϵ縴λ��������λ
    {
        /*��������������ʱ��*/
        /*����Ƿ��������*/
        if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
        {
//		    printf("\r\n\r\n Power On Reset occurred....\r\n\r\n");
        }
        /*����Ƿ�Reset��λ*/
        else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
        {
//			printf("\r\n\r\n External Reset occurred..\r\n");
        }
//		  printf("\r\n No need to configure RTC..\r\n");

        RTCNeedReConfig = 1;

        /*�ȴ��Ĵ���ͬ��*/
        RTC_WaitForSynchro();

        /*����RTC���ж�*/
        RTC_ITConfig(RTC_IT_SEC, ENABLE);

        /*�ȴ��ϴ�RTC�Ĵ���д�������*/
        RTC_WaitForLastTask();

    }
    /*������ʱ������꣬������У��ʱ�������PC13*/

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
 * ��������RTC_Configuration
 * ����  ������RTC
 * ����  ����
 * ���  ����
 * ����  ���ڲ�����
 */
void RTC_Configuration(void)
{
    /******************************************************************
    ϵͳ��λ�󣬶Ժ󱸼Ĵ�����RTC�ķ��ʱ���ֹ������Ϊ�˷�ֹ�Ժ�����(BKP)������д������
    ִ�����²�����ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʣ�
    �� ���üĴ���RCC_APB1ENR��PWREN��BKPENλ��ʹ�ܵ�Դ�ͺ󱸽ӿ�ʱ��
    �� ���üĴ���PWR_CR��DBPλ��ʹ�ܶԺ󱸼Ĵ�����RTC�ķ��ʡ�
    ******************************************************************/
    /* Enable PWR and BKP clocks */   //�󱸵�Դ  �󱸵�ش�
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);//  ʹ���������   PWR_CR��DBP=1��������򿪣�

    /* Reset Backup Domain */
//	BKP_DeInit();// Ҫ��Ҫ������  ȥ�������޸ı���ʱ��

    /*ȷ��ʱ��*/
    /* Enable LSE */
    RCC_LSEConfig(RCC_LSE_ON);// ���ⲿ���پ�������
//	RCC_LSECmd(ENABLE);
    /* Wait till LSE is ready */  //�ⲿ����
    while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {}

    /* Select LSE as RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Enable RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC registers synchronization
     * ��ΪRTCʱ���ǵ��ٵģ��ڻ�ʱ���Ǹ��ٵģ�����Ҫͬ��
     */
    RTC_WaitForSynchro();  //RTC �Ĵ�����APBͬ��

    /* Wait until last write operation on RTC registers has finished */
    /* ��RTC�κμĴ�����д��������������ǰһ��д������������С� */
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
 * ��������Time_Adjust
 * ����  ��ʱ�����
 * ����  �����ڶ�ȡRTCʱ��Ľṹ��ָ��
 * ���  ����
 * ����  ���ڲ�����
 */
void Time_Adjust(struct rtc_time *tm)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Get tm->wday */
    GregorianDay(tm);

    /* �޸ĵ�ǰRTC�����Ĵ������� */
    /* �������ڹ����е��������Ϊ����ʱ��, ������ת����ʱ���ʱ���Ҫ�ӱ���ʱ��ת��Ϊ��׼ʱ���ʱ��� */
    RTC_SetCounter(mktimev(tm)); //����RTC��������ֵ

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}
//

/*
	��RTC counter�Ĵ���ֵ��ת��д��ʱ��ṹ����
*/
void RTCTimeTransfer(struct rtc_time* tm)
{
    uint32_t TimeVar;
    uint32_t BJ_TimeVar;

    TimeVar = RTC_GetCounter();
    BJ_TimeVar = TimeVar + 8*60*60;  /* �ѱ�׼ʱ��ת��Ϊ����ʱ�� */

    to_tm(BJ_TimeVar, tm);	/* �ѱ���ʱ��д��ʱ��ṹ���� */
}

///*
//	������������RTCʱ����LCD��ʾ����ǰʱ��
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
