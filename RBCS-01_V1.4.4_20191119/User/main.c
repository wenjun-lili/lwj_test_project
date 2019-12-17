/**
*@file main
*@brief RBCS-01������
*@autor ��������
*@version V1.4.4
*@date 2019-11-22
*/
#include "stm32f10x.h"
#include "stdio.h"
#include "sd_fat_app.h"
#include "bsp.h"
#include "CO2_Serial.h"
#include "BME280.h"

extern __IO uint32_t TimeDisplay;///< RTC���־
uint8_t RetFrame3Flag=1;///< �������־

int main()
{
    bsp_init();/// �ײ��ʼ��
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)==0)  /// ��������CK�����Ը��������ó�ʼֵ
    {
        BtnPressedFlag=-1;
        Chu_Zhi();
    }
    else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)==0)/// ��������DEC���������л�Ϊ����
    {
        BtnPressedFlag=-1;
        F_tiaoji=1;///������־
    }
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0)//��������SET�������������Լ���
    {
        BtnPressedFlag=-1;
        framedisplayAirTight();
        airTightTest();
    }

    OnlinePC();/// ������ʾOnlining    

    framedisplay1(); /// ��ʾWarming up.......Ԥ�ȳ���Ҳ������
    BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
    if(F_DC==1)
    {
        printf("BME280\r\n");
        printf("Ԥ��ʪ��=%f\r\n",BME_Humidity);
        printf("Ԥ���¶�=%f\r\n",BME_Temperature);
        printf("Ԥ����ѹ=%f\r\n",BME_Pressure);
    }
    Bme280Fault();///�Դ��������Ͻ������
    Humidity_Fault();///ʪ����ֵ�ж�
    CO2_CO_SensorZERO();/// CO��CO2 ZERO �����ж�
    Rubber_Ring_Fault();///��Ȧʹ�ô���Ԥ��-20180621
    BtnPressedFlag=-1;
    while(1)
    {
        if(RetFrame3Flag ==1) ///�ط��������ʱ��ˢ��
        {
            framedisplay3(); 
            RetFrame3Flag = 0;
        }

        if(TimeDisplay ==1) ///ʱ�ӱ�־
        {
            TimeDisplay=0;
            framedisplay3_sub();
        }
        if(ReceiveFlag6==1) ///����������������
        {
            ReceiveFlag6=0;
            PC_Cmd_Analy();///��λ���������
        }
        switch(BtnPressedFlag)
        {
        case 0:   ///������
            BtnPressedFlag=-1;
            RUN_Program();
            break;
        case 1:   ///���
            BtnPressedFlag=-1;
            PeiGas_PROM();
            break;
        case 2:    ///�¼�
            BtnPressedFlag=-1;
            QC_PROM();
            break;
        case 3:    ///�ϼ�
            BtnPressedFlag=-1;
            CO2_CON();
            break;
        case 4:   ///set��
            BtnPressedFlag=-1;
            Set_Para();
            break;
        case 5:  ///�Ҽ�
            BtnPressedFlag=-1;
            Curve_PROM();
            break;
        default:
            break;

        }
    }
}









