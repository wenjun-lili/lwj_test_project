/*
ADC以及DMA配置
*/
#include "adc.h"
#include "globalval.h"
#include "math.h"
#include "bsp.h"

#define ADC3_DR_Address    ((u32)0x40013C00+0x4c)
#define SPI_MASTER_DR_Base      0x4000380C
#define buffer_Size  1000

//__IO uint16_t ADC_ConvertedValue[7];			/* 系统所有模拟量 */

__IO uint16_t ADC_ConvertedValue2[4];			/* 所有温度信号 */
__IO uint16_t ADC_ConvertedValue3;				/* 压力量 */
uint16_t  I2S_DMA_buffer[buffer_Size];
uint16_t  ADC_buffer[buffer_Size];
uint32_t  ADC_Convert[buffer_Size/2]; //ADC转换数据

int32_t  AK5351L_Data[buffer_Size/4];//用于存储左声道数据
int32_t  AK5351R_Data[buffer_Size/4];//用于存储右声道数据

/**
  * @brief  使能ADC和DMA的时钟，初始化PA1, PC1~5
  * @param  无
  * @retval 无
  */
static void ADC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    /*ADC时钟最高只能是14M，配置ADC时钟，为PCLK2的8分频，即9MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    /* Enable DMA clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, ENABLE);
    /* Enable ADC1, GPIOA and GPIOC clock */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_ADC2 | RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOA, ENABLE);

    /* Configure PC1~5  as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);				//输入时不用设置速率


    /* Configure PA1 as analog input */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);				//输入时不用设置速率
}


static void ADC1_2_NVIC_Config(void)//中断配置
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
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;// 独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;// 禁止扫描模式，扫描模式用于多通道采集
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//禁止开启连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//不使用外部触发模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1;// 要转换的通道数目1
    ADC_Init(ADC2, &ADC_InitStructure);
    /* ADC2 regular channels configuration */
    /*配置ADC2的通道1为7.5个采样周期，序列为1*/
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
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;	 			//ADC地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&ADC_ConvertedValue2;	//内存地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 4;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;	//外设地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  				//内存地址不固定
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;	//半字
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;										//循环传输
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;										//循环传输
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA2_Channel5, &DMA_InitStructure);

    //DMA2_Channel4_5_NVIC_Config();
    /* Enable DMA2 channel5 */
    DMA_Cmd(DMA2_Channel5, ENABLE);
    /* 配置DMA发送完成后产生中断 */
    //DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);

    /* ADC3 configuration */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;						//独立ADC模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE ; 	 						//禁止扫描模式，扫描模式用于多通道采集
    //ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;						//开启连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;						//开启连续转换模式，即不停地进行ADC转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;		//不使用外部触发转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;					//采集数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 4;	 								//要转换的通道数目
    ADC_Init(ADC3, &ADC_InitStructure);

    /*配置ADC3的通道11为55.	5个采样周期，序列为1 */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_10, 1, ADC_SampleTime_7Cycles5);	/* 探测器A结温 */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 2, ADC_SampleTime_7Cycles5);	/* 探测器B结温 */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_12, 3, ADC_SampleTime_7Cycles5);	/* 探测腔温度 */
    ADC_RegularChannelConfig(ADC3, ADC_Channel_13, 4, ADC_SampleTime_7Cycles5);	/* 气温温度 */

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADC3, ENABLE);
    /* Enable ADC3 */
    ADC_Cmd(ADC3, ENABLE);

    /*复位校准寄存器 */
    ADC_ResetCalibration(ADC3);
    /*等待校准寄存器复位完成 */
    while(ADC_GetResetCalibrationStatus(ADC3));
    /* ADC校准 */
    ADC_StartCalibration(ADC3);
    /* 等待校准完成*/
    while(ADC_GetCalibrationStatus(ADC3));

    /* 由于没有采用外部触发，所以使用软件触发ADC转换 */
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
        memcpy(ADC_buffer,I2S_DMA_buffer,2*buffer_Size);//将I2S_DMA_buffer所指内存区域复制2*buffer_Size个字节到ADC_buffer
        /*清除 小标*/
        j=0;
        L=0;
        R=0;
        for(i=2; i<=(uint16_t)buffer_Size; i++)
        {
            if(i%2==0)
            {
                /*将ADC_buffer两个字节赋值给ADC_Convert*/
                ADC_Convert[j]=(uint32_t)ADC_buffer[i-2];
                ADC_Convert[j]=ADC_Convert[j]<<16;
                ADC_Convert[j]|=(uint32_t)ADC_buffer[i-1];
                j++;
            }
        }

        for(i=0; i<(uint16_t)buffer_Size/2; i++)
        {
            /*将ADC_Convert赋值给AK5351L_Data*/
            if(i%2==0)
            {
                AK5351L_Data[L]=ADC_Convert[i];
                AK5351L_Data[L]=AK5351L_Data[L]>>12;//12位为乱码，删除
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
        // 左声道数据打印
        if(number==1)
            for(i=0; i<(uint16_t)buffer_Size/4; i++)
            {
                printf(" %d  ",i);
                printf(" %d\r\n",AK5351L_Data[i]);
            }
#endif

#if 0

// 右声道数据打印
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
//	a = (float) ADC2_Average/4096*3;	/* 读取转换的AD值 */
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
        *(p+i) =((float)ADC3_Average[i]/4096*3.3-0.675)/0.058; // 读取转换的AD值
        //*(a+i) =(float)ADC3_Average[i]/4096*3.3; // 读取转换的AD值
        //printf("%d  The current AD value = %f V \r\n", i, a[i]);
    }
    for(i = 2; i<4; i++)
    {
        ADC3_Average[i] /= averagecnt;
        *(p+i) =((float)ADC3_Average[i]/4096*3.3)/0.0535; // 读取转换的AD值
        //printf("%d  The current AD value = %d V \r\n", i, ADC_ConvertedValue1[i]);
        //*(a+i) =(float)ADC3_Average[i]/4096*3.3; // 读取转换的AD值
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
  * @brief  ADC1初始化
  * @param  无
  * @retval 无
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
