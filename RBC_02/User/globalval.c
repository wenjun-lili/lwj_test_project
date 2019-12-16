#include "globalval.h"

uint8_t BtnPressedFlag=0;//������־��0Ϊ��ʼ��־
uint8_t ADC_ConvtComplFlag = 0;  /* ADC Bit0��1�ǹ�ǿ�ź�ת����� Bit1λ��1���¶��ź�ת����� Bit2��ѹ���ź�ת����� */

uint16_t  CO_ZERO=2645;// ��������ֵΪ2645̨��
uint16_t  CO2_PB=0; // ������CO2��������С�ٷֱȣ�1%~5%��
uint16_t  R_XB=400;// ϵ��B (0.04*10000)
uint16_t  CO2_Taijie=150; // 1%CO2�ĵ�ƽ��
uint16_t  CO2_ZERO=500;// CO2���̨����
uint16_t  HB=0; //���˵�Ѫ�쵰����(�����ˣ�110g/L)
uint16_t  Ring_Num=0;//��Ȧʹ�ô���-20180228
uint16_t  Eeprom_Init=0;//eepromд���־
uint8_t   time=100;//��ɷ�������ʱ��

uint16_t  ATM_Coeff=10000;//��ѹ����ϵ�� 20180817

uint16_t QC_CONC=400;//�ʿ�����COŨ��4.99ppmX100
uint16_t QC_CO2_CONC=500;// �ʿ�����CO2Ũ��5.00ppmX100

uint16_t Curve_CONC=400; // 5ppm X100
uint16_t BGas_Num=1;//��������������
uint16_t ZGas_Num=1;//��������������

uint16_t Cal_CO_A=0;//A��׼��COŨ��
uint16_t Cal_CO_B=0;//B��׼��COŨ��

uint16_t CO2_B=0; // CO2�ı���Ũ��
uint16_t H0=65;//��׼����65m
uint16_t T0=28;//��׼�¶�28�棬301������
uint16_t H_Var=0;//�����������20180817
uint16_t T_Var=0;//�¶��������20180817
uint16_t Version=0;//�汾��
uint16_t Password=6666;//����ʦ��������
uint16_t Key_Input=0;//��������ֵ
uint16_t Patient_ID;//����ID
//// ���ڽ��ܷ�����

uint8_t RBC_pc_ComR[255]= {0};
uint8_t RBC_pc_ComT[6];
uint8_t Write_Receive[255];
uint8_t HB_H=0;//Ѫ�쵰��Ũ�ȸ�λ
uint8_t HB_L=0;//Ѫ�쵰��Ũ�ȵ�λ

uint8_t F_PC=0; //������־��0���ѻ���1������
uint8_t F_tiaoji=0; // ������־��0�� �û����� 1����������
uint8_t F_RUN2=0; // �û������� 0�� һ�����  1����ȷ����

uint8_t ReceiveFlag=0;// 1:��ʾ�������
uint8_t RevSDFlag=0; // ���ܵ���λ����SD����Ҫ�洢����Ϣ
uint8_t PC_ctr_Flag=0;//pc���Ʊ�־��1��PC���Ʋ��� 0����ֱ�Ӳ���20180228
uint8_t ReceiveFlag1=0;//1��ʾ��λ������������0��ʾ��λ��δ��������
uint8_t ReceiveFlag6=0;//���ֽ����ݽ��ձ�־ 1��־�ǽ��ճɹ���
/*
DWIN����ز��� lcy 20180317
*/
uint8_t  Com3TBuf[200];       // ����3���ݷ��ͻ�����
uint8_t  Com3RBuf[200];      //����3���ݽ��ջ�����
uint8_t  Com3Index;         //����3����������������
uint8_t  Index3End;         //����3������������
uint8_t  Rec3FinishF=0;       // ����3������ɱ�־

// ���λ��
uint8_t cursorx;
uint8_t cursory;

uint8_t Sex=0;
uint8_t F_CO2_Low=0;//CO2���ͱ�־λ
uint8_t F_Zero_Error;//zero������ʾ��־��1
uint8_t F_Zero=0;//�۲����ͨѶ��־��0Ϊ���۲죬1Ϊ�۲�
uint16_t USART1Timeout=5000;//����1ͨѶ��ʱ�ȴ�ʱ��1ms
uint8_t F_Timeout=0;//����ͨѶ��ʱ��־
uint8_t  F_DC=0;///��ƽ��ӡģʽ 1��������ӡģʽ
uint8_t  F_Sex=0;//�Ա�ֵ���������1Ϊ���յ�
uint8_t  F_CO2=0;//co2ֵ���������1Ϊ���յ���ֵ
uint8_t  Gender=0;///�Ա�0ΪŮ�ԣ�1Ϊ����

float    BME_Temperature;///����һ��������ȡ�¶�
float    BME_Humidity;///����һ��������ȡ��ʪ��
float    BME_Pressure;///����һ��������ȡ����ѹ
uint8_t  HumidityThreshold=40;///ʪ����ֵ
uint8_t  F_CO2_Num=0;//CO2����ֵС����ֵ�Ĵ���
uint8_t  F_Zero_Num=0;//CO��������쳣�Ĵ���
uint8_t RetFrame3Flag=1;// �������־,�Ƿ�Ҫ������ʾ����3��־λ���� 1:Ҫ������ʾ��3 */
