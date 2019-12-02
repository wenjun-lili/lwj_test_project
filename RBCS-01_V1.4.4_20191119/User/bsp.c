#include "bsp.h"
#include "globalval.h"
#include "beep.h"
#include "CO2_Serial.h"
#include "BME280.h"

struct rtc_time systmtime;

/**
  * @brief  bsp_init(void)
	* @datail bsp��ʼ��
	* @param  none
	* @return none
	*/
void  bsp_init(void)
{

    NVIC_Config();//NVIC����

    led_GPIO_Init();// �ر������������ָʾ�ƣ�����LAGEҲ�Ѿ�����

    USART1_Config();//����1��ʼ��

    EValve_Init();//��ŷ���ʼ��

    TIM6_Config();//��ʱ��6��ʼ��
    BEEP_Init();// �����˷�������õ�GPIO�ڣ��������Ѿ��أ���������û�в���
    MotorInit();//���������ʼ��

    Delay10ms(1);
    LCD_Init();//24064LCM����ʼ��

    TIM7_Config();	//��ʱ��7��ʼ��
    RTC_Init(&systmtime);//RTC��ʼ��
    sd_fs_init();//sd����ʼ��

    ADS7816_GPIO_Init();// �������Ŷ��ø�
    CD4051_Init();// ������������ ������ͨ����ֹ

    bsp_InitTimer();// ϵͳ�δ�ʱ����ʼ��
    bsp_InitButton(); //������ʼ��

    ADC123_Init();
    I2CEEPROM_Init();//eeprom��ʼ��

    QiGuang_Reset();	// ���׸�λ

    BME280_Init();//����һ��������ʼ�� 20181218

    ee_ReadBytes((uint8_t *)&CO_ZERO, 0, 2);//��ȡCO_ZEROֵ
    ee_ReadBytes((uint8_t *)&CO2_PB, 2, 2);//��ȡCO2_PBֵ
    ee_ReadBytes((uint8_t *)&R_XB, 4, 2);
    ee_ReadBytes((uint8_t *)&CO2_Coeff, 6, 2);
    ee_ReadBytes((uint8_t *)&Ring_Num,10,2);//��ȡ��Ȧʹ�ô��� 20180621
    ee_ReadBytes((uint8_t *)&CO2_ZERO, 8, 2);
		ee_ReadBytes((uint8_t *)&F_GasType,14,2);
    ee_ReadBytes((uint8_t *)&T0_Var,16,2);
		ee_ReadBytes((uint8_t *)&gasTypeFactor,20,2);
    ee_ReadBytes((uint8_t *)&HumidityThreshold,18,2);
    T0=(float)T0_Var/10;

}
