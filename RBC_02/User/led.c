
#include "led.h"
#include "bsp.h"


void led_GPIO_Init(void)
{
    // ��������ONLINE      PE6   rbcs-02��ɾ�� 20180321
    //�� FAULT            PF13

    /*����һ�� GPIO_InitTypeDef ���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOE GPIOF  ʱ�ӳ�ʼ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);

    /* ����Ϊͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*�����ٶ�Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //PF13  Fault
    GPIO_Init(GPIOF, &GPIO_InitStructure);


    /* ����������ӿ� LAG1 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//  PF1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStructure);


    /*�ر� ONLINE_L ��FAULT_L ��  KR_L KL_L ָʾ�ƵƲ���Ҫ����Ա�趨����������λ���ص�λ�þ���*/
    LED_All_Close();  //
}

void LED_All_Close( void )
{
    // �����𣬶���λ���ҵ�λָʾ�ȸ�����λ���ص�λ�þ���
//	GPIO_ResetBits( GPIOE,  GPIO_Pin_6);//  ONLINE

    GPIO_ResetBits( GPIOF, GPIO_Pin_13);// FAULT
}

uint8_t is_gasbag_exit(uint8_t num)
{
    uint8_t ret = 0;

    switch (num)
    {
    case 0:
        ret = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8); //LAG1
        break;
//    case 1:
//        ret = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);// LAG2
////        break;
////    case 2:
////        ret = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10); //LAG3
//        break;
    default :
        break;
    }

    return ret;
}


void Send_Gas_Com(void)
{

    if (is_gasbag_exit(0)==1) // LAG1 ����
    {
        FS_GasYes(0);
    }
    else                    // LAG1 ����
    {
        FS_GasNo(0);
    }

//    if (is_gasbag_exit(1)==1)// LAG2 ����
//    {
//        FS_GasYes(1);
//        Delay10ms(6);
//    }
//    else                     // LAG2����
//    {
//        FS_GasNo(1);
//        Delay10ms(6);
//    }
//    if (is_gasbag_exit(2) == 1)// LAG3����
//    {
//        FS_GasYes(2);
//        Delay10ms(6);
//    }
//    else                     // LAG3 ����
//    {
//        FS_GasNo(2);
//        Delay10ms(6);
//    }
    F_Timeout=0;//ͨѶ��ʱ��0

}