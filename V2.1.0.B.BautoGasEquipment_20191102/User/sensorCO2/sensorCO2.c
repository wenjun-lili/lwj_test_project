/**
  ******************************************************************************
  * @file    通过串口读取翼芯G4 CO2传感器浓度
  * @detail  G4传感器以2Hz频率通过串口发送出浓度值
  * @author  seekya
  * @version V1.0
  * @date    27-June-2019
  ******************************************************************************
  * @attention
  * 是否考虑不做校验？因为校验需要数据处理，特别是对自动采气装置时间要求比较快
  ******************************************************************************
  */
#include "sensorCO2.h"
#include "utility.h"
#include "globalVar.h"
#include "stdio.h"
#include "math.h"
#include "evalve.h"

/**
  *@breif  ASCII值转化为十进制数字
  *@param  bHex:ASCII值
  *@retval bHex:十进制值
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
        bHex=0xFF;//如果接受到其他字符，则FF
    }
    return bHex;
}

#if 1 //若使用USART_DMA方式
/**
  *@breif 串口3配置
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
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//配置空闲中断IDLE
	USART_ClearFlag(USART3,USART_FLAG_TC);
 
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure);
	
	DMA_DeInit(DMA1_Channel3);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;// 初始化外设地址
	DMA_InitStructure.DMA_MemoryBaseAddr =(uint32_t)Com3RBuf;// 内存地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//传输方向
	DMA_InitStructure.DMA_BufferSize = 50 ; //缓冲大小，50字节
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设地址不递增
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设宽度，1字节
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存宽度，1字节
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//单次传输
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;          
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//非内存到内存 
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);                     
	DMA_Cmd(DMA1_Channel3, ENABLE);
	  
}
/**
  *@breif  串口DMA重复赋值使能
  *@param  none
  *@retval none
  */
void USART_DMA_Enable(DMA_Channel_TypeDef* DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, DISABLE ); //关闭USART3 TX DMA1 所指示的通道 
	DMA_SetCurrDataCounter(DMA_CHx,50);//从新设置缓冲大小,指向数组0
	DMA_Cmd(DMA_CHx, ENABLE); //使能USART3 DMA1 所指示的通道 
} 
/**
  *@breif  串口3中断处理程序
  *@detail G4传感器串口数据包格式：数据区+TAB(0x09)+校验区+回车(0x0D)+换行(0x0A)
		       a.一个数据包是全部由ASCII码组成的字符串；
		       b.校验字算法：数据区的所有字节累加求和，假定为S，则校验字Checksum=(unsigned char)-(signed char)S
           c.校验区的内容：是校验字的两位Hex码，例如校验字为0x23，则是字符“23”
  *@prarm  none
  *@retval none
  */
void USART3_IRQHandler(void)                 //串口3中断服务程序
{
	  if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)  //接收中断
    {
        USART_ReceiveData(USART3);//读取数据，清除中断标志位。
			  CO2Con=(Com3RBuf[4]-0x30)*1000+(Com3RBuf[5]-0x30)*100+(Com3RBuf[6]-0x30)*10+(Com3RBuf[7]-0x30);//得到CO2浓度
//			  printf("ndu=%d\r\n",CO2Con);
        USART_ClearITPendingBit(USART3, USART_IT_IDLE);//清除中断标志
        USART_DMA_Enable(DMA1_Channel3);//恢复DMA功能，等待下一次的接收		
    } 
} 

/**
  *@breif  CO2结果计算
  *@param  none
  *@retval none
  */
void CO2Calculation(void)
{
    uint8_t Checksum=0;//校验和
    int8_t sum=0;
	  uint8_t t=0;
	  uint8_t a,b;
	
    for(t=0; t<10; t++) //求数据和
    {
         sum+=(int8_t)Com3RBuf[t];
    }
		  
    Checksum=(uint8_t)(-sum);//得到校验和

    a=CharToDec(Com3RBuf[11]);//将校验和转化为字符
    b=CharToDec(Com3RBuf[12]);
    if((Checksum==(a*16+b))&&(Com3RBuf[1]==0x2E))//因为和以16进制表示，将后面两位转化ASCII码
    {
       if((Com3RBuf[0]==0x20)||(Com3RBuf[2]==0x20)||(Com3RBuf[3]==0x20))//如果前4位存在空格
       {
						CO2Con=0;			 
       }				 
       else
       {
            CO2Con=(uint16_t)(Com3RBuf[0]-0x30)*100+(uint16_t)(Com3RBuf[2]-0x30)*10+(uint16_t)(Com3RBuf[3]-0x30);//计算浓度值								 
        }	
		}	
    else
    {
	      CO2Con=0;
    }
	
}

#else   //不使用DMA模式
/**
  *@breif  USART3配置
  *@detail 波特率9600 数据位：8 校验位：none 停止位：1
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

	  
		/*串口中断配置*/
    NVIC_InitStructure.NVIC_IRQChannel =USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
    /*引脚配置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /*串口参数配置 */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);


    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);	//开启串口空闲中断
    USART_ClearFlag(USART3, USART_FLAG_TC); /* 清除接收中断标志 */		
}

/**
  *@breif  串口3中断处理程序
  *@detail 快速响应传感器每隔50ms发送一次数值，数值格式为“Z XXXXX”，其中XXXXX为CO2浓度*100，如5.2%，则数值为520
  *@prarm  none
  *@retval none
  */
void USART3_IRQHandler(void)
{
    uint8_t Res;
    uint8_t t;
    uint8_t a,b;
    uint8_t Checksum=0;//校验和
    uint16_t sum=0;
	
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
		    USART_ClearITPendingBit(USART3, USART_IT_RXNE);//  清除中断标志位
        Res =USART_ReceiveData(USART3);	//读取接收到的数据
	    
        if(Res!=0x0A)
        {
            Com3RBuf[Com3Index]=Res ;
            Com3Index++;
        }
        else
        {
            Com3RBuf[Com3Index]=Res ;
						Com3Index=0;
						CO2Con=(Com3RBuf[3]-0x30)*1000+(Com3RBuf[4]-0x30)*100+(Com3RBuf[5]-0x30)*10+(Com3RBuf[6]-0x30);//得到CO2浓度
		    }

    }

}

#endif

