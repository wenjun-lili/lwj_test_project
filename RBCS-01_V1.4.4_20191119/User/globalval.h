
#ifndef _GLOBALVAL_H_
#define _GLOBALVAL_H_

#include "stm32f10x.h"

extern int8_t BtnPressedFlag;
extern uint8_t ADC_ConvtComplFlag;

extern uint16_t  CO_ZERO; // CO�����
extern uint16_t   CO2_PB; //0~5%CO2 �ж�ֵ
extern uint16_t  R_XB; //   ����ϵ��
extern uint16_t  CO2_Taijie; //1%��̨����
extern uint16_t  CO2_Coeff;
extern uint16_t  CO2_ZERO;// CO2���̨����
extern uint16_t  Ring_Num;//20180621
extern uint16_t   HB;

extern uint16_t QC_CONC;//�ʿ�����COŨ�Ȳ�
extern uint16_t QC_CO2_CONC;// �ʿ�����CO2Ũ��5.00ppmX100

extern uint16_t Curve_CONC;// �����ߵ�CO Ũ�Ȳ�


extern uint16_t CO2_B; // CO2�ı���Ũ��

// ���ڽ�����
extern uint8_t RBC_pc_ComR[];
extern uint8_t RBC_pc_ComT[6];

extern  uint8_t Write_Receive[];



extern uint8_t F_PC;
extern uint8_t F_Version;//�汾�����־�� 0��δ���� 1������
extern uint8_t F_Zero;//����ȶ��Բ��Ա�־��0��δ���Ա�־��1�����Ա�־
extern uint8_t F_tiaoji;
extern uint8_t F_RUN2;
extern uint8_t F_PC_Ctrl;

extern uint8_t ReceiveFlag;
extern uint8_t ReceiveFlag1;
extern uint8_t RevSDFlag;

extern uint8_t  Com3TBuf[ ];
extern uint8_t  Com3RBuf[ ];
extern uint8_t  Buf_Zero[ ];
extern uint8_t  Com3Index;
extern uint8_t  Index3End;
extern uint8_t  Rec3FinishF;
// ���λ��
extern uint8_t cursorx;
extern uint8_t cursory;
extern float Version;
extern uint8_t CO2_Fault_Code;
extern uint16_t USART1Timeout;
extern uint8_t  F_Timeout;

//����У������
extern uint16_t H0;//�����ص��״α궨ʱ�ĺ���
extern float T0;//�����ص��״α궨ʱ���¶�
extern uint16_t T0_Var;
extern uint16_t CO2_Taijie_Calib;//CO2�궨ϵ��*100
extern uint16_t R_XB_Final;//CO�궨ϵ��*100
extern uint16_t QC_Calib;//�ʿغ�������ϵ��*100
extern uint8_t F_HT;//�������״α궨�Ƿ�����T��H��
extern uint16_t ATM_Coeff;//��ѹ��������
extern uint8_t Gender;//�Ա�

extern float BME_Temperature;//����һ��������ȡ�¶�
extern float BME_Humidity;//����һ��������ȡ��ʪ��
extern float BME_Pressure;//����һ��������ȡ����ѹ
extern uint8_t F_DC;
extern uint16_t HumidityThreshold;///ʪ����ֵ
extern uint8_t  F_CO2_Num;//CO2����ֵС����ֵ�Ĵ���
extern uint8_t  F_Zero_Num;//CO��������쳣�Ĵ���
extern uint8_t  ReceiveFlag6;
extern uint8_t  F_Sex;
extern uint8_t  F_CO2;
extern uint8_t  F_GasType;
extern uint8_t  gasTypeFactor;
#endif

