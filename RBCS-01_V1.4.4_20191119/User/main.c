/**
*@file main
*@brief RBCS-01主程序
*@autor 先亚生物
*@version V1.4.4
*@date 2019-11-22
*/
#include "stm32f10x.h"
#include "stdio.h"
#include "sd_fat_app.h"
#include "bsp.h"
#include "CO2_Serial.h"
#include "BME280.h"

extern __IO uint32_t TimeDisplay;///< RTC秒标志
uint8_t RetFrame3Flag=1;///< 主界面标志

int main()
{
    bsp_init();/// 底层初始化
    if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8)==0)  /// 开机长按CK键可以给各参数置初始值
    {
        BtnPressedFlag=-1;
        Chu_Zhi();
    }
    else if(GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10)==0)/// 开机长按DEC键，程序切换为调机
    {
        BtnPressedFlag=-1;
        F_tiaoji=1;///调机标志
    }
    else if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_12)==0)//开机长按SET键，进入气密性检验
    {
        BtnPressedFlag=-1;
        framedisplayAirTight();
        airTightTest();
    }

    OnlinePC();/// 联机显示Onlining    

    framedisplay1(); /// 显示Warming up.......预热程序也在里面
    BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
    if(F_DC==1)
    {
        printf("BME280\r\n");
        printf("预后湿度=%f\r\n",BME_Humidity);
        printf("预后温度=%f\r\n",BME_Temperature);
        printf("预后气压=%f\r\n",BME_Pressure);
    }
    Bme280Fault();///对传感器故障进行诊断
    Humidity_Fault();///湿度阈值判断
    CO2_CO_SensorZERO();/// CO与CO2 ZERO 故障判断
    Rubber_Ring_Fault();///橡胶圈使用次数预警-20180621
    BtnPressedFlag=-1;
    while(1)
    {
        if(RetFrame3Flag ==1) ///重返主界面的时候刷新
        {
            framedisplay3(); 
            RetFrame3Flag = 0;
        }

        if(TimeDisplay ==1) ///时钟标志
        {
            TimeDisplay=0;
            framedisplay3_sub();
        }
        if(ReceiveFlag6==1) ///接收完了整包数据
        {
            ReceiveFlag6=0;
            PC_Cmd_Analy();///上位机命令解析
        }
        switch(BtnPressedFlag)
        {
        case 0:   ///测量键
            BtnPressedFlag=-1;
            RUN_Program();
            break;
        case 1:   ///左键
            BtnPressedFlag=-1;
            PeiGas_PROM();
            break;
        case 2:    ///下键
            BtnPressedFlag=-1;
            QC_PROM();
            break;
        case 3:    ///上键
            BtnPressedFlag=-1;
            CO2_CON();
            break;
        case 4:   ///set键
            BtnPressedFlag=-1;
            Set_Para();
            break;
        case 5:  ///右键
            BtnPressedFlag=-1;
            Curve_PROM();
            break;
        default:
            break;

        }
    }
}









