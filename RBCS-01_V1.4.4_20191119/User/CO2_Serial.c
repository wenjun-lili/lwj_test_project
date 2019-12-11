/*
�����ļ�
���ݣ�����CO2�������������ڷ������ݽ��ж�����̼Ũ�Ȼ�ȡ
ʱ�䣺20180607
*/
#include "CO2_Serial.h"
#include "utility.h"
#include "globalval.h"
#include "stdio.h"
#include "math.h"

/*
����USART3�����ж�
*/
static void USART3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
����USART3
�����ʣ�1200
����λ��8
У��λ����
ֹͣλ��1
*/
void USART3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);



    /* USART3 GPIO config */
    /* Configure USART3 Tx (PB10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    /* Configure USART3 Rx (PB11) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//	/* USART3 mode config */
    USART_InitStructure.USART_BaudRate = 38400;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);


    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//TC��������жϡ�RXNE�����жϡ�PE��ż�����жϣ������Ƕ��


//	USART_Cmd(USART3, ENABLE);


    USART_ClearFlag(USART3, USART_FLAG_TC); /* Transmission Complete flag */

    /* ����USART3�����ж� */
    USART3_NVIC_Config();

}

/**function: CharToDec()
*** ��ascii����ֵת��Ϊʮ���ƣ���ascii��Ϊ1��ת���Ϊ1��ascii��ΪA��Ϊ10
*** input:ACSII
***Return :Hex
**/
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
void USART3_IRQHandler(void)
{
    uint8_t Res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
    {
        Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
        if(Res!=0x0A)
        {
            Com3RBuf[Com3Index]=Res ;
            Com3Index++;
        }
        else
        {
            Rec3FinishF=1;	//���������
            Com3RBuf[Com3Index]=Res ;
            Index3End=Com3Index;
            Com3Index=0;
//			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//�ر��ж�
            USART_Cmd(USART3,DISABLE);//�رմ���ʹ��
        }
    }
}
