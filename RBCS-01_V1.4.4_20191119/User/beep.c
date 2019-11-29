#include "beep.h"
#include "timer.h"

// ������SPEAK���ڸߵ�ƽ��������
void BEEP_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;				//GPIOF14
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15 ;				//GPIOE15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_SetBits(GPIOF,GPIO_Pin_14);  // ���1���رշ��������

    GAS_PUB_Closed();// �����ùر�
}

void BEEP_DOING(void)
{
    GPIO_ResetBits(GPIOF,GPIO_Pin_14);  // ���0���򿪷��������
    TIM6Delay_f(1000); //��ʱ1S
    GPIO_SetBits(GPIOF,GPIO_Pin_14);  // ���1���رշ��������
}

// ����
void GAS_PUB_Closed(void)
{
    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
    TIM6Delay_f(2000); //��ʱ2S

}

void GAS_PUB_OPEN(void)
{
    GPIO_SetBits(GPIOE,GPIO_Pin_15);
    TIM6Delay_f(2000); //��ʱ2S
}

