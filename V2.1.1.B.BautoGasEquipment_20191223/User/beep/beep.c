#include "beep.h"

/**
  *@breif  ��������ʼ��
	*@param  none
	*@retval none
	*/
void beepInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;				//GPIOD11
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD,GPIO_Pin_11);  // ����͵�ƽ���رշ��������
}

/**
  *@breif ����������ģʽ
  *@param select
        @1����1��
        @2��������
        @���� ��select��
  *@retval none
  */

void beepMode(uint8_t select)
{
    uint8_t i=0;
    if(select==2)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_11);  // ���1���򿪷��������
    }
    for(i=0; i<select; i++)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_11);  // ���0���رշ��������
        Delay100ms(5);
        GPIO_ResetBits(GPIOD,GPIO_Pin_11);  // ���1���򿪷��������
        Delay100ms(5);
    }
}

