/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   调试用的printf串口，重定向printf到串口
  ******************************************************************************
  * @attention
 
  ******************************************************************************
  */ 


#include "./usart/bsp_usart.h"
#include "globalval.h"
#include <string.h>
/**
  * @brief  USART1_NVIC_Config(void)
	* @datail 配置USART1接收中断
	* @param  none
	* @return none
  */
static void USART1_NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}



 /**
  * @brief  USART GPIO 配置,工作参数配置
  * @param  none
  * @retval none
  */
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	/* ??GPIOA?USART1??? */
	/* ???????GPIOA?PA9?PA10?????USART1???,??????????,??????????USART1 */
	/* ??GPIO??EXTI???? ???????????,????AFIO??;
	   ?????????????,?????AFIO??,???????????,?USART1 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	
	/* USART1 GPIO config */
	/* Configure USART1 Tx (PA.09) as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);    
	/* Configure USART1 Rx (PA.10) as input floating */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/* USART1 mode config */
	USART_InitStructure.USART_BaudRate =9600;//115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	#if 0
	USART_Cmd(USART1, ENABLE);
    #elif 1
		USART_Cmd(USART1, ENABLE);	
	  USART1_NVIC_Config();
	/* ????1???? */
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
	/* ??????,????????,???????????????????*/
  USART_ClearFlag(USART1, USART_FLAG_TC);
	/* ??USART1???? */ 

    #endif
}

#if 1
// ???c???printf?USART1
int fputc(int ch, FILE *f)
{
		/* ?????????USART1 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* ?????? */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);		
	
		return (ch);
}

// ???c???scanf?USART1
int fgetc(FILE *f)
{
		/* ????1???? */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
#endif

 



#if 1

/**
  * @brief  USART1_IRQHandler(void)
	* @datail USART1 中断处理程序
	* @param  none
	* @return none
  */ 
void USART1_IRQHandler(void)
{
	 static uint8_t bytecount = 0;
	 uint16_t ch;
	
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
		 USART_ClearITPendingBit(USART1, USART_IT_RXNE);//  清除中断标志位
		
		/* 1.  接收6个字节 */
		 ch = USART_ReceiveData(USART1);
		
     RBC_pc_ComR[bytecount]=(uint8_t)ch;
//  printf( "bytecount=%d, RBC_pc_ComR[%d]=0x%0x\r\n", bytecount,bytecount, RBC_pc_ComR[bytecount] );    /*将接受到的数据直接返回打印 */
 		  
// 		 USART_SendData(USART1,(uint8_t)ch); 
//     while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  
	   bytecount++;
		
		/* 2. 若下位机接收的 0XBB 或 0x00 或者 0XFF，说明是单字节指令*/
     if((RBC_pc_ComR[0]==0xBB)||(RBC_pc_ComR[0]==0x00) ||(RBC_pc_ComR[0]==0xFF)||(RBC_pc_ComR[0]==0xEE)||(RBC_pc_ComR[0]==0x99))
		 { 		 
			 if(bytecount==1)
			 {
				 bytecount = 0;
				 ReceiveFlag=1; // 表示接受完毕
			 }
		 }
		 if(RBC_pc_ComR[0]==0xDD)//20180704
		 {
			 if(bytecount==1)
			 {
				 bytecount = 0;
				 USART_SendData(USART1, (uint8_t)0xCC);
				 while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
				 F_PC=1;
				 F_Timeout=0;
				 GPIO_SetBits( GPIOE,  GPIO_Pin_6);//ONLINE 指示灯亮
				 ReceiveFlag1=1;
			 }
		 }
		 // 3.如果下位机接收的是0x20帧头
		 else if((RBC_pc_ComR[0]==0x20)||(RBC_pc_ComR[0]==0xE0))
		 {
				if(bytecount == 6)
			 {
				   bytecount = 0;
				   ReceiveFlag6=1; // 表示接受完毕
			 } 			 
		 }
		 else if(RBC_pc_ComR[0]==0x5A)
		 {
			 if(bytecount==RBC_pc_ComR[2]+3)
			 {
				   bytecount = 0;
				  
				   RevSDFlag=1; // 表示接收到了上位机给SD卡需要存储的信息
			 }
		 }
		 
		 else
		 {
			   bytecount = 0;// 如果接受的数据首字节不满足要求，重新接受
		 }
		 
	} 

}

#endif
/**
  * @brief  USART1_Timeout_UserCallback(void)
	* @datail usart1等待时间超时处理函数
	* @param  none
	* @return none
  */ 
void  USART1_Timeout_UserCallback(void)
{
   F_PC=0;
   GPIO_ResetBits( GPIOE,  GPIO_Pin_6);//ONLINE 指示灯灭 
}

/********************************END OF FILE************************/

