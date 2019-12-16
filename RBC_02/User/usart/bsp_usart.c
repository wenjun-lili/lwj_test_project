/**
  ******************************************************************************
  * @file    bsp_usart.c
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �����õ�printf���ڣ��ض���printf������
  ******************************************************************************
  * @attention

  ******************************************************************************
  */


#include "./usart/bsp_usart.h"
#include "globalval.h"
#include <string.h>
#include "DWIN.h"

/*  ����USART1�����ж� */
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
 * @brief  USART GPIO ����,������������
 * @param  ��
 * @retval ��
 */



/**
 * @brief  USART1 GPIO ??,???????9600 8-N-1
 * @param  ?
 * @retval ?
 */
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

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

    USART1_NVIC_Config();
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);
    USART_ClearFlag(USART1, USART_FLAG_TC);

}

#if 1
int fputc(int ch, FILE *f)
{
    USART_SendData(USART1, (uint8_t) ch);

    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return (ch);
}

int fgetc(FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

    return (int)USART_ReceiveData(USART1);
}
#endif

#if 1

/**
  *@breif  ����1�жϴ������
  *@param  none
  *@retval none
  *@note   20190610�� lcy
*/
void USART1_IRQHandler(void)
{
    static uint8_t bytecount = 0;
    uint16_t ch;
    uint8_t temp[2]= {0x00,0x01};

    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);//  ����жϱ�־λ

        /* ����6���ֽ� */
        ch = USART_ReceiveData(USART1);

        RBC_pc_ComR[bytecount]=(uint8_t)ch;

        bytecount++;

        /* ����λ�����յ�0xBB��0x00��0xFF��0xEE��0x99��˵���ǵ��ֽ�ָ��*/
        if((RBC_pc_ComR[0]==0xBB)||(RBC_pc_ComR[0]==0x00) ||(RBC_pc_ComR[0]==0xFF)||(RBC_pc_ComR[0]==0xEE)||(RBC_pc_ComR[0]==0x99))
        {

            if(bytecount==1)
            {
                bytecount = 0;
                ReceiveFlag=1; // ��ʾ�������
            }
        }

        /*����λ�����յ���0xDD��˵����λ����������*/
        if(RBC_pc_ComR[0]==0xDD)
        {
            if(bytecount==1)
            {
                bytecount = 0;
								F_Timeout=0;//��ʱ��־��ʼ��
                USART_SendData(USART1, (uint8_t)0xCC);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
                F_PC=1;
                Wr_82_Scr(0x0070,temp,2);//����������ͼ������
                ReceiveFlag1=1;
            }
        }

        /*3.�����λ�����յ���0x20֡ͷ������0xE0*/
        else if((RBC_pc_ComR[0]==0x20)||(RBC_pc_ComR[0]==0xE0))
        {
            if(bytecount == 6)
            {
                bytecount = 0;
                ReceiveFlag6=1; // ��ʾ�������
            }
        }
        else if(RBC_pc_ComR[0]==0x5A)
        {
            if(bytecount==RBC_pc_ComR[2]+3)
            {
                bytecount = 0;

                RevSDFlag=1; // ��ʾ���յ�����λ����SD����Ҫ�洢����Ϣ
            }
        }

        else
        {
            bytecount = 0;// ������ܵ��������ֽڲ�����Ҫ�����½���
        }

    }

}

#endif
/**
  * @brief  USART1_Timeout_UserCallback(void)
	* @datail usart1�ȴ�ʱ�䳬ʱ������
	* @param  none
	* @return none
  */
void  USART1_Timeout_UserCallback(void)
{
    uint8_t temp[2]= {0x00,0x00};
    F_PC=0;
    Wr_82_Scr(0x0070,temp,2);
}


/********************************END OF FILE************************/

