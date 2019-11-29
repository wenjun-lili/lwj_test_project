/*
ADC�Լ�DMA����
*/
#include "adc.h"
#include "globalval.h"
#include "math.h"
#include "bsp.h"

#define ADC3_DR_Address    ((u32)0x40013C00+0x4c)
#define SPI_MASTER_DR_Base      0x4000380C
#define buffer_Size  1000

//__IO uint16_t ADC_ConvertedValue[7];			/* ϵͳ����ģ���� */

__IO uint16_t ADC_ConvertedValue2[4];			/* �����¶��ź� */
__IO uint16_t ADC_ConvertedValue3;				/* ѹ���� */
uint16_t  I2S_DMA_buffer[buffer_Size];
uint16_t  ADC_buffer[buffer_Size];
uint32_t  ADC_Convert[buffer_Size/2]; //ADCת������

int32_t  AK5351L_Data[buffer_Size/4];//���ڴ洢����������
int32_t  AK5351R_Data[buffer_Size/4];//���ڴ洢����������

/**
  * @brief  ʹ��ADC��DMA��ʱ�ӣ���ʼ��PA1, PC1~5
  * @param  ��
  * @retval ��
  */
static void ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*ADCʱ�����ֻ����14M������ADCʱ�ӣ�ΪPCLK2��8��Ƶ����9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
    /* Enable ADC1, GPIOA and GPIOC clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PC1~5  as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//����ʱ������������


    /* Configure PA1 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//����ʱ������������
}


static void ADC1_2_NVIC_Config(void)//�ж�����
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure and enable ADC interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = ADC1_2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
static void ADC2_Config(void)
{
    ADC_InitTypeDef ADC_InitStructure;

    /* ADC2 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// ����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;// ��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//��ֹ��������ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//��ʹ���ⲿ����ģʽ
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�ɼ������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 1;// Ҫת����ͨ����Ŀ1
    ADC_Init(ADC2, &ADC_InitStructure);
    /* ADC2 regular channels configuration */
    /*����ADC2��ͨ��1Ϊ7.5���������ڣ�����Ϊ1*/
    ADC_RegularChannelConfig(ADC2, ADC_Channel_1, 1, ADC_SampleTime_7Cycles5);

    /* Enable ADC2 */
    ADC_Cmd(ADC2, ENABLE);
    /* Enable ADC2 reset calibration register */
    ADC_ResetCalibration(ADC2);
    /* Check the end of ADC2 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC2));
    /* Start ADC2 calibration */
    ADC_StartCalibration(ADC2);
    /* Check the end of ADC2 calibration */
    while(ADC_GetCalibrationStatus(ADC2));

    ADC1_2_NVIC_Config();
    ADC_ClearFlag(ADC2, ADC_FLAG_EOC);
    /* Enable ADC2 EOC interrupt */
    ADC_ITConfig(ADC2, ADC_IT_EOC, ENABLE);

    /* Start ADC2 Software Conversion */
    ADC_SoftwareStartConvCmd(ADC2, ENABLE);
}


#if 0
static void ADC3_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;

    /* DMA2 channel5 configuration */
    DMA_DeInit(DMA2_Channel5);
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	 			//ADC��ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue2;	//�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//�����ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				//�ڴ��ַ���̶�
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//����
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;										//ѭ������
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//ѭ������
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel5, &DMA_InitStructure);

    //DMA2_Channel4_5_NVIC_Config();
    /* Enable DMA2 channel5 */
    DMA_Cmd(DMA2_Channel5, ENABLE);
    /* ����DMA������ɺ�����ж� */
    //DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);

    /* ADC3 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//����ADCģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 						//��ֹɨ��ģʽ��ɨ��ģʽ���ڶ�ͨ���ɼ�
    //ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//��������ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//��������ת��ģʽ������ͣ�ؽ���ADCת��
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//��ʹ���ⲿ����ת��
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//�ɼ������Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = 4;	 								//Ҫת����ͨ����Ŀ
    ADC_Init(ADC3, &ADC_InitStructure);

    /*����ADC3��ͨ��11Ϊ55.	5���������ڣ�����Ϊ1 */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);	/* ̽����A���� */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 2, ADC_SampleTime_7Cycles5);	/* ̽����B���� */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 3, ADC_SampleTime_7Cycles5);	/* ̽��ǻ�¶� */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 4, ADC_SampleTime_7Cycles5);	/* �����¶� */

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);
    /* Enable ADC3 */
    ADC_Cmd(ADC3, ENABLE);

    /*��λУ׼�Ĵ��� */
    ADC_ResetCalibration(ADC3);
    /*�ȴ�У׼�Ĵ�����λ��� */
    while(ADC_GetResetCalibrationStatus(ADC3));
    /* ADCУ׼ */
    ADC_StartCalibration(ADC3);
    /* �ȴ�У׼���*/
    while(ADC_GetCalibrationStatus(ADC3));

    /* ����û�в����ⲿ����������ʹ���������ADCת�� */
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}
#endif


void ADC1_ValConvt(float *p)
{

    uint16_t i=0;
    uint16_t j=0;
    uint8_t L=0;
    uint8_t R=0;
    uint16_t number=0;
    double VERL=0;
    double VERR=0;


    for(number=0; number<250; number++)
    {
        memcpy(ADC_buffer,I2S_DMA_buffer,2*buffer_Size);//��I2S_DMA_buffer��ָ�ڴ�������2*buffer_Size���ֽڵ�ADC_buffer
        /*��� С��*/
        j=0;
        L=0;
        R=0;
        for(i=2; i<=(uint16_t)buffer_Size; i++)
        {
            if(i%2==0)
            {
                /*��ADC_buffer�����ֽڸ�ֵ��ADC_Convert*/
                ADC_Convert[j]=(uint32_t)ADC_buffer[i-2];
                ADC_Convert[j]=ADC_Convert[j]<<16;
                ADC_Convert[j]|=(uint32_t)ADC_buffer[i-1];
                j++;
            }
        }

        for(i=0; i<(uint16_t)buffer_Size/2; i++)
        {
            /*��ADC_Convert��ֵ��AK5351L_Data*/
            if(i%2==0)
            {
                AK5351L_Data[L]=ADC_Convert[i];
                AK5351L_Data[L]=AK5351L_Data[L]>>12;//12λΪ���룬ɾ��
                L++;
            }
            else
            {
                AK5351R_Data[R]=ADC_Convert[i];
                AK5351R_Data[R]=AK5351R_Data[R]>>12;
                R++;
            }
        }

#if 0
        // ���������ݴ�ӡ
        if(number==1)
            for(i=0; i<(uint16_t)buffer_Size/4; i++)
            {
                printf(" %d  ",i);
                printf(" %d\r\n",AK5351L_Data[i]);
            }
#endif

#if 0

// ���������ݴ�ӡ
        if(number==1)
            for(i=0; i<(uint16_t)buffer_Size/4; i++)
            {
                printf(" %d  ",i);
                printf(" %d\r\n",AK5351R_Data[i]);

            }
#endif


        for(i=0; i<(uint16_t)buffer_Size/4; i++)
        {

            VERL+=AK5351L_Data[i];
            VERR+=AK5351R_Data[i];

        }
    }

    *p= VERL/62500;
    *(p+1)= VERR/62500;

//  printf(" *p=%f    ",*p);
//  printf(" *(p+1)=%f\r\n",*(p+1));

}
void ADC2_ValConvt(uint16_t *p)
{
    uint16_t averagecnt =400;
    uint16_t j;
    uint32_t ADC2_Average = 0;
//	float a;

    for(j = 0; j < averagecnt; j++)
    {
        Delay1us(50);
        while(!(ADC_ConvtComplFlag &= (1<<3)));
        ADC_ConvtComplFlag &= (~(1<<3));
        ADC_SoftwareStartConvCmd(ADC2, ENABLE);
        //ADC2_Config();
        ADC2_Average += ADC_ConvertedValue3;
    }

    ADC2_Average /= averagecnt;
//	a = (float) ADC2_Average/4096*3;	/* ��ȡת����ADֵ */
//	*p = (a - 1) / 4;
    *p=ADC2_Average;

//	printf("2= %d V \r\n",  ADC2_Average);
//	printf("2= %f V \r\n", a);
//	printf("2= %f V \r\n", *p);
//	printf("\r\n");
}
#if 0
void ADC3_ValConvt(float *p)
{
    uint32_t averagecnt = 36000;
    uint8_t i;
    uint32_t j;
    uint32_t ADC3_Average[4] = {0,0,0,0};
    //float a[2];

    //ADC_ConvtComplFlag &= (~(1<<2));
    for(j = 0; j < averagecnt; j++)
    {
        //ADC3_Config();
        //Delay100us(10);
        //while(!(ADC_ConvtComplFlag &= (1<<2)));
        //ADC_ConvtComplFlag &= (~(1<<2));
        for(i = 0; i < 4; i++)
        {
            ADC3_Average[i] += ADC_ConvertedValue2[i];
        }
    }
    for(i = 0; i<2; i++)
    {
        ADC3_Average[i] /= averagecnt;
        *(p+i) =((float)ADC3_Average[i]/4096*3.3-0.675)/0.058; // ��ȡת����ADֵ
        //*(a+i) =(float)ADC3_Average[i]/4096*3.3; // ��ȡת����ADֵ
        //printf("%d  The current AD value = %f V \r\n", i, a[i]);
    }
    for(i = 2; i<4; i++)
    {
        ADC3_Average[i] /= averagecnt;
        *(p+i) =((float)ADC3_Average[i]/4096*3.3)/0.0535; // ��ȡת����ADֵ
        //printf("%d  The current AD value = %d V \r\n", i, ADC_ConvertedValue1[i]);
        //*(a+i) =(float)ADC3_Average[i]/4096*3.3; // ��ȡת����ADֵ
        //printf("%d  The current AD value = %f V \r\n", i, a[i]);
    }
    //ADC_Cmd(ADC3, ENABLE);
    //ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}
#endif

void ADC1_2_IRQHandler(void)
{
    if(ADC_GetITStatus(ADC2, ADC_IT_EOC) == SET)
    {
        ADC_ConvertedValue3 = ADC_GetConversionValue(ADC2);
        ADC_ConvtComplFlag |= (1 << 3);
        ADC_ClearITPendingBit(ADC2, ADC_IT_EOC);
    }
}

/**
  * @brief  ADC1��ʼ��
  * @param  ��
  * @retval ��
  */
void ADC123_Init(void)
{
    ADC_GPIO_Config();
//	I2S_GPIO_Config();
//	I2S_Config();
    ADC2_Config();
//	ADC3_Config();
}
/*********************************************END OF FILE**********************/
