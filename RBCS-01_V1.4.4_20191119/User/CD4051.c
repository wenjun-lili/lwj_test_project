#include "CD4051.h"

// CD4051�൱��һ�������������أ����ؽ�ͨ��һͨ�����������3λ��ַ��ABC��������
/*
   INH �ǽ�ֹ�ˣ�����INH" =1ʱ����ͨ��������ͨ
   A0����A2��ַ
   IO/O0����I7/O7 ���������
   INH��ֹ��
	 O/I�������/�����
	 VDD����Դ
	 VEEģ���źŵ�

	 INH  C  B  A  ѡ�����ͨ��
	  0   0  0  0       0    CO_put
		0   0  0  1       1    CO2_out
		0   0  1  0       2    GAS_TEM_IN
		0   0  1  1       3    SES_TEM_IN
*/
void CD4051_Init(void)
{
    /*����һ�� GPIO_InitTypeDef ���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOE GPIOF  ʱ�ӳ�ʼ��*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE);

    /* ����Ϊͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*�����ٶ�Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 ;  // INH INA  INB
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    IN_Select(0); // �ر����е�ͨ��

}
/**
  * @breif  IN_Select(uint8_t i)
  * @datail ��ŷ�ѡ��
  * @param  i:�����0 ��ʾINH=1���ر����е�ͨ��
              �����1 ��ʾ ѡ��CO��ģ�����źŽ���ADS7816
              �����2 ��ʾ ѡ��CO2��ģ�����źŽ���ADS7816
  */

void IN_Select(uint8_t i)
{
    switch(i)
    {
    case 0:// �ر�����ͨ��
        GPIO_SetBits(GPIOG,GPIO_Pin_2);//INH=1
        GPIO_ResetBits(GPIOG,GPIO_Pin_3 | GPIO_Pin_4);// INA INB =00
        break;
    case 1:// ѡ��ͨ��1��CO_OUT_PUT
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_ResetBits(GPIOG,GPIO_Pin_3 | GPIO_Pin_4);// INA INB =00
        break;
    case 2:// ѡ��ͨ��2  CO2_OUT
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_SetBits(GPIOG,GPIO_Pin_3 );// INA INB =10
        GPIO_ResetBits(GPIOG,GPIO_Pin_4);
        break;
    case 3:// ѡ��ͨ��3   GAS_TEM_IN
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_ResetBits(GPIOG,GPIO_Pin_3 );// INA INB =01
        GPIO_SetBits(GPIOG,GPIO_Pin_4);
        break;
    case 4: // ѡ��ͨ��4   SES_TEM_IN
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_SetBits(GPIOG,GPIO_Pin_3 | GPIO_Pin_4);// INA INB =00
        break;
    default:
        break;
    }

}


