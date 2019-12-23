#include "bsp_led.h"

/**
  *@breif  errorLedInit
	*@datail ���ϵƳ�ʼ��
	*@param  none
	*@retval none
**/
void errorLedInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 ;				//GPIOF13
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD,GPIO_Pin_10);  // ��ʼ״̬��Ϊ���

}

/**
  *@breif  errorLedDoing
  *@datail ���ϵƹ���״̬
  *@param  select
           @0���ر�
           @1����
  *@retval none
**/
void errorLedDoing(uint8_t select)
{
    switch(select)
    {
    case 0:
        GPIO_ResetBits(GPIOD,GPIO_Pin_10);  // ���
        break;
    case 1:
        GPIO_SetBits(GPIOD,GPIO_Pin_10);  // ����
        break;
    default:
        break;
    }
}

/**
  *@breif  runLedInit
	*@datail ����״̬�Ƴ�ʼ��
	*@param  none
	*@retval none
**/
void runLedInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;				//GPIOD9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD,GPIO_Pin_9);  // ��ʼ״̬��Ϊ���

}

/**
  *@breif  runLedDoing
  *@datail ���й���״̬
  *@param  select
           @0���ر�
           @1����
  *@retval none
**/
void runLedDoing(uint8_t select)
{
    switch(select)
    {
    case 0:
        GPIO_ResetBits(GPIOD,GPIO_Pin_9);  // ���
        break;
    case 1:
        GPIO_SetBits(GPIOD,GPIO_Pin_9);  // ����
        break;
    default:
        break;
    }
}
/**
  *@breif  CO2LedInit
	*@datail CO2Ũ��״̬�Ƴ�ʼ��
	*@param  none
	*@retval none
**/
void CO2LedInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 ;				//GPIOD9
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOD,GPIO_Pin_8);  // ��ʼ״̬��Ϊ���

}

/**
  *@breif  CO2LedDoing
  *@datail CO2���й���״̬
  *@param  select
           @0���ر�
           @1����
  *@retval none
**/
void CO2LedDoing(uint8_t select)
{
    switch(select)
    {
    case 0:
        GPIO_ResetBits(GPIOD,GPIO_Pin_8);  // ���
        break;
    case 1:
        GPIO_SetBits(GPIOD,GPIO_Pin_8);  // ����
        break;
    default:
        break;
    }
}

/**
  *@breif  ledInit
  *@datail ����led�Ƴ�ʼ��
  *@param  none
  *@retval none
**/
void ledInit(void)
{
    runLedInit();
    errorLedInit();
    CO2LedInit();
}
