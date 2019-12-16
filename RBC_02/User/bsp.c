#include "bsp.h"
#include "globalval.h"
#include "DWIN.h"
#include "beep.h"
#include "BME280.h"

struct rtc_time systmtime;
extern void Touch_Process(void);
extern void Event_Process(void);

/**
  *@breif  �����ʼ��
	*@param  none
	*@retval none
  */
void bsp_init(void)
{
    NVIC_Config();//�жϳ�ʼ��

    USART1_Config();//����1��ʼ��

    USART2_Config();//����2��ʼ��

    EValve_Init();//��ŷ���ʼ��

    TIM6_Config();//��ʱ����ʼ��

    GAS_PUB_Init();//�������õ�GPIO�ڣ��ô��ڹر�״̬

    MotorInit();//���������ʼ��

    TIM7_Config();

    RTC_Init(&systmtime);//RTC��ʼ����

    sd_fs_init();//SD����ʼ��

    led_GPIO_Init();// �ر������������ָʾ�ƣ�����LAGEҲ�Ѿ�����

    ADS7816_GPIO_Init();// �������Ŷ��øߣ�ADC

    bsp_InitTimer();// ϵͳ�δ�ʱ����ʼ��

    ADC123_Init();//CO2����ADC��ʼ��

    I2CEEPROM_Init();//EEPROM��ʼ��

    QiGuang_Reset();	//���׸�λ

    BME280_Init();//����һ��������ʼ�� 20181218

    Dwins_Variable();//DGUSͨѶָ��֡ͷ��ʼ��

    DGUS_Set_para();//�����ĳ�ʼ��
}
/**
  *@breif  bsp_RunPer10ms
	*@detail �ú���ÿ��10ms��Systick�жϵ���1�Ρ���� systick.c�Ķ�ʱ�жϷ������һЩ��Ҫ�����Դ���
 			     ��������Է��ڴ˺��������磺����ɨ�衢���������п��Ƶȡ�
  *@param  none
	*@retval none
*/
void bsp_RunPer10ms(void)
{
    Touch_Process();

}


