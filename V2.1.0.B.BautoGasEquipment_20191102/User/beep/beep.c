#include "beep.h"

/**
  *@breif  蜂鸣器初始化
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

    GPIO_ResetBits(GPIOD,GPIO_Pin_11);  // 输出低电平，关闭蜂鸣器输出
}

/**
  *@breif 蜂鸣器工作模式
  *@param select
        @1：响1声
        @2：持续响
        @其他 响select声
  *@retval none
  */

void beepMode(uint8_t select)
{
    uint8_t i=0;
    if(select==2)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_11);  // 输出1，打开蜂鸣器输出
    }
    for(i=0; i<select; i++)
    {
        GPIO_SetBits(GPIOD,GPIO_Pin_11);  // 输出0，关闭蜂鸣器输出
        Delay100ms(5);
        GPIO_ResetBits(GPIOD,GPIO_Pin_11);  // 输出1，打开蜂鸣器输出
        Delay100ms(5);
    }
}

