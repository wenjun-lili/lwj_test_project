/*
新增文件
内容：利用CO2传感器自身串口发送数据进行二氧化碳浓度获取
时间：20180607
*/
#include "CO2_Serial.h"
#include "utility.h"
#include "globalval.h"
#include "stdio.h"
#include "math.h"

/*
配置USART3接收中断
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
配置USART3
波特率：1200
数据位：8
校验位：无
停止位：1
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


    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	//TC传输完成中断、RXNE接收中断、PE奇偶错误中断，可以是多个


//	USART_Cmd(USART3, ENABLE);


    USART_ClearFlag(USART3, USART_FLAG_TC); /* Transmission Complete flag */

    /* 配置USART3接收中断 */
    USART3_NVIC_Config();

}

/**function: CharToDec()
*** 将ascii码数值转化为十进制，即ascii码为1则转后后为1，ascii码为A则为10
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
        bHex=0xFF;//如果接受到其他字符，则FF
    }
    return bHex;
}
void USART3_IRQHandler(void)
{
    uint8_t Res;
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res =USART_ReceiveData(USART3);	//读取接收到的数据
        if(Res!=0x0A)
        {
            Com3RBuf[Com3Index]=Res ;
            Com3Index++;
        }
        else
        {
            Rec3FinishF=1;	//接收完成了
            Com3RBuf[Com3Index]=Res ;
            Index3End=Com3Index;
            Com3Index=0;
//			USART_ITConfig(USART3, USART_IT_RXNE, DISABLE);//关闭中断
            USART_Cmd(USART3,DISABLE);//关闭串口使能
        }
    }
}

