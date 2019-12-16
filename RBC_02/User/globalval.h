
#ifndef _GLOBALVAL_H_
#define _GLOBALVAL_H_

#include "stm32f10x.h"

extern uint8_t BtnPressedFlag;
extern uint8_t ADC_ConvtComplFlag;  /* ADC Bit1��1�ǹ�ǿ�ź�ת����� Bit2λ��1���¶��ź�ת����� Bit3��ѹ���ź�ת����� */
extern uint8_t time;
extern uint16_t ATM_Coeff;//��ѹ����ϵ��

extern uint16_t  CO_ZERO; // CO�����
extern uint16_t   CO2_PB; //0~5%CO2 �ж�ֵ
extern uint16_t  R_XB; //   ����ϵ��
extern uint16_t  CO2_Taijie; //1%��̨����
extern uint16_t  CO2_ZERO;// CO2���̨����
extern uint16_t   HB;
extern uint16_t   Ring_Num;//20180228
extern uint16_t  Eeprom_Init;//eeprom�Ƿ�����ֵ�ж�
extern uint16_t QC_CONC;//�ʿ�����COŨ�Ȳ�
extern uint16_t QC_CO2_CONC;// �ʿ�����CO2Ũ��5.00ppmX100

extern uint16_t Curve_CONC;// �����ߵ�CO Ũ�Ȳ�
extern uint16_t Cal_CO_A;
extern uint16_t Cal_CO_B;
extern uint16_t BGas_Num;
extern uint16_t ZGas_Num;

extern uint16_t CO2_B; // CO2�ı���Ũ��
extern uint16_t H0;
extern uint16_t T0;
extern uint16_t H_Var;//�����������20180817
extern uint16_t T_Var;//�¶��������20180817
extern uint16_t Version;//�汾��
extern uint16_t Password;//����
extern uint16_t Key_Input;//���������
extern uint16_t Patient_ID;//����ID
extern uint8_t HB_H;//Ѫ�쵰��Ũ�ȸ�λ
extern uint8_t HB_L;
// ���ڽ�����
extern uint8_t RBC_pc_ComR[];
extern uint8_t RBC_pc_ComT[6];

extern  uint8_t Write_Receive[];

#endif

extern  uint8_t F_PC;
extern  uint8_t F_tiaoji;
extern  uint8_t F_RUN2;

extern  uint8_t ReceiveFlag;
extern  uint8_t RevSDFlag;
extern  uint8_t PC_ctr_Flag;
extern  uint8_t ReceiveFlag1;
extern  uint8_t  ReceiveFlag6;

//DWIN����ز��� lcy 20180317
extern uint8_t  Com3TBuf[ ];      // ����3���ݷ��ͻ�����
extern uint8_t  Com3RBuf[ ];      //����3���ݽ��ջ�����
extern uint8_t  Com3Index;         //����3����������������
extern uint8_t  Index3End;         //����3������������
extern uint8_t  Rec3FinishF; // ����3������ɱ�־

extern uint8_t Sex;
extern uint8_t F_CO2_Low;
extern uint8_t F_Zero_Error;
extern uint8_t F_Zero;
extern uint16_t USART1Timeout;
extern uint8_t F_Timeout;
extern uint8_t F_DC;
extern float BME_Temperature;//����һ��������ȡ�¶�
extern float BME_Humidity;//����һ��������ȡ��ʪ��
extern float BME_Pressure;//����һ��������ȡ����ѹ
extern uint8_t  HumidityThreshold;///ʪ����ֵ
extern uint8_t  F_Sex;//�Ա�ֵ���������1Ϊ���յ�
extern uint8_t  F_CO2;//co2ֵ���������1Ϊ���յ���ֵ
extern uint8_t  Gender;///�Ա�0ΪŮ�ԣ�1Ϊ����
extern uint8_t  F_CO2_Num;//CO2����ֵС����ֵ�Ĵ���
extern uint8_t  F_Zero_Num;//CO��������쳣�Ĵ���
extern uint8_t RetFrame3Flag;

