#include "steppermotor.h"
#include "timer.h"
// ����TIME2_CH3

static void Motor_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /* ����TIM2CLK Ϊ 72MHZ */
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* GPIOA clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOG, ENABLE);

    /* ����motor1��GPIO PA2ʱ������  PG8��������  */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8  ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;  // K_L
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;  // K_R
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  // �����������
    //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
//static void Motor_NVIC_Config(void)
//{
//	NVIC_InitTypeDef NVIC_InitStructure;


//	/* ����TIM2_IRQ�ж�Ϊ�ж�Դ */
//	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//}


static void TIM2_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;


    /* -----------------------------------------------------------------------
    TIM2 Channel3 duty cycle = (TIM3_CCR/ TIM3_ARR+1)* 100% = 50%��ռ�ձȣ�
    PWM����Ϊ = 1/(72MHZ /72) * (TIM3_ARR+1)
    ----------------------------------------------------------------------- */

    /* Time base configuration */

    TIM_TimeBaseStructure.TIM_Period = 499;    // 500�� *1us=0.5ms
    TIM_TimeBaseStructure.TIM_Prescaler = 71;	    /* ����Ԥ��Ƶ��72��Ƶ����Ϊ1MHz */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1 ;	/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  /* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    /* ����ΪPWMģʽ1 */
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 249;	   /* ��������ֵ�������������������ֵʱ����ƽ�������� */
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low ;  /* ����ʱ������ֵС��CCR_ValʱΪ�ߵ�ƽ */
    TIM_OC3Init(TIM2, &TIM_OCInitStructure);	 /* ʹ��ͨ��3 */
    TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM2, ENABLE);			 /* ʹ��TIM2���ؼĴ���ARR */

//	TIM_ITConfig(TIM2,TIM_IT_CC3, ENABLE);		 /* ʹ�ܱȽ��ж� */

    /* TIM2 enable counter */
    TIM_Cmd(TIM2, DISABLE);                   /* ʹ�ܶ�ʱ��2 */
//TIM_Cmd(TIM2, ENABLE);

}


void MotorInit(void)
{
    Motor_GPIO_Config();
//Motor_NVIC_Config();
    TIM2_Mode_Config();

//	GPIO_ResetBits(GPIOG, GPIO_Pin_5);// �ر�ʹ��
//
////	GPIO_SetBits(GPIOG, GPIO_Pin_3);// ȫ/�벽������
//	GPIO_ResetBits(GPIOG, GPIO_Pin_3);


}


void Motor1Start(uint8_t dir, uint16_t speed_us)  // �������
{

// 	GPIO_SetBits(GPIOG, GPIO_Pin_5); //MOTOR1ʹ��
//  GPIO_ResetBits(GPIOG, GPIO_Pin_5); //MOTOR1ʹ��

    if(dir == 0)  // ��ת
    {
        CW_M1();
    }
    else if(dir == 1) // ��ת
    {
        CCW_M1();
    }

    TIM2->ARR = speed_us;

    TIM2->CCR3= speed_us/2;
//	TIM2->CNT = 0x0000;
    TIM_Cmd(TIM2, ENABLE);
}

void Motor1Stop(void)
{
    TIM_Cmd(TIM2, DISABLE); // TIMER2_CH3 ����
//  GPIO_ResetBits(GPIOG, GPIO_Pin_5); //ʧ��
    //2017.6.20 ���ͣ�º���ʱ
    TIM6Delay_f(1500);// �ȴ�1S

}
