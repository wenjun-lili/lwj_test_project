#include "beep.h"
#include "timer.h"

/**
  *@breif  气泵初始化
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
    GAS_PUB_Closed();// 将气泵关闭
}

/**
  *@breif  蜂鸣器响一声
	*@param  none
	*@retval none
  */
void BEEP_DOING(void)
{
    GPIO_ResetBits(GPIOF,GPIO_Pin_14);  // 输出0，打开蜂鸣器输出
    TIM6Delay_f(1000); //延时1S
    GPIO_SetBits(GPIOF,GPIO_Pin_14);  // 输出1，关闭蜂鸣器输出
}


/**
  *@breif  气泵关闭
	*@param  none
	*@retval none
  */
void GAS_PUB_Closed(void)
{
    GPIO_ResetBits(GPIOE,GPIO_Pin_15);
    TIM6Delay_f(2000); //延时2S

}

/**
  *@breif  气泵打开
	*@param  none
	*@retval none
  */
void GAS_PUB_OPEN(void)
{
    GPIO_SetBits(GPIOE,GPIO_Pin_15);
    TIM6Delay_f(2000); //延时2S
}


