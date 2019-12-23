#include "bsp_exti.h"

/**
 * @brief  NVIC_Configuration
* @datail ����Ƕ�������жϿ�����NVIC
 * @param  ��
 * @retval ��
 */
static void KEY_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* �����ж�Դ*/
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    /* ������ռ���ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    /* ���������ȼ� */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    /* ʹ���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  ���� IOΪEXTI�жϿڣ��������ж����ȼ�
 * @detail KEY1:��λ����  KEY2����������������  KEY3����������������
 * @param  ��
 * @retval ��
 */
void EXTI_Key_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    /*��������GPIO�ڵ�ʱ��*/
    RCC_APB2PeriphClockCmd(KEY1_INT_GPIO_CLK, ENABLE);

    /* ���� NVIC �ж�*/
    KEY_NVIC_Configuration();

    /*--------------------------KEY1����-----------------------------*/
    GPIO_InitStructure.GPIO_Pin = KEY1_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY1_INT_GPIO_PORT, &GPIO_InitStructure);

    GPIO_EXTILineConfig(KEY1_INT_EXTI_PORTSOURCE, KEY1_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY1_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY2����-----------------------------*/
    GPIO_InitStructure.GPIO_Pin = KEY2_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY2_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(KEY2_INT_EXTI_PORTSOURCE, KEY2_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY2_INT_EXTI_LINE;
    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /*--------------------------KEY3����-----------------------------*/
    GPIO_InitStructure.GPIO_Pin = KEY3_INT_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(KEY3_INT_GPIO_PORT, &GPIO_InitStructure);

    /* ѡ��EXTI���ź�Դ */
    GPIO_EXTILineConfig(KEY3_INT_EXTI_PORTSOURCE, KEY3_INT_EXTI_PINSOURCE);
    EXTI_InitStructure.EXTI_Line = KEY3_INT_EXTI_LINE;

    /* EXTIΪ�ж�ģʽ */
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    /* �½����ж� */
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    /* ʹ���ж� */
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
}

/**
  *@breif  �ⲿ�жϷ������
  *@param  none
  *@retval none
  */
void EXTI15_10_IRQHandler(void)
{
    uint16_t i = 0;
    SysTick_Delay_Ms(10);//����
    if(KEY1 == 0) //�ٴ�ȷ���Ƿ���
    {
        for(i = 0; i < 290; i++)
        {
            SysTick_Delay_Ms(10);
            if(KEY1 == 1)
            {
                __set_FAULTMASK(1); // �ر������ж�
                NVIC_SystemReset(); // ��λ
            }
        }
        if(KEY1 == 0) //��������ģʽ���л�
        {
            F_Key = 1;
            flagAgeMode = !flagAgeMode; //�л�����/��ͯģʽ

            /*���ò�ͬģʽ�Ĳ���ֵ*/
            if(flagAgeMode)
            {
                printf("�������ģʽ��ռ�ձ�0��ÿ�βɼ���ʱ0.3s���ɼ�����ʱ0.6s\r\n");
                PWMDuty = 0; //����ȫ�ٹ���
                delayBegin = 300; //��ʼ���ӳ�0.3s��������
                delayAfter = 600; //��������ʱ0.6s�ر�����

            }
            else
            {
                printf("����Ӥ��ģʽ��ռ�ձ�50��ÿ�βɼ���ʱ0.1s���ɼ�����ʱ0.1s\r\n");
                PWMDuty = 50; //Ӥ�����ٹ���
                delayBegin = 100; //��ʼ���ӳ�0.1s��������
                delayAfter = 100; //�����������ر�����

            }

            /*��ͬģʽ��״̬����ʾ*/
            for(i = 1; i <= 10; i++)
            {
                runLedDoing(i % 2); //״̬���������Ϩ��
                SysTick_Delay_Ms(200 + 800 * flagAgeMode); //��˸ʱ��,����ģʽ��1s��˸һ�Σ��׶�ģʽ����˸0.5s
            }

        }

    }

    if((KEY2 == 0) && (flagError == 0)&& (flagInit==1)) //�ٴ�ȷ���Ƿ�KEY2�Ƿ���,ǰ������δ��������
    {
        F_Key = 2;
    }

    if((KEY3 == 0) && (flagError == 0)&& (flagInit==1)) //�ٴ�ȷ���Ƿ�KEY2�Ƿ��£�ǰ������δ��������
    {
        F_Key = 3;
    }

    EXTI_ClearITPendingBit(KEY1_INT_EXTI_LINE);  //���LINE12�ϵ��жϱ�־λ
    EXTI_ClearITPendingBit(KEY2_INT_EXTI_LINE);  //���LINE13�ϵ��жϱ�־λ
    EXTI_ClearITPendingBit(KEY3_INT_EXTI_LINE);  //���LINE14�ϵ��жϱ�־λ
}





/*********************************************END OF FILE**********************/