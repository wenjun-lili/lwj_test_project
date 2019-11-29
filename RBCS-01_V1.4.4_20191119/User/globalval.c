#include "globalval.h"

int8_t   BtnPressedFlag=-1;
uint8_t  ADC_ConvtComplFlag = 0;  /// ADC Bit0��1�ǹ�ǿ�ź�ת����� Bit1λ��1���¶��ź�ת����� Bit2��ѹ���ź�ת�����
uint16_t CO_ZERO=2645;/// ��������ֵΪ2645̨��
uint16_t CO2_PB=0; /// ������CO2��������С�ٷֱȣ�1%~5%��
uint16_t R_XB=400;/// ϵ��B (0.04*10000)
uint16_t CO2_Taijie=350; /// 1%CO2�ĵ�ƽ��
uint16_t CO2_Coeff=100;///CO2����ϵ����=����������Ũ��ֵ/��׼��Ũ��ֵ
uint16_t CO2_ZERO=500;/// CO2���̨����
uint16_t HB=150; ///���˵�Ѫ�쵰����(�����ˣ�110g/L)
uint16_t Ring_Num=0;///��Ȧʹ�ô��� 20180621
uint16_t QC_CONC=400;///�ʿ�����COŨ��4.99ppmX100
uint16_t QC_CO2_CONC=500;/// �ʿ�����CO2Ũ��5.00ppmX100
uint16_t Curve_CONC=400; /// 5ppm X100
uint16_t CO2_B=500; /// CO2�ı���Ũ��
uint8_t  RBC_pc_ComR[255]= {0};///���ڽ��ջ���
uint8_t  RBC_pc_ComT[6];///���ڷ��ͻ���
uint8_t  Write_Receive[255];
uint8_t  F_PC=0; ///������־��0���ѻ���1������
uint8_t  F_Version=0;///�汾�����־�� 0��δ���� 1������
uint8_t  F_Zero=0;///����ȶ��Բ��Ա�־��0��δ���Ա�־��1�����Ա�־
uint8_t  F_tiaoji=0; /// ������־��0�� �û����� 1����������
uint8_t  F_RUN2=0; /// �û������� 0�� һ�����  1����ȷ����
uint8_t  F_PC_Ctrl=0;///PC������λ����־  0��δ���ƣ�1������
uint8_t  ReceiveFlag=0;/// 1:��ʾ�������
uint8_t  ReceiveFlag1=0;///1:��ʾDD������ȷ
uint8_t  RevSDFlag=0; /// ���ܵ���λ����SD����Ҫ�洢����Ϣ
uint8_t  Com3TBuf[200];       /// ����3���ݷ��ͻ�����
uint8_t  Com3RBuf[200];       ///����3���ݽ��ջ�����
uint8_t  Buf_Zero[200]= {0};  ///����������
uint8_t  Com3Index;           ///����3����������������
uint8_t  Index3End=0;         ///����3������������
uint8_t  Rec3FinishF=0;       /// ����3������ɱ�־,1Ϊ������ɱ�־
uint8_t  cursorx;///���x����
uint8_t  cursory;///���y����
float    Version=3.0;///�汾��
uint8_t  CO2_Fault_Code=0;///CO2������������룬0������   1����ͨѶ  2��У��Ͳ�ͨ��
uint16_t USART1Timeout=40000;///����1ͨѶ��ʱ�ȴ�ʱ40s
uint8_t  F_Timeout=0;///����1ͨѶ��ʱ��־
uint16_t ATM_Coeff=10000;///��ѹ����ϵ�ATM_Coeff=(uint16_t)(10000*pow(1-H0/44330,5.255)/pow(1-H_Var/44330,5.255));��ת��Ϊ5λ��
uint16_t H0=65;///��׼����65m
float    T0=28.0;///��׼�¶�28�棬301������
uint16_t T0_Var=280;
uint16_t Calib=100;///�궨ϵ��*100
uint16_t CO2_Taijie_Calib=100;///CO2�궨ϵ��*100
uint16_t R_XB_Final=100;///CO�궨ϵ��*100
uint16_t QC_Calib=100;///�ʿغ�������ϵ��*100
//uint8_t F_HT=0;///�����غ��κ��¶��Ƿ������־λ��0Ϊδд�룬1Ϊд��
uint8_t  F_HT=0;///�Ƿ�����T0��H0
uint8_t  Gender=0;///�Ա�0ΪŮ�ԣ�1Ϊ����
float    BME_Temperature;///����һ��������ȡ�¶�
float    BME_Humidity;///����һ��������ȡ��ʪ��
float    BME_Pressure;///����һ��������ȡ����ѹ
uint8_t  F_DC=0;///��ƽ��ӡģʽ 1��������ӡģʽ
uint16_t HumidityThreshold;///ʪ����ֵ

uint8_t  F_CO2_Num=0;//CO2����ֵС����ֵ�Ĵ���
uint8_t  F_Zero_Num=0;//CO��������쳣�Ĵ���
uint8_t  ReceiveFlag6=0;//���ֽ����ݽ��ձ�־ 1��־�ǽ��ճɹ���
uint8_t  F_Sex=0;//�Ա�ֵ���������1Ϊ���յ�
uint8_t  F_CO2=0;//co2ֵ���������1Ϊ���յ���ֵ
uint8_t  gasTypeFactor=135;//�Զ�������������ϵ��������ֵ��ʵ��ֵ*100
uint8_t  F_GasType=0;
