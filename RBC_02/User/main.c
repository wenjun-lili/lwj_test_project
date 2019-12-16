#include "stm32f10x.h"
#include "stdio.h"
#include "sd_fat_app.h"
#include "bsp.h"
#include "GasRoad.h"
#include "systick.h"
#include "process.h"
#include "DWIN.h"
#include "globalval.h"
#include "faultHandle.h"

uint16_t DGUS_Layout_Flag = 1000; //��־DGUS��ʾ��һ�����棬����׼���ú�ű���ֵ��ˢ��ҳ��ʱ����ֵΪ1000 lcy 20180320
extern __IO uint32_t TimeDisplay;// RTC���־

int main()
{
    bsp_init();// �ײ��ʼ��
    OnlinePC();// ����
    framedisplay2(); //Ԥ��
    Bme280Fault();//����һ�������Լ�
    Humidity_Fault();//����Ч���Լ�
    CO2_CO_SensorZERO();//CO2��CO�Լ�
    Rubber_Ring_Fault();//��Ȧ�Լ�
		BtnPressedFlag=0;//����ֵ����

    while(1)
    {
        if(RetFrame3Flag == 1) //�ط�������ʱ��ˢ��
        {
            RetFrame3Flag = 0;
            framedisplayStandby();
        }

        if(TimeDisplay == 1) //���ж�
        {
            TimeDisplay = 0;
            framedisplay_zero(); //��������ƽ
        }

        if(ReceiveFlag6 == 1) // ������6�ֽ�����
        {
            ReceiveFlag6 = 0;
            PC_Cmd_Analy();// ��λ���������
        }

        switch(BtnPressedFlag)
        {
        case 0xCC:   //��׼����ģʽ
            BtnPressedFlag = 0;
            framedisplay11();
            break;

        case 0xDD:   //��ȷ����ģʽ
            BtnPressedFlag = 0;
            framedisplay20();
            break;

        case 0x22:    //�ʿ�ģʽ
            BtnPressedFlag = 0;
            framedisplay37();
            break;

        case 0x11:    //CO2�궨
            BtnPressedFlag = 0;
            framedisplay30();
            break;

        case 0xEE:   //����ģʽ
            BtnPressedFlag = 0;
            framedisplaySet();
            break;

        case 0xFF:  //CO�궨
            BtnPressedFlag = 0;
            framedisplay34();
            break;

        case 0x33: //����ģʽ
            BtnPressedFlag = 0;
            framedisplay47();
            break;

        case 0x55://�����Լ���
            BtnPressedFlag = 0;
            framedisplay45();
            break;

        default:
            break;
        }
    }
}

