#include "bsp.h"
#include "globalval.h"
#include "DWIN.h"
#include "beep.h"
#include "BME280.h"

struct rtc_time systmtime;
extern void Touch_Process(void);
extern void Event_Process(void);

/**
  *@breif  整体初始化
	*@param  none
	*@retval none
  */
void bsp_init(void)
{
    NVIC_Config();//中断初始化

    USART1_Config();//串口1初始化

    USART2_Config();//串口2初始化

    EValve_Init();//电磁阀初始化

    TIM6_Config();//定时器初始化

    GAS_PUB_Init();//定义气泵的GPIO口，泵处于关闭状态

    MotorInit();//步进电机初始化

    TIM7_Config();

    RTC_Init(&systmtime);//RTC初始化，

    sd_fs_init();//SD卡初始化

    led_GPIO_Init();// 关闭了联机与错误指示灯，气袋LAGE也已经定义

    ADS7816_GPIO_Init();// 三个引脚都置高，ADC

    bsp_InitTimer();// 系统滴答定时器初始化

    ADC123_Init();//CO2测量ADC初始化

    I2CEEPROM_Init();//EEPROM初始化

    QiGuang_Reset();	//气缸复位

    BME280_Init();//三合一传感器初始化 20181218

    Dwins_Variable();//DGUS通讯指令帧头初始化

    DGUS_Set_para();//参数的初始化
}
/**
  *@breif  bsp_RunPer10ms
	*@detail 该函数每隔10ms被Systick中断调用1次。详见 systick.c的定时中断服务程序。一些需要周期性处理
 			     的事务可以放在此函数。比如：按键扫描、蜂鸣器鸣叫控制等。
  *@param  none
	*@retval none
*/
void bsp_RunPer10ms(void)
{
    Touch_Process();

}


