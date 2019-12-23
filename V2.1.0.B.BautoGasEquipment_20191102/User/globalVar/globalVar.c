#include "globalVar.h"

/*
������ز���
*/
uint8_t F_Key=0;//������ 0Ϊ��ʼ��ֵ��1Ϊ�̰���λ����2Ϊ������λ����3Ϊ�������ɼ�����4Ϊ�������ɼ�����

/*
CO2������ͨѶ��ز���
*/
uint8_t  Com3TBuf[200];       //����3���ݷ��ͻ�����
uint8_t  Com3RBuf[50];       //����3���ݽ��ջ�����
uint8_t  Buf_Zero[200]= {0};  //����������
uint8_t  Com3Index;           //����3����������������
uint8_t  Index3End=0;         //����3������������
uint8_t  Rec3FinishF=0;       //����3������ɱ�־,1Ϊ������ɱ�־
uint16_t CO2Con=0;            //CO2Ũ��ֵ������100��
uint16_t CO2CONC[1200]={0};
uint16_t CO2Threshold;
uint8_t  USART3_DMA=1;        //CO2���ڶ�ȡ�����Ƿ�ʹ��DMA��Ϊ1ʱ����DMA��Ϊ0ʱ������DMA
uint16_t breathPeriod;        //��������
uint16_t diffCO2=0;           //����������CO2Ũ��ƽ����ֵ
uint16_t minCO2=0;            //�������ݵõ���������CO2ƽ�����Ũ��ֵ
uint16_t maxCO2=0;            //�������ݵõ���������CO2ƽ�����Ũ��ֵ

/*
����һ��������ز���
*/
float  BME_Temperature;//����һ��������ȡ�¶�
float  BME_Humidity;//����һ��������ȡ��ʪ��
float  BME_Pressure;//����һ��������ȡ����ѹ

/*
������ز���
*/
uint8_t F_Debug=1;//���Ա�־λ��0Ϊ�����ԣ�1Ϊ����

/*
����������
*/
uint8_t CO2_Fault_Code;//co2��ȡ����

/*
�жϲ���
*/
uint16_t timeCnt;//��ʱ������1s�ı���


/*
  ���ڷ������ݣ�Ĭ����������е��ǳ���ģʽ
 */
uint16_t delayBegin=500;//ÿ������ǰ��ʱʱ�䣬��λ��ms
uint16_t delayAfter=500;//ÿ�ν�������ʱ�ر�ʱ�䣬��λ��ms
uint8_t  PWMDuty=100;//��������ռ�ձȣ�Ĭ��0%

/*
����1��ز���
*/
uint8_t  Com1TBuf[200];       //����1���ݷ��ͻ�����
uint8_t  Com1RBuf[200];       //����1���ݽ��ջ�����
uint8_t  Com1Index;           //����1����������������
uint8_t  Index1End=0;         //����1������������
uint8_t  Rec1FinishF=0;       //����1������ɱ�־,1Ϊ������ɱ�־
uint32_t inputTemp=0;         //����1���������ݴ����
uint8_t  onLineFlag=0;        //����������־ 0����������  1������


/*
���б�־
*/
uint8_t flagLungRun=0;//�������������̱�־
uint8_t flagAgeMode=1;//��Ⱥģʽѡ��1Ĭ��Ϊ���ˣ�0Ĭ��Ϊ��ͯ��
uint8_t flagOKGas=0;//�ɼ����ϸ��������־��0���ڲɼ������л�δ�ɼ�����1���ڲɼ��������Ѿ��ɼ�����
uint8_t flagOKSec=0;//��������ı�־������5s�ı�������ֵΪ1
uint16_t overTimeCnt=0;//�����ϸ�ʱ�����ֵ
uint8_t flagInit=0;//��ʼ����ɱ�־��1Ϊ��ʼ�����

/*
�����־
*/
uint8_t flagError=0;//���ϱ�־�����ֲ�����ʱ��CO2ģ�����ݴ���ʱ����ȱ�־��1

float peakDeter=0.5;//�����ж�ϵ��
float peakInter=0.5;//�������ж�ϵ��
float vallcyDeter=0.25;//�����ж�ϵ��
float thresholdCoeff=0.50;//����ʼ������ֵϵ��
float cutOffCoeff=0.333333;//��ֹ������̼ϵ��
uint16_t cutOffCO2;//��ֹCO2Ũ��
uint16_t CO2LowExp=80;//CO2�������ֵ80
uint16_t CO2HighExp=180;//CO2�������ֵ180
float getTimeCoeff=0.5;//����ʱ��ϵ��


