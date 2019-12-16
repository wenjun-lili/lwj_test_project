#include "beep.h"
#include "timer.h"

/**
  *@breif  ���ó�ʼ��
	*@param  none
	*@retval none
  */
void GAS_PUB_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;				//GPIOE15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
    GAS_PUB_Closed();// �����ùر�
}

/**
  *@breif  ��������һ��
	*@param  none
	*@retval none
  */
void BEEP_DOING(void)
{
    GPIO_ResetBits(GPIOF,GPIO_Pin_14);  // ���0���򿪷��������
    TIM6Delay_f(1000); //��ʱ1S
    GPIO_SetBits(GPIOF,GPIO_Pin_14);  // ���1���رշ��������
}


/**
  *@breif  ���ùر�
	*@param  none
	*@retval none
  */
void GAS_PUB_Closed(void)
{
    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
    TIM6Delay_f(2000); //��ʱ2S

}

/**
  *@breif  ���ô�
	*@param  none
	*@retval none
  */
void GAS_PUB_OPEN(void)
{
    GPIO_SetBits(GPIOE,GPIO_Pin_15);
    TIM6Delay_f(2000); //��ʱ2S
}


