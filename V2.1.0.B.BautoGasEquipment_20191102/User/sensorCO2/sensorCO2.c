/**
  ******************************************************************************
  * @file    ͨ�����ڶ�ȡ��оG4 CO2������Ũ��
  * @detail  G4��������2HzƵ��ͨ�����ڷ��ͳ�Ũ��ֵ
  * @author  seekya
  * @version V1.0
  * @date    27-June-2019
  ******************************************************************************
  * @attention
  * �Ƿ��ǲ���У�飿��ΪУ����Ҫ���ݴ����ر��Ƕ��Զ�����װ��ʱ��Ҫ��ȽϿ�
  ******************************************************************************
  */
#include "sensorCO2.h"
#include "utility.h"
#include "globalVar.h"
#include "stdio.h"
#include "math.h"
#include "evalve.h"

/**
  *@breif  ASCIIֵת��Ϊʮ��������
  *@param  bHex:ASCIIֵ
  *@retval bHex:ʮ����ֵ
  */
uint8_t CharToDec(uint8_t bHex)
{
    if((bHex>=48)&&(bHex<=57))
    {
        bHex -=48;
    }
    else if((bHex>=65)&&(bHex<=70))//Capital
    {
        bHex -= 55;
    }
    else
    {
        bHex=0xFF;//������ܵ������ַ�����FF
    }
    return bHex;
}

#if 1 //��ʹ��USART_DMA��ʽ
/**
  *@breif ����3����
	*@param none
	*@retval none
  */
void USART3_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;             
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;	 
	
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;	
	GPIO_Init(GPIOB,&GPIO_InitStructure); 

	USART_InitStructure.USART_BaudRate=9600;
	USART_InitStructure.USART_WordLength=USART_WordLength_8b;
	USART_InitStructure.USART_StopBits=USART_StopBits_1;
	USART_InitStructure.USART_Parity=USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl=USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
	USART_Init(USART3,&USART_InitStructure);
//	USART_Cmd(USART3, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//���ÿ����ж�IDLE
	USART_ClearFlag(USART3,USART_FLAG_TC);
 
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;// ��ʼ�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)Com3RBuf;// �ڴ��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//���䷽��
	DMA_InitStructure.DMA_BufferSize = 50 ; //�����С��50�ֽ�
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// �����ַ������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ����
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�����ȣ�1�ֽ�
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// �ڴ��ȣ�1�ֽ�
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//���δ���
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//���ڴ浽�ڴ� 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);                     
	DMA_Cmd(DMA1_Channel3, ENABLE);
	  
}
/**
  *@breif  ����DMA�ظ���ֵʹ��
  *@param  none
  *@retval none
  */
void USART_DMA_Enable(DMA_Channel_TypeDef* DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE ); //�ر�USART3 TX DMA1 ��ָʾ��ͨ�� 
	DMA_SetCurrDataCounter(DMA_CHx,50);//�������û����С,ָ������0
	DMA_Cmd(DMA_CHx, ENABLE); //ʹ��USART3 DMA1 ��ָʾ��ͨ�� 
} 
/**
  *@breif  ����3�жϴ������
  *@detail G4�������������ݰ���ʽ��������+TAB(0x09)+У����+�س�(0x0D)+����(0x0A)
		       a.һ�����ݰ���ȫ����ASCII����ɵ��ַ�����
		       b.У�����㷨���������������ֽ��ۼ���ͣ��ٶ�ΪS����У����Checksum=(unsigned char)-(signed char)S
           c.У���������ݣ���У���ֵ���λHex�룬����У����Ϊ0x23�������ַ���23��
  *@prarm  none
  *@retval none
  */
void USART3_IRQHandler(void)                 //����3�жϷ������
{
	  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //�����ж�
    {
        USART_ReceiveData(USART3);//��ȡ���ݣ�����жϱ�־λ��
			  CO2Con=(Com3RBuf[4]-0x30)*1000+(Com3RBuf[5]-0x30)*100+(Com3RBuf[6]-0x30)*10+(Com3RBuf[7]-0x30);//�õ�CO2Ũ��
//			  printf("ndu=%d\r\n",CO2Con);
        USART_ClearITPendingBit(USART3, USART_IT_IDLE);//����жϱ�־
        USART_DMA_Enable(DMA1_Channel3);//�ָ�DMA���ܣ��ȴ���һ�εĽ���		
    } 
} 

/**
  *@breif  CO2�������
  *@param  none
  *@retval none
  */
void CO2Calculation(void)
{
    uint8_t Checksum=0;//У���
    int8_t sum=0;
	  uint8_t t=0;
	  uint8_t a,b;
	
    for(t=0; t<10; t++) //�����ݺ�
    {
         sum+=(int8_t)Com3RBuf[t];
    }
		  
    Checksum=(uint8_t)(-sum);//�õ�У���

    a=CharToDec(Com3RBuf[11]);//��У���ת��Ϊ�ַ�
    b=CharToDec(Com3RBuf[12]);
    if((Checksum==(a*16+b))&&(Com3RBuf[1]==0x2E))//��Ϊ����16���Ʊ�ʾ����������λת��ASCII��
    {
       if((Com3RBuf[0]==0x20)||(Com3RBuf[2]==0x20)||(Com3RBuf[3]==0x20))//���ǰ4λ���ڿո�
       {
						CO2Con=0;			 
       }				 
       else
       {
            CO2Con=(uint16_t)(Com3RBuf[0]-0x30)*100+(uint16_t)(Com3RBuf[2]-0x30)*10+(uint16_t)(Com3RBuf[3]-0x30);//����Ũ��ֵ								 
        }	
		}	
    else
    {
	      CO2Con=0;
    }
	
}

#else   //��ʹ��DMAģʽ
/**
  *@breif  USART3����
  *@detail ������9600 ����λ��8 У��λ��none ֹͣλ��1
  *@param  none
  *@retval none
  */
void USART3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	  
		/*�����ж�����*/
    NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    /*�������� */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*���ڲ������� */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);


    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	//�������ڿ����ж�
    USART_ClearFlag(USART3, USART_FLAG_TC); /* ��������жϱ�־ */		
}

/**
  *@breif  ����3�жϴ������
  *@detail ������Ӧ������ÿ��50ms����һ����ֵ����ֵ��ʽΪ��Z XXXXX��������XXXXXΪCO2Ũ��*100����5.2%������ֵΪ520
  *@prarm  none
  *@retval none
  */
void USART3_IRQHandler(void)
{
    uint8_t Res;
    uint8_t t;
    uint8_t a,b;
    uint8_t Checksum=0;//У���
    uint16_t sum=0;
	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
		    USART_ClearITPendingBit(USART3, USART_IT_RXNE);//  ����жϱ�־λ
        Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
	    
        if(Res!=0x0A)
        {
            Com3RBuf[Com3Index]=Res ;
            Com3Index++;
        }
        else
        {
            Com3RBuf[Com3Index]=Res ;
						Com3Index=0;
						CO2Con=(Com3RBuf[3]-0x30)*1000+(Com3RBuf[4]-0x30)*100+(Com3RBuf[5]-0x30)*10+(Com3RBuf[6]-0x30);//�õ�CO2Ũ��
		    }

    }

}

#endif

