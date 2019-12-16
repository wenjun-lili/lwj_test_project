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

uint16_t DGUS_Layout_Flag = 1000; //标志DGUS显示哪一个界面，界面准备好后才被赋值，刷新页面时被赋值为1000 lcy 20180320
extern __IO uint32_t TimeDisplay;// RTC秒标志

int main()
{
    bsp_init();// 底层初始化
    OnlinePC();// 联机
    framedisplay2(); //预热
    Bme280Fault();//三合一传感器自检
    Humidity_Fault();//干燥效果自检
    CO2_CO_SensorZERO();//CO2和CO自检
    Rubber_Ring_Fault();//橡胶圈自检
		BtnPressedFlag=0;//按键值归零

    while(1)
    {
        if(RetFrame3Flag == 1) //重返主界面时候刷新
        {
            RetFrame3Flag = 0;
            framedisplayStandby();
        }

        if(TimeDisplay == 1) //秒中断
        {
            TimeDisplay = 0;
            framedisplay_zero(); //测试零点电平
        }

        if(ReceiveFlag6 == 1) // 接收完6字节数据
        {
            ReceiveFlag6 = 0;
            PC_Cmd_Analy();// 上位机命令解析
        }

        switch(BtnPressedFlag)
        {
        case 0xCC:   //标准测量模式
            BtnPressedFlag = 0;
            framedisplay11();
            break;

        case 0xDD:   //精确测量模式
            BtnPressedFlag = 0;
            framedisplay20();
            break;

        case 0x22:    //质控模式
            BtnPressedFlag = 0;
            framedisplay37();
            break;

        case 0x11:    //CO2标定
            BtnPressedFlag = 0;
            framedisplay30();
            break;

        case 0xEE:   //设置模式
            BtnPressedFlag = 0;
            framedisplaySet();
            break;

        case 0xFF:  //CO标定
            BtnPressedFlag = 0;
            framedisplay34();
            break;

        case 0x33: //配气模式
            BtnPressedFlag = 0;
            framedisplay47();
            break;

        case 0x55://气密性检验
            BtnPressedFlag = 0;
            framedisplay45();
            break;

        default:
            break;
        }
    }
}

