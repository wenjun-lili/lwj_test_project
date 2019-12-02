#include "bsp.h"
#include "globalval.h"
#include "beep.h"
#include "CO2_Serial.h"
#include "BME280.h"

struct rtc_time systmtime;

/**
  * @brief  bsp_init(void)
	* @datail bsp初始化
	* @param  none
	* @return none
	*/
void  bsp_init(void)
{

    NVIC_Config();//NVIC配置

    led_GPIO_Init();// 关闭了联机与错误指示灯，气袋LAGE也已经定义

    USART1_Config();//串口1初始化

    EValve_Init();//电磁阀初始化

    TIM6_Config();//定时器6初始化
    BEEP_Init();// 定义了蜂鸣器与泵的GPIO口，蜂鸣器已经关，泵在这里没有操作
    MotorInit();//步进电机初始化

    Delay10ms(1);
    LCD_Init();//24064LCM屏初始化

    TIM7_Config();	//定时器7初始化
    RTC_Init(&systmtime);//RTC初始化
    sd_fs_init();//sd卡初始化

    ADS7816_GPIO_Init();// 三个引脚都置高
    CD4051_Init();// 单刀八掷开关 ，所有通道截止

    bsp_InitTimer();// 系统滴答定时器初始化
    bsp_InitButton(); //按键初始化

    ADC123_Init();
    I2CEEPROM_Init();//eeprom初始化

    QiGuang_Reset();	// 气缸复位

    BME280_Init();//三合一传感器初始化 20181218

    ee_ReadBytes((uint8_t *)&CO_ZERO, 0, 2);//读取CO_ZERO值
    ee_ReadBytes((uint8_t *)&CO2_PB, 2, 2);//读取CO2_PB值
    ee_ReadBytes((uint8_t *)&R_XB, 4, 2);
    ee_ReadBytes((uint8_t *)&CO2_Coeff, 6, 2);
    ee_ReadBytes((uint8_t *)&Ring_Num,10,2);//读取橡胶圈使用次数 20180621
    ee_ReadBytes((uint8_t *)&CO2_ZERO, 8, 2);
		ee_ReadBytes((uint8_t *)&F_GasType,14,2);
    ee_ReadBytes((uint8_t *)&T0_Var,16,2);
		ee_ReadBytes((uint8_t *)&gasTypeFactor,20,2);
    ee_ReadBytes((uint8_t *)&HumidityThreshold,18,2);
    T0=(float)T0_Var/10;

}
