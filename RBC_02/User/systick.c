
#include "systick.h"
#include "bsp.h"
#include "bsp_button.h"

/* 这2个全局变量适用于bsp_DelayMS()   */
static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t  s_ucTimeOutFlag = 0;

/*
在此定义若干个软件定时器全局变量
注意，必须增加__IO 即 volatile，因为这个变量在中断和主程序中同时被访问，有可能造成编译器错误优化。
*/
#define TMR_COUNT	4		/* 软件定时器的个数，第1个保留给bsp_DelayMS()使用 */

SOFT_TMR g_Tmr[TMR_COUNT];

/* 全局运行时间，单位10ms，用于uIP */
__IO int32_t g_iRunTime = 0;

static void bsp_SoftTimerDec(SOFT_TMR *_tmr);

void bsp_InitTimer(void)
{
    uint8_t i;

    /* 清零所有的软件定时器 */
    for (i = 0; i < TMR_COUNT; i++)
    {
        g_Tmr[i].count = 0;
        g_Tmr[i].flag = 0;
    }

    /*
    	配置systic中断周期为1ms，并启动systick中断。
    	这个函数在 \Libraries\CMSIS\CM3\CoreSupport\core_cm3.h

    	Systick中断向量在(\Libraries\CMSIS\CM3\DeviceSupport\ST\STM32F10x\startup\arm\
    		startup_stm32f10x_hd.s 文件中定义为 SysTick_Handler。
    	SysTick_Handler函数的实现在stm32f10x_it.c 文件。
    	SysTick_Handler函数调用了SysTick_ISR()函数，在本文件末尾。
    */
    SysTick_Config(SystemCoreClock / 1000); // 计数初值为72000
    // Systick计数频率为每秒72000000，所以72000次就是1/1000秒，也就是1ms
    // SysTick是1个24bit递减计数器，通过对SYSTick控制与状态寄存器的设置，可选择HCLK时钟
    // (72M)或者HCLK的8分频作为SYSTICK的时钟源// V3.5默认为HCLK时钟
}

/*
*********************************************************************************************************
*	函 数 名: SysTick_ISR
*	功能说明: SysTick中断服务程序，每隔1ms进入1次
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
extern void bsp_RunPer10ms(void);

void SysTick_ISR(void)
{
    static uint8_t s_count = 0;
    uint8_t i;

    /* 每隔1ms进来一次  ( 仅用于 bsp_DelayMS) */
    if (s_uiDelayCount > 0)
    {
        if (--s_uiDelayCount == 0)
        {
            s_ucTimeOutFlag = 1;
        }
    }
    /*每隔1ms,对软件定时器的计数器进行减一操作*/
    for (i = 0; i < TMR_COUNT; i++)
    {
        bsp_SoftTimerDec(&g_Tmr[i]);
    }
    g_iRunTime++;	/* 全局运行时间每1ms增1 */
    if (g_iRunTime == 0x7FFFFFFF)
    {
        g_iRunTime = 0;
    }
    if (++s_count >= 10)
    {
        s_count = 0;
        bsp_RunPer10ms();/*每隔10ms调用一次此函数，此函数在bsp.c 20180320*/

    }

}

/*
*********************************************************************************************************
*	函 数 名: bsp_SoftTimerDec
*	功能说明: 每隔1ms对所有定时器变量减1。必须被SysTick_ISR周期性调用。
*	形    参：_tmr : 定时器变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_SoftTimerDec(SOFT_TMR *_tmr)
{
    if (_tmr->flag == 0)
    {
        if (_tmr->count > 0)
        {
            /* 如果定时器变量减到1则设置定时器到达标志 */
            if (--_tmr->count == 0)
            {
                _tmr->flag = 1;
            }
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DelayMS
*	功能说明: ms级延迟，延迟精度为正负1ms
*	形    参：n : 延迟长度，单位1 ms。 n 应大于2
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_DelayMS(uint32_t n)
{
    /* 避免 n = 1 出现主程序死锁 */
    if (n <= 1)
    {
        n = 2;
    }

    __set_PRIMASK(1);  		/* 关中断 */
    g_Tmr[0].count = n;
    g_Tmr[0].flag = 0;
    __set_PRIMASK(0);  		/* 开中断 */

    while (1)
    {
        CPU_IDLE();	/* 此处是空操作。用户可以定义，让CPU进入IDLE状态，以降低功耗；或实现喂狗 */

        /* 等待延迟时间到 */
        if (g_Tmr[0].flag == 1)
        {
            break;
        }
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_StartTimer
*	功能说明: 启动一个定时器，并设置定时周期。
*	形    参：	_id     : 定时器ID，值域【1,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*						  定时器ID = 0 已用于bsp_DelayMS()函数
*				_period : 定时周期，单位1ms
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_StartTimer(uint8_t _id, uint32_t _period)
{
    if (_id >= TMR_COUNT)
    {
        /* while(1); 死机 */
        return;
    }

    __set_PRIMASK(1);  		/* 关中断 */
    g_Tmr[_id].count = _period;
    g_Tmr[_id].flag = 0;
    __set_PRIMASK(0);  		/* 开中断 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_CheckTimer
*	功能说明: 检测定时器是否超时
*	形    参：	_id     : 定时器ID，值域【1,TMR_COUNT-1】。用户必须自行维护定时器ID，以避免定时器ID冲突。
*						  0 用于
*				_period : 定时周期，单位1ms
*	返 回 值: 返回 0 表示定时未到， 1表示定时到
*********************************************************************************************************
*/
uint8_t bsp_CheckTimer(uint8_t _id)
{
    if (_id >= TMR_COUNT)
    {
        return 0;
    }

    if (g_Tmr[_id].flag == 1)
    {
        g_Tmr[_id].flag = 0;
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
*********************************************************************************************************
*	函 数 名: bsp_GetRunTime
*	功能说明: 获取CPU运行时间，单位1ms
*	形    参：无
*	返 回 值: CPU运行时间，单位1ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime(void)
{
    int runtime;

    __set_PRIMASK(1);  		/* 关中断 */

    runtime = g_iRunTime;	/* 由于在Systick中断被改写，因此关中断进行保护 */

    __set_PRIMASK(0);  		/* 开中断 */

    return runtime;
}


/*
*********************************************************************************************************
*	函 数 名: SysTick_Handler
*	功能说明: 系统嘀嗒定时器中断服务程序。启动文件中引用了该函数。
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void SysTick_Handler(void)
{
    SysTick_ISR();
}





/*********************************************END OF FILE**********************/
