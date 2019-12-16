#include "DWIN.h"
#include "utility.h"
#include "globalval.h"
#include "stdio.h"
#include "bsp_i2c_eeprom.h"
/*
�����ļ����Ǵ����������ж�����lcy 20180317
1�����RBCS-01�޸ĵ�����
����DGUS���ڹ���
2�����C13�޸ĵ�����
ͬ������USART3���������õ�������PB10��PB11
*/
/* ����USART2�����ж� */
static void USART2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Enable the USARTy Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel =USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

}

/**
 * @brief  USART2 GPIO ����,����ģʽ���� 115200 8-N-1
 * @param  ��
 * @retval ��
 */
void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    /* Configure USART2 Tx (PA2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART2 Rx (PA3) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* USART2 mode config */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);


    USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
    /* ʹ�ܴ���2�����ж� */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);

    USART_ClearFlag(USART2, USART_FLAG_TC); /* Transmission Complete flag */

    /* ����USART2�����ж� */
    USART2_NVIC_Config();

}

/**
  *@breif  DWIM��ָ���ʶ������ʼ��
	*@param  none
	*@retval none
  */
void Dwins_Variable(void)
{
    Com3Index=0;//����2����������������
    Com3TBuf[0]=0x4A;
    Com3TBuf[1]=0xA4;
    Delay100ms(10);
}

/**
  *@breif  ���ò���ֵ�û�ȡ�ͷ���
	*@detail ���ȴ�eeprom�ж�ȡ�������ã����а汾������Ϊȫ�ֱ���������Ҫÿ�ζ�ȡ����ÿ�γ�һ���汾ʱ��ֻ�轫�ð汾��ȫ�ֱ����޸ļ��ɡ�
	*@param  none
	*@retval none
*/
void DGUS_Set_para(void)
{
    uint8_t temp[2];
    ee_ReadBytes((uint8_t *)&Eeprom_Init,12,2);
    //��ȡeeprom�Ƿ�д������
    if(Eeprom_Init!=0xABCD)
    {
        Eeprom_Init=0xABCD;
        ee_WriteBytes((uint8_t *)&CO_ZERO, 0, 2);//д��CO_ZEROֵ
        ee_WriteBytes((uint8_t *)&CO2_PB, 2, 2);//д��CO2_PBֵ
        ee_WriteBytes((uint8_t *)&R_XB, 4, 2);//д��R_XBֵ
        ee_WriteBytes((uint8_t *)&CO2_Taijie, 6, 2);//д��CO2_Taijieֵ
        ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);//д��CO2_ZEROֵ
        ee_WriteBytes((uint8_t *)&Ring_Num,10,2);//���ܷ�Ȧ��ʼ��д��
        ee_WriteBytes((uint8_t *)&Eeprom_Init,12,2);//eeprom�Ƿ�д���־д��
        ee_WriteBytes((uint8_t *)&ATM_Coeff,14,2);//��ȡ��ѹУ��ϵ��
    }

    ee_ReadBytes((uint8_t *)&CO_ZERO, 0, 2);//��ȡCO_ZEROֵ
    ee_ReadBytes((uint8_t *)&CO2_PB, 2, 2);//��ȡCO2_PBֵ
    ee_ReadBytes((uint8_t *)&R_XB, 4, 2);//��ȡR_XBֵ
    ee_ReadBytes((uint8_t *)&CO2_Taijie, 6, 2);//��ȡCO2_Taijieֵ
    ee_ReadBytes((uint8_t *)&CO2_ZERO, 8, 2);//��ȡCO2_ZEROֵ
    ee_ReadBytes((uint8_t *)&Ring_Num,10,2);//��ȡ�ܷ�Ȧ��ʹ�ô���
    ee_ReadBytes((uint8_t *)&ATM_Coeff,14,2);//��ȡ��ѹУ��ϵ��

    //COϵ�� R_XB
    Delay100ms(10);
    temp[0]= R_XB>>8;
    temp[1]= R_XB;
    Wr_82_Scr(0x0005,temp,2);

    //CO2ϵ�� CO2_Taijie
    temp[0]= CO2_Taijie>>8;
    temp[1]= CO2_Taijie;
    Wr_82_Scr(0x0006,temp,2);

    //CO2��� CO2_ZERO
    temp[0]= CO2_ZERO>>8;
    temp[1]= CO2_ZERO;
    Wr_82_Scr(0x0007,temp,2);

    //CO�����ֵ CO_ZERO
    temp[0]= CO_ZERO>>8;
    temp[1]= CO_ZERO;
    Wr_82_Scr(0x0004,temp,2);

    //CO2Ũ����ֵ CO2_PB
    temp[0]= CO2_PB>>8;
    temp[1]= CO2_PB;
    Wr_82_Scr(0x0003,temp,2);

    //��Ȧʹ�ô���
    temp[0]= Ring_Num>>8;
    temp[1]= Ring_Num;
    Wr_82_Scr(0x0009,temp,2);

    //�汾��
    temp[0]= Version>>8;
    temp[1]= Version;
    Wr_82_Scr(0x001D,temp,2);

    //��ѹУ׼ϵ��
    temp[0]= ATM_Coeff>>8;
    temp[1]= ATM_Coeff;
    Wr_82_Scr(0x0030,temp,2);
}

//-------------����2-DGUSͨѶ-----------------

void Rs0_Out(uint8_t Cmd)
{
    Com3TBuf[3] = Cmd;								// ����:0x80-0x84
    Index3End = Com3TBuf[2] + 3;      //����3����ָ����ֽڸ�����3Ϊ5A A5���ֽڸ������ֽ���
    Com3Index=0;		                  //���͵�����

    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

    USART_SendData(USART2, (uint8_t)Com3TBuf[Com3Index++]);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

    while((--Com3Index) != 0);			//�ȴ��������
    // �������Com3Index��0����Ϊ�жϷ��ص��˺���USART_SendData(USART3, (uint8_t)Com3TBuf[Com3Index++]);
}

//------------------����2����-------------------
// �򴮿�0����һ�����ݴ�d�ǵ�ַ��s�����ݵ�ַƫ����
void send_Ti_scr(uint16_t d,uint8_t *p,uint8_t s)
{
    Com3TBuf[2] = 5;								//  ����=8-3
    Com3TBuf[4] = d >> 8;						//  ��ַ����8λ
    Com3TBuf[5] = d;								//  ��ַ����8λ
    Com3TBuf[6] = *(p + s);
    Com3TBuf[7] = *(p + s + 1);

    Rs0_Out(0x82);									// д���ݵ�DGUS�洢������
}


//***********����һ�����ݵ�DGUS�洢��:d�Ǵ洢����ַ��p�����ݵ�ַָ�룻s:�ֽ����� **************
//82ָ�� 4A A4 ���ݳ��� 82 ������ʼ��ַ ���� ��������ʼ��ַΪ2���ֽڣ�
void Wr_82_Scr(uint16_t d,uint8_t *p,uint8_t s)
{
    uint8_t i;
    Com3TBuf[2] = s + 3;							// ����+H��L��ַ+�����ֽ�����
    Com3TBuf[4] = d >> 8;							// ��ַ����8λ
    Com3TBuf[5] = d;							  	// ��ַ����8λ
    for(i=0; i<s; i++)
    {
        Com3TBuf[i+6] = *(p + i);					//  ��Ҫд����������鰴�ֽ�װ��Com3Tbuf[6]��ʼ
    }
    Rs0_Out(0x82);									//  0x82д���ݵ�DGUS�����洢����
}
//д���ƼĴ���
//80ָ�� 4A A4 ���ݳ��� 80 �Ĵ��� ���� ���Ĵ���Ϊ1���ֽڣ�
void Wr_80_Scr(uint8_t d,uint8_t *p,uint8_t s)
{
    uint8_t i;
    Com3TBuf[2] = s + 2;
    Com3TBuf[4] = d;

    for(i=0; i<s; i++)
    {
        Com3TBuf[i+5] = *(p + i);
    }
    Rs0_Out(0x80);
}

// �����ƼĴ���ָ��  s�������ٸ��ֽ�
//80ָ�� 4A A4���ݳ��� 81 �Ĵ��� �ֽڳ���
void Rd_81_Scr(uint8_t d,uint8_t s)
{
    Com3TBuf[2] = 3;
    Com3TBuf[4] = d;
    Com3TBuf[5] = s;	//ͼƬ�������ֽ�
    Rs0_Out(0x81);
}



//
void Wr_84_Scr(uint8_t d,uint16_t s)
{
    Com3TBuf[2] = 0x04;								// 5���ֽ�����
    Com3TBuf[4] = d;  // ͨ����
    Com3TBuf[5] = s >> 8;						// ���ݸ�8λ
    Com3TBuf[6] = s;								// ���ݵ�8λ
    Rs0_Out(0x84);									// 0x82д���ݵ�DGUS�����洢��
}


// 0x83�����ݴ洢��ָ�� d:��ַ  s:�ֳ��� ��ʾ�ӱ�����ַ����s���ֳ���
void Rd_83_Scr(uint16_t d,uint8_t s)
{
    Com3TBuf[2] = 4;								//  4�����ݳ���
    Com3TBuf[4] = d >> 8;						//  ��λ��ַ
    Com3TBuf[5] = d;								//  ��λ��ַ
    Com3TBuf[6] = s;								//  �ֳ���

    Rs0_Out(0x83);									// ��ȡDGUS�洢������
}

//����2����һ�����ݴ� d;�洢����ַ��m:��ȡģʽ
//z���ֿ��ַ����Եģ���g�����ݸ���
void CQ_To_screen(uint8_t m,uint8_t z,uint16_t d,uint16_t g)
{
    Com3TBuf[2] = 12;								// �ֽ�����=15-3
    Com3TBuf[4] = 0x56;             // 56�Ĵ���

    Com3TBuf[5] = 0x5A;								// [0x40]=0x5A ������ֿ�洢������
    Com3TBuf[6] = m;								// [0x41]=m=0x50:д;m=0xA0:��
    /*
    Com0TBuf[7] = z;
    Com0TBuf[8] = 0;
    Com0TBuf[9] = 0;
    Com0TBuf[10] = 0;
    */
    Com3TBuf[7] = 0;								// �ֿ�ռ�:0x40-0x7F:ÿ���ؼ�128KW
    Com3TBuf[8] = 0xac+z;								// 3:�ֿ��׵�ַ:0-0x01 FF FF
    Com3TBuf[9] = 0;
    Com3TBuf[10] = 0;

    Com3TBuf[11] = d >> 8;							// 2:�����洢���׵�ַ:0x0000-0x6FFF
    Com3TBuf[12] = d;
    Com3TBuf[13] = g >> 8;							// 2:����:0-0x6F FF
    Com3TBuf[14] = g;

    Rs0_Out(0x80);
    Delay100ms(5);
}

//----������Ϣ--------------
// ��������BCDtoDEC
//����������BCDת����ʮ����
//��ڲ�����temp:ת����BCD��
// ���أ�ת�����ʮ������
uint8_t BCDtoDec(uint8_t temp)
{
    return ((temp/16)*10+temp%16);
}

/* USART2�жϴ������� */
void USART2_IRQHandler(void)
{
    uint8_t  ch;

    // ��������
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);// ����жϱ�־λ

        ch = USART_ReceiveData(USART2);
        Com3RBuf[Com3Index]=(uint8_t)ch;

        Com3Index++;

        if(Com3Index==(Com3RBuf[2]+3))
        {
            Rec3FinishF=1;	// ������ɱ�־��1
            Com3Index=0;
        }

    }

    // ��������
    else if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) // �жϷ����ж��Ƿ���Է���
    {
        USART_ClearITPendingBit(USART2, USART_IT_TXE);// ����жϱ�־λ

        if(Com3Index<Index3End)
        {
            USART_SendData(USART2, (uint8_t)Com3TBuf[Com3Index++]);
            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
        }
        else  // �ѷ����������
        {
            Com3Index=0;
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

        } // ��������

    }
}



