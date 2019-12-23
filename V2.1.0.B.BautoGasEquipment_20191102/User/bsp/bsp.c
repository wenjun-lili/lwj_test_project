#include "bsp.h"

#define runLED   PDout(9)// PD9
#define errorLED PDout(10)// PD10
#define co2LED   PDout(8) //PD8

static void TIM7_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  * @brief  bsp_init
  * @datail �ײ��ʼ��
  * @param  none
  * @return none
*/
void bspInit(void)
{
    NVIC_Config();//NVIC����

    delay_init();//��ʱ�������ó�ʼ��

    USART_Config();//��ʼ��USART

    I2C_BME280_Init();//����һ��������ʼ��

    beepInit();//��������ʼ��

    ledInit();//led�Ƴ�ʼ��

    evalveInit();//��ŷ���ʼ��

    EXTI_Key_Config();//������ʼ��

    USART3_Config();//CO2��������  CO2������Ԥ�����ڴ��Ϊ50s��ʵ��

    ADVANCE_TIM_Init();//tim8��ʼ���������ó�ʼ��

    TIM7_Config();//������ʱ����ʼ��
     
		TIM6_Config();//������ʱ����ʼ��

    breathSensorInit();//����������������ʼ��

}
/**
  *@breif  TIM7��ʱ������
	*@detail ��ʱ�������ж�����Ϊ200ms
	*@param  none
	*@retval none
	*/
void TIM7_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* ����TIM6CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period = 1999;  //200ms

    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 7199;
    /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM7, TIM_IT_Update);

    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM7, DISABLE);

    TIM7_NVIC_Config();
}

/**
  *@breif �Լ�����
  *@detail V01��V02�򿪣�ȫ���������ã���ϴ10s����ϴ���ȡ��ѹֵ��
           �粻��[500,1200]hPa��Χ�ڣ���������ϵ�
  *@param  none
  *@retval none
  */
void selfInspection(void)
{
    evalveOpen(1);//�򿪵�ŷ�1����Ϊ2��1ͨ

    evalveClose(2);//�رյ�ŷ�2

    timeCnt=10;//��ϴʱ��10s

    TIM_Cmd(TIM7,ENABLE);//�򿪶�ʱ��

    TIM_SetCompare3(TIM8,0);//����ռ�ձ�0��������1�͵�ƽȫ��

    while(timeCnt!=0);//�鿴10s�Ƿ����

    TIM_Cmd(TIM7,DISABLE);//�رն�ʱ��

    TIM_SetCompare3(TIM8,100);//����ռ�ձ�100%������1ֹͣ����

    BME280_ReadPressure(&BME_Pressure);//��ȡ����һ����������ѹֵ
    printf("�Լ���ѹֵΪ��%f\r\n",BME_Pressure);

    if((BME_Pressure>1200)||(BME_Pressure<500))///��ѹ�����ж�
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPressure(&BME_Pressure);
        if((BME_Pressure>1200)||(BME_Pressure<500))
        {
            errorLedDoing(1);//���ϵ�������
            while(1);//����
        }
    }
    evalveAllClose();//�ر����е�ŷ�
		
		beepMode(1);
    

    
}
/**
  *@breif  ��������
  *@detail ����ÿ�β���ǰ��ʱ����������ռ�ձ�
  *@param  none
  *@retval none
  */
void parametersSet(void)
{
    uint16_t num=0;

    printf("�Ƿ��������ò�������ظ�(Y/N)-�������д\r\n");
    while(Rec1FinishF!=1)
    {
        Delay10ms(1);
        num++;
        if(num>400)//���δ�𸴣���ֱ������
        {
            printf("δ���ò���\r\n");
            break;
        }
    }

    if(Com1RBuf[0]==0x59)
    {
        Rec1FinishF=0;//���ձ�־����
        onLineFlag=1;//������־λ��1

        printf("���������ǰ��ʱʱ�䣬��λ��ms\r\n");
        while(Rec1FinishF!=1);//�ȴ��������
        Rec1FinishF=0;
        delayBegin=inputTemp;

        printf("�������������ʱʱ�䣬��λ��ms\r\n");
        while(Rec1FinishF!=1);//�ȴ��������
        Rec1FinishF=0;
        delayAfter=inputTemp;

        printf("��������������ռ�ձ�(�ߵ�ƽ����)����λ���ٷֱ�\r\n");
        while(Rec1FinishF!=1);//�ȴ��������
        Rec1FinishF=0;
        PWMDuty=inputTemp;
    }
    printf("����ǰ��ʱ=%d\r\n",delayBegin);
    printf("��������ʱ=%d\r\n",delayAfter);
    printf("ռ�ձȣ��ߵ�ƽ������=%d\r\n",PWMDuty);
    Rec1FinishF=0;//���ձ�־����

}

/**
  *@breif ��ʱ��3��ʼ��
  *@param
         @arr �Զ���װֵ
         @psc ʱ��Ԥ��Ƶֵ
  *@retval none

  */
void TIM3_Int_Init(uint16_t arr,uint16_t psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

    //��ʱ��TIM3��ʼ��
    TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
    TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM3�ж�,��������ж�

    //�ж����ȼ�NVIC����
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�����ȼ�3��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
    NVIC_Init(&NVIC_InitStructure);  //��ʼ��NVIC�Ĵ���


    TIM_Cmd(TIM3, ENABLE);  //ʹ��TIMx
}
/**
  *@breif ��ʱ��3�жϷ������
  *@param none
  *@retval none
  */
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־
        co2LED=!co2LED;
    }
}

/**
  *@breif  TIM6��ʱ���ж����ȼ�����
	*@param  none
	*@retval none
	*/
static void TIM6_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/**
  *@breif  TIM6��ʱ������
	*@detail ��ʱ�������ж�����Ϊ50ms
	*@param  none
	*@retval none
	*/
void TIM6_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    /* ����TIM6CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

    /* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period = 4999;  //50ms

    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler = 719;
    /* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);

    TIM_ClearITPendingBit(TIM6, TIM_IT_Update);

    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);

    TIM_Cmd(TIM6, DISABLE);

    TIM6_NVIC_Config();
}
