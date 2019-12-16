#include "DWIN.h"
#include "utility.h"
#include "globalval.h"
#include "stdio.h"
#include "bsp_i2c_eeprom.h"
/*
新增文件，是触摸屏串口中断配置lcy 20180317
1、相对RBCS-01修改的内容
新增DGUS串口功能
2、相对C13修改的内容
同样利用USART3，但是利用的是引脚PB10和PB11
*/
/* 配置USART2接收中断 */
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
 * @brief  USART2 GPIO 配置,工作模式配置 115200 8-N-1
 * @param  无
 * @retval 无
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
    /* 使能串口2接收中断 */
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);

    USART_Cmd(USART2, ENABLE);

    USART_ClearFlag(USART2, USART_FLAG_TC); /* Transmission Complete flag */

    /* 配置USART2接收中断 */
    USART2_NVIC_Config();

}

/**
  *@breif  DWIM屏指令标识变量初始化
	*@param  none
	*@retval none
  */
void Dwins_Variable(void)
{
    Com3Index=0;//串口2数据数组索引变量
    Com3TBuf[0]=0x4A;
    Com3TBuf[1]=0xA4;
    Delay100ms(10);
}

/**
  *@breif  设置参数值得获取和发送
	*@detail 首先从eeprom中读取，再设置，其中版本号设置为全局变量，不需要每次读取。在每形成一个版本时，只需将该版本中全局变量修改即可。
	*@param  none
	*@retval none
*/
void DGUS_Set_para(void)
{
    uint8_t temp[2];
    ee_ReadBytes((uint8_t *)&Eeprom_Init,12,2);
    //读取eeprom是否写入数据
    if(Eeprom_Init!=0xABCD)
    {
        Eeprom_Init=0xABCD;
        ee_WriteBytes((uint8_t *)&CO_ZERO, 0, 2);//写入CO_ZERO值
        ee_WriteBytes((uint8_t *)&CO2_PB, 2, 2);//写入CO2_PB值
        ee_WriteBytes((uint8_t *)&R_XB, 4, 2);//写入R_XB值
        ee_WriteBytes((uint8_t *)&CO2_Taijie, 6, 2);//写入CO2_Taijie值
        ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);//写入CO2_ZERO值
        ee_WriteBytes((uint8_t *)&Ring_Num,10,2);//将密封圈初始化写入
        ee_WriteBytes((uint8_t *)&Eeprom_Init,12,2);//eeprom是否写入标志写入
        ee_WriteBytes((uint8_t *)&ATM_Coeff,14,2);//读取气压校正系数
    }

    ee_ReadBytes((uint8_t *)&CO_ZERO, 0, 2);//读取CO_ZERO值
    ee_ReadBytes((uint8_t *)&CO2_PB, 2, 2);//读取CO2_PB值
    ee_ReadBytes((uint8_t *)&R_XB, 4, 2);//读取R_XB值
    ee_ReadBytes((uint8_t *)&CO2_Taijie, 6, 2);//读取CO2_Taijie值
    ee_ReadBytes((uint8_t *)&CO2_ZERO, 8, 2);//读取CO2_ZERO值
    ee_ReadBytes((uint8_t *)&Ring_Num,10,2);//读取密封圈已使用次数
    ee_ReadBytes((uint8_t *)&ATM_Coeff,14,2);//读取气压校正系数

    //CO系数 R_XB
    Delay100ms(10);
    temp[0]= R_XB>>8;
    temp[1]= R_XB;
    Wr_82_Scr(0x0005,temp,2);

    //CO2系数 CO2_Taijie
    temp[0]= CO2_Taijie>>8;
    temp[1]= CO2_Taijie;
    Wr_82_Scr(0x0006,temp,2);

    //CO2零点 CO2_ZERO
    temp[0]= CO2_ZERO>>8;
    temp[1]= CO2_ZERO;
    Wr_82_Scr(0x0007,temp,2);

    //CO零点阈值 CO_ZERO
    temp[0]= CO_ZERO>>8;
    temp[1]= CO_ZERO;
    Wr_82_Scr(0x0004,temp,2);

    //CO2浓度阈值 CO2_PB
    temp[0]= CO2_PB>>8;
    temp[1]= CO2_PB;
    Wr_82_Scr(0x0003,temp,2);

    //橡胶圈使用次数
    temp[0]= Ring_Num>>8;
    temp[1]= Ring_Num;
    Wr_82_Scr(0x0009,temp,2);

    //版本号
    temp[0]= Version>>8;
    temp[1]= Version;
    Wr_82_Scr(0x001D,temp,2);

    //气压校准系数
    temp[0]= ATM_Coeff>>8;
    temp[1]= ATM_Coeff;
    Wr_82_Scr(0x0030,temp,2);
}

//-------------串口2-DGUS通讯-----------------

void Rs0_Out(uint8_t Cmd)
{
    Com3TBuf[3] = Cmd;								// 命令:0x80-0x84
    Index3End = Com3TBuf[2] + 3;      //串口3接收指令的字节个数，3为5A A5和字节个数的字节数
    Com3Index=0;		                  //发送的数量

    USART_ITConfig(USART2, USART_IT_TXE, ENABLE);

    USART_SendData(USART2, (uint8_t)Com3TBuf[Com3Index++]);

    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);

    while((--Com3Index) != 0);			//等待发送完毕
    // 这里必须Com3Index清0，因为中断返回到了函数USART_SendData(USART3, (uint8_t)Com3TBuf[Com3Index++]);
}

//------------------串口2发送-------------------
// 向串口0发送一个数据串d是地址，s是数据地址偏移量
void send_Ti_scr(uint16_t d,uint8_t *p,uint8_t s)
{
    Com3TBuf[2] = 5;								//  数量=8-3
    Com3TBuf[4] = d >> 8;						//  地址：高8位
    Com3TBuf[5] = d;								//  地址：低8位
    Com3TBuf[6] = *(p + s);
    Com3TBuf[7] = *(p + s + 1);

    Rs0_Out(0x82);									// 写数据到DGUS存储器变量
}


//***********发送一组数据到DGUS存储器:d是存储器地址，p是数据地址指针；s:字节数量 **************
//82指令 4A A4 数据长度 82 变量起始地址 数据 （变量起始地址为2个字节）
void Wr_82_Scr(uint16_t d,uint8_t *p,uint8_t s)
{
    uint8_t i;
    Com3TBuf[2] = s + 3;							// 命令+H、L地址+数据字节数量
    Com3TBuf[4] = d >> 8;							// 地址：高8位
    Com3TBuf[5] = d;							  	// 地址：低8位
    for(i=0; i<s; i++)
    {
        Com3TBuf[i+6] = *(p + i);					//  把要写入的数据数组按字节装入Com3Tbuf[6]开始
    }
    Rs0_Out(0x82);									//  0x82写数据到DGUS变量存储区中
}
//写控制寄存器
//80指令 4A A4 数据长度 80 寄存器 数据 （寄存器为1个字节）
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

// 读控制寄存器指令  s代表多少个字节
//80指令 4A A4数据长度 81 寄存器 字节长度
void Rd_81_Scr(uint8_t d,uint8_t s)
{
    Com3TBuf[2] = 3;
    Com3TBuf[4] = d;
    Com3TBuf[5] = s;	//图片是两个字节
    Rs0_Out(0x81);
}



//
void Wr_84_Scr(uint8_t d,uint16_t s)
{
    Com3TBuf[2] = 0x04;								// 5个字节数据
    Com3TBuf[4] = d;  // 通道号
    Com3TBuf[5] = s >> 8;						// 数据高8位
    Com3TBuf[6] = s;								// 数据低8位
    Rs0_Out(0x84);									// 0x82写数据到DGUS变量存储区
}


// 0x83读数据存储区指令 d:地址  s:字长度 表示从变量地址处读s个字长度
void Rd_83_Scr(uint16_t d,uint8_t s)
{
    Com3TBuf[2] = 4;								//  4个数据长度
    Com3TBuf[4] = d >> 8;						//  高位地址
    Com3TBuf[5] = d;								//  低位地址
    Com3TBuf[6] = s;								//  字长度

    Rs0_Out(0x83);									// 读取DGUS存储区数据
}

//串口2发送一个数据串 d;存储器地址，m:存取模式
//z是字库地址（相对的），g是数据个数
void CQ_To_screen(uint8_t m,uint8_t z,uint16_t d,uint16_t g)
{
    Com3TBuf[2] = 12;								// 字节数量=15-3
    Com3TBuf[4] = 0x56;             // 56寄存器

    Com3TBuf[5] = 0x5A;								// [0x40]=0x5A 申请对字库存储器操作
    Com3TBuf[6] = m;								// [0x41]=m=0x50:写;m=0xA0:读
    /*
    Com0TBuf[7] = z;
    Com0TBuf[8] = 0;
    Com0TBuf[9] = 0;
    Com0TBuf[10] = 0;
    */
    Com3TBuf[7] = 0;								// 字库空间:0x40-0x7F:每个控件128KW
    Com3TBuf[8] = 0xac+z;								// 3:字库首地址:0-0x01 FF FF
    Com3TBuf[9] = 0;
    Com3TBuf[10] = 0;

    Com3TBuf[11] = d >> 8;							// 2:变量存储器首地址:0x0000-0x6FFF
    Com3TBuf[12] = d;
    Com3TBuf[13] = g >> 8;							// 2:长度:0-0x6F FF
    Com3TBuf[14] = g;

    Rs0_Out(0x80);
    Delay100ms(5);
}

//----函数信息--------------
// 函数名：BCDtoDEC
//函数描述：BCD转换成十进制
//入口参数：temp:转化成BCD码
// 返回：转化后的十进制码
uint8_t BCDtoDec(uint8_t temp)
{
    return ((temp/16)*10+temp%16);
}

/* USART2中断处理程序 */
void USART2_IRQHandler(void)
{
    uint8_t  ch;

    // 接收数据
    if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);// 清除中断标志位

        ch = USART_ReceiveData(USART2);
        Com3RBuf[Com3Index]=(uint8_t)ch;

        Com3Index++;

        if(Com3Index==(Com3RBuf[2]+3))
        {
            Rec3FinishF=1;	// 接收完成标志置1
            Com3Index=0;
        }

    }

    // 发送数据
    else if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET) // 判断发送中断是否可以发送
    {
        USART_ClearITPendingBit(USART2, USART_IT_TXE);// 清除中断标志位

        if(Com3Index<Index3End)
        {
            USART_SendData(USART2, (uint8_t)Com3TBuf[Com3Index++]);
            while(USART_GetFlagStatus(USART2,USART_FLAG_TXE) == RESET);
        }
        else  // 已发送完成数据
        {
            Com3Index=0;
            USART_ITConfig(USART2, USART_IT_TXE, DISABLE);

        } // 发送数据

    }
}




