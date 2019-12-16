#include "steppermotor.h"
#include "timer.h"

/**
  *@breif  ���������������
	*@param  none
	*@retval none
	*/
static void Motor_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ����TIM3CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOG, ENABLE);

    /* ���÷�������PG8*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /*������λ����PD5*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /*�����ҵ�λ����PG7*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    /*����ʱ������PA7*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  *@breif  TIM2��ʱ��������
	*@param  none
	*@retval none
	*/
static void TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;

    /* -----------------------------------------------------------------------
    TIM3 Channel2 duty cycle = (TIM3_CCR/ TIM3_ARR+1)* 100% = 50%��ռ�ձȣ�
    PWM����Ϊ = 1/(72MHZ /72) * (TIM3_ARR+1)
    ----------------------------------------------------------------------- */

    TIM_TimeBaseStructure.TIM_Period = 499;    // 500�� *1us=0.5ms
    TIM_TimeBaseStructure.TIM_Prescaler = 71;	    //����Ԥ��Ƶ��72��Ƶ����Ϊ1MHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	// ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ�
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	// ����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 249;	   //��������ֵ�������������������ֵʱ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low ;  // ����ʱ������ֵС��CCR_ValʱΪ�ߵ�ƽ
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);	 // ʹ��ͨ��2
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM2���ؼĴ���ARR

    TIM_Cmd(TIM3, DISABLE);                 // ʧ�ܶ�ʱ��2

}

/**
  *@breif  ���������ʼ��
	*@param  none
	*@retval none
	*/
void MotorInit(void)
{
    Motor_GPIO_Config();

    TIM3_Mode_Config();
}

/**
  *@breif  �����������
	*@param  none
	*@retval none
	*/
void Motor1Start(uint8_t dir, uint16_t speed_us)  // �������
{
    if(dir == 0)  // ��ת
    {
        CW_M1();
    }
    else if(dir == 1) // ��ת
    {
        CCW_M1();
    }

    TIM3->ARR = speed_us;

    TIM3->CCR2= speed_us/2;

    TIM_Cmd(TIM3, ENABLE);
}

/**
  *@breif  �������ֹͣ
	*@param  none
	*@retval none
	*/
void Motor1Stop(void)
{
    TIM_Cmd(TIM3, DISABLE); // TIMER2_CH3 ����

    TIM6Delay_f(1500);// �ȴ�1S

}