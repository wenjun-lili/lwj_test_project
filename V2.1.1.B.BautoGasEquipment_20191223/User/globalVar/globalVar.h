#ifndef _GLOBALVAR_H
#define _GLOBALVAR_H

#include "stm32f10x.h"
/*
������ز���
*/
extern uint8_t F_Key;//����״̬


/*
CO2��������ز���
*/
extern uint8_t  Com3TBuf[ ]; // ����3���ݷ��ͻ�����
extern uint8_t  Com3RBuf[ ]; //����3���ݽ��ջ�����
extern uint8_t  Buf_Zero[ ]; //����������
extern uint8_t  Com3Index;   //����3����������������
extern uint8_t  Index3End;   //����3������������
extern uint8_t  Rec3FinishF; // ����3������ɱ�־,1Ϊ������ɱ�־
extern uint16_t CO2Con;      //CO2Ũ��ֵ������100��
extern uint16_t CO2CONC[1200];
extern uint16_t CO2Threshold;
extern uint8_t  USART3_DMA;  //CO2��ȡ���ݣ��Ƿ�ʹ��DMA
extern uint16_t breathPeriod;
extern uint16_t breathPeriod;        //��������
extern uint16_t diffCO2;           //����������CO2Ũ��ƽ����ֵ
extern uint16_t minCO2;
extern uint16_t maxCO2;
/*
����һ��������ز���
*/
extern float  BME_Temperature;//����һ��������ȡ�¶�
extern float  BME_Humidity;//����һ��������ȡ��ʪ��
extern float  BME_Pressure;//����һ��������ȡ����ѹ

/*
������ز���
*/
extern uint8_t F_Debug;//���Ա�־��0Ϊ������ԣ�1Ϊ����״̬

/*
����������
*/
extern uint8_t CO2_Fault_Code;//CO2��ȡ�������

/*
�жϲ���
*/
extern uint16_t timeCnt;

/*
  ���ڷ�������
 */
extern uint16_t delayBegin;//ÿ������ǰ��ʱʱ�䣬��λ��ms
extern uint16_t delayAfter;//ÿ�ν�������ʱ�ر�ʱ�䣬��λ��ms
extern uint8_t PWMDuty;//��������ռ�ձȣ�Ĭ��50%

/*
����1��ز���
*/
extern uint8_t  Com1TBuf[200];       //����1���ݷ��ͻ�����
extern uint8_t  Com1RBuf[200];       //����1���ݽ��ջ�����
extern uint8_t  Com1Index;           //����1����������������
extern uint8_t  Index1End;         //����1������������
extern uint8_t  Rec1FinishF;       //����1������ɱ�־,1Ϊ������ɱ�־
extern uint32_t inputTemp;         //����1���������ݴ����
extern uint8_t  onLineFlag;        //������־ 0��δ����  1����

/*
���б�־
*/
extern uint8_t flagLungRun;        //�������������̱�־
extern uint8_t flagAgeMode;        //����ģʽ��ѡ��
extern uint8_t flagOKGas;
extern uint8_t flagOKSec;
extern uint16_t overTimeCnt;
extern uint8_t flagInit;

/*
�����־
*/
extern uint8_t flagError;//���ϱ�־�����ֲ�����ʱ��CO2ģ�����ݴ���ʱ����ȱ�־��1

extern float peakDeter;//�����ж�ϵ��
extern float peakInter;//�������ж�ϵ��
extern float vallcyDeter;//�����ж�ϵ��
extern float thresholdCoeff;//ʼ��CO2��ֵϵ��
extern float cutOffCoeff;//��ֹCO2ϵ��
extern uint16_t cutOffCO2;//��ֹCO2Ũ��
extern uint16_t CO2LowExp;//CO2�������ֵ80
extern uint16_t CO2HighExp;//CO2�������ֵ180
extern float getTimeCoeff;//����ʱ��ϵ��

#endif
