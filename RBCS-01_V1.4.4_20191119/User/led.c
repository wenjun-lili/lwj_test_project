/**
  * @file  Led.c
	* @brief Led�����ļ�
  */
#include "led.h"
#include "bsp.h"

/**
  * @brief  led_GPIO_Init(void)
	* @datail LED���ų�ʼ��
	* @param  none
	* @return none
  */
void led_GPIO_Init(void)
{

    /*����һ�� GPIO_InitTypeDef ���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOE GPIOF  ʱ�ӳ�ʼ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF, ENABLE);

    /* ����Ϊͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*�����ٶ�Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  //PE6   ONLINE
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //PF13  Fault
    GPIO_Init(GPIOF, &GPIO_InitStructure);


    /* ����������ӿ� LAG1  LAGE2 LAG3 LAG4 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//  PF1 PF2 PF3 PF4
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStructure);


    /*�ر� ONLINE_L ��FAULT_L ��  KR_L KL_L ָʾ�ƵƲ���Ҫ����Ա�趨����������λ���ص�λ�þ���*/
    LED_All_Close();  //
}
/**
  * @brief  LED_All_Close(void)
	* @datail LED���ж��ر�
	* @param  none
	* @return none
  */
void LED_All_Close( void )
{
    // �����𣬶���λ���ҵ�λָʾ�ȸ�����λ���ص�λ�þ���
    GPIO_ResetBits( GPIOE,  GPIO_Pin_6);//  ONLINE

    GPIO_ResetBits( GPIOF, GPIO_Pin_13);// FAULT
}
/**
  * @brief  is_gasbag_exit(uint8_t num)
	* @datail ������״̬�ж�
	* @param  num�����������
	* @return ret��0
  */
uint8_t is_gasbag_exit(uint8_t num)
{
    uint8_t ret = 0;

    switch (num)
    {
    case 0:
        ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_1); //LAG1
        break;
    case 1:
        ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_2);// LAG2
        break;
    case 2:
        ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_3); //LAG3
        break;
    case 3:
        ret = GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_4);// LAG4
        break;
    default :
        break;
    }

    return ret;
}

/**
  * @brief  Send_Gas_Com(void)
	* @datail ��������״̬
	* @param  none
	* @return none
  */
void Send_Gas_Com(void)
{
    if (is_gasbag_exit(0)==1) // LAG1 ����
    {
        FS_GasYes_None(0);
        Delay10ms(6);
    }
    else                    // LAG1 ����
    {
        FS_GasNo_None(0);
        Delay10ms(6);
    }

    if (is_gasbag_exit(1)==1)// LAG2 ����
    {
        FS_GasYes_None(1);
        Delay10ms(6);
    }
    else                     // LAG2����
    {
        FS_GasNo_None(1);
        Delay10ms(6);
    }
    if (is_gasbag_exit(2) == 1)// LAG3����
    {
        FS_GasYes_None(2);
        Delay10ms(6);
    }
    else                     // LAG3 ����
    {
        FS_GasNo_None(2);
        Delay10ms(6);
    }

    if (is_gasbag_exit(3) ==1)// LAG4 ����
    {
        FS_GasYes_None(3);
        Delay10ms(6);
    }
    else                    // LAG4 ����
    {
        FS_GasNo_None(3);
        Delay10ms(6);
    }

    F_Timeout=0;//ͨѶ��ʱ��0
}
