#include "process.h"
#include "rtc.h"
#include "globalval.h"
#include "DWIN.h"
#include "bsp.h"

extern uint8_t BtnPressedFlag;
extern uint16_t DGUS_Layout_Flag;
extern uint16_t Warming_up_Cnt;

/**
  *@breif �¼�����
	*@param none
	*@retval none
	*/
void Event_Process(void)
{
    //Ԥ�Ƚ����ж�ˢ��
    if( (DGUS_Layout_Flag == 2) && ((BtnPressedFlag == 0xAA) || (Warming_up_Cnt == 0)) )
    {
        DGUS_Layout_Flag = 1000;//1000����ҳ��ˢ��
        BtnPressedFlag = 0;
    }
    //���������ж�ˢ��
    if( (DGUS_Layout_Flag == 1) && (BtnPressedFlag == 0xAA))
    {
        DGUS_Layout_Flag = 1000;
        BtnPressedFlag = 0;
    }

}

/**
  *@breif ��������
	*@param none
	*@retval none
	*/
void Touch_Process(void)
{
    /*��ֵ���� 20180320
    ��ֵ      ����
    0x0011    CO2�궨��
    0x0022    �ʿؼ�
    0x0033    ������
    0x0044    �ܷ�Ȧ���������
    0x0055    �����Լ�
    0x0066    ����/ʱ�����ü����� ����/ʱ��/�ܷ�Ȧ���ü�
    0x0077    �رյ�ŷ�
    0x0088    �汾�ż�
    0x0099    �������ü�
    0x00AA    ����Ԥ�Ƚ���/���ش�������/ȡ����
    0x00BB    ��������ʦ�����������/������/�궨��/������ŷ���/ȷ�ϼ�/��ʼ��
    0x00CC    ��׼������
    0x00DD	  ��ȷ������
    0x00EE    �������ü�
    0x00FF    CO�궨��
    */
    if((Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x20) && (Com3RBuf[5] == 0x00))
    {
        while(!Rec3FinishF);
        Rec3FinishF = 0;
        BtnPressedFlag = Com3RBuf[8];
        Com3RBuf[4] = 0;
        Com3RBuf[5] = 0;
        Com3RBuf[8] = 0;
//        printf("��ֵ��%02x\r\n", BtnPressedFlag);
    }
    /*�������봥������ 20180320
    ��Ҫ���������롢COϵ����CO2ϵ����CO2��㡢CO�����ֵ��CO2Ũ����ֵ������ID��HB��A������COŨ�ȡ�B������CO Ũ��
    				�CCO2��׼��Ũ��
              ������������������������������
    */
    /*������������ 20180320
    ָ��:5A A5 06 83 00 21 01 XX XX
    */
#if 0
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x21) && (Com3RBuf[6] == 0x01))
    {
        Key_Input = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("key is %x\r\n", Key_Input);
    }
    /*R_XB�������� 20180320
    ָ��:5A A5 06 83 00 05 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x05) && (Com3RBuf[6] == 0x01))
    {
        R_XB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("R_XB is	%x\r\n", R_XB);
    }
    /*CO2_Taijie�������� 20180320
    ָ��:5A A5 06 83 00 06 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x06) && (Com3RBuf[6] == 0x01))
    {
        CO2_Taijie = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_Taijie is	%d\r\n", CO2_Taijie);
    }
    /*CO2_ZERO�������� 20180320
    ָ��:5A A5 06 83 00 07 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x07) && (Com3RBuf[6] == 0x01))
    {
        CO2_ZERO = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_ZERO is	%d\r\n", CO2_ZERO);
    }
    /*CO2_PB�������� 20180320
    ָ��:5A A5 06 83 00 07 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x03) && (Com3RBuf[6] == 0x01))
    {
        CO2_PB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_PB is	%d\r\n", CO2_PB);
    }
    /*CO2_PB�������� 20180320
    ָ��:5A A5 12 83 00 0A 04 XX XX XX XX XX XX XX XX
    */
    if((Com3RBuf[2] == 0x12) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0A) && (Com3RBuf[6] == 0x01))
    {
        Patient_ID = (uint16_t)Com3RBuf[7] << 8 | (uint16_t)Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("Patient_ID is	%lld\r\n", Patient_ID);
    }
    /*HB�������� 20180320
    ָ��:5A A5 06 83 00 0E 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0E) && (Com3RBuf[6] == 0x01))
    {
        HB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("HB is	%d\r\n", HB);
    }
    /*CO2_B(co2��׼��Ũ��)�������� 20180320
    ָ��:5A A5 06 83 00 12 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
    {
        CO2_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_B is	%d\r\n", CO2_B);
    }
    /*Cal_CO_A(A��׼��COŨ��)�������� 20180320
    ָ��:5A A5 06 83 00 13 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x13) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("Cal_CO_A is	%d\r\n", Cal_CO_A);
    }
    /*Cal_CO_B(B��׼��COŨ��)�������� 20180320
    ָ��:5A A5 06 83 00 15 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x15) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        printf("Cal_CO_B is	%d\r\n", Cal_CO_B);
    }
    /*BGas_Num(��������������)�������� 20180320
    ָ��:5A A5 06 83 00 1E 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1E) && (Com3RBuf[6] == 0x01))
    {
        BGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("BGas_Num is	%d\r\n", BGas_Num);
    }
    /*ZGas_Num(��������������)�������� 20180320
    ָ��:5A A5 06 83 00 1F 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1F) && (Com3RBuf[6] == 0x01))
    {
        ZGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("ZGas_Num is	%d\r\n", ZGas_Num);
    }
#endif
}