

#ifndef __RTC_H
#define	__RTC_H


#include "stm32f10x.h"
#include "date.h"
#include "./usart/bsp_usart.h"

/* 如果定义了下面这个宏的话,PC13就会输出频率为RTC Clock/64的时钟 */
//#define RTCClockOutput_Enable  /* RTC Clock/64 is output on tamper pin(PC.13) */

void RTC_CheckAndConfig(struct rtc_time *tm);
void RTC_Configuration(void);
void Time_Adjust(struct rtc_time *tm);
void RTCTimeTransfer(struct rtc_time *tm);
void RTCTimeShow(struct rtc_time* tm );

void SysTimeBakeup(struct rtc_time* tm, struct rtc_time* tmbakeup);

void RTC_Init(struct rtc_time *tm);

#endif /* __RTC_H */
