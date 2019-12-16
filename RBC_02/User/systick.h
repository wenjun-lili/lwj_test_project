#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "bsp.h"


/* Ŀǰ�ǿղ������û����Զ�����CPU����IDLE״̬�ĺ�����ι������ */
#define CPU_IDLE()

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
    volatile uint32_t count;	/* ������ */
    volatile uint8_t flag;		/* ��ʱ�����־  */
} SOFT_TMR;


/* ���ⲿ���õĺ������� */
void bsp_InitTimer(void);
void bsp_DelayMS(uint32_t n);
void bsp_StartTimer(uint8_t _id, uint32_t _period);
uint8_t bsp_CheckTimer(uint8_t _id);
int32_t bsp_GetRunTime(void);




#endif /* __SYSTICK_H */
