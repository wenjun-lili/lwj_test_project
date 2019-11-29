#include "T6963C.h"
#include "stm32f10x.h"
#include "T6963C_Port.h"

#include "stdio.h"

void LCM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    /* ??FSMC??*/
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

    /* ??FSMC???????? */
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE
                            | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOG
                            | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOC
                            | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    /* ??LCD??????   ????????,???????*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 ;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    /* ??FSMC???????,FSMC-D0~D7: PD 14 15 0 1,PE 7 8 9 10 */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* ??FSMC???????
     * PD4-FSMC_NOE   :LCD-RD
     * PD5-FSMC_NWE   :LCD-WR
     * PD7-FSMC_NE1   :LCD-CS
     * PF0-FSMC_A0    :LCD-DC
    */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/**
 * @brief  LCD  FSMC 模式配置
 * @param  无
 * @retval 无
 */
void LCD_FSMC_Config(void)
{
    FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
    FSMC_NORSRAMInitTypeDef FSMC_NORSRAMInitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;//??NE1
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; //???????
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;//SRAM
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;//????8?
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime       = 2;       // ADDSET
    FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime        = 2;       // ADDHOLD
    FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime          = 7;      // DATAST
    FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration  = 1;      // BUSTURN
    FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision            = 1;      // CLKDIV
    FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency            = 2;      // DATLAT
    FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;    // ACCMOD
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &FSMC_NORSRAMTimingInitStructure;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);
}

void LCD_PortInit(void)
{
    LCM_GPIO_Config();
    LCD_FSMC_Config();
}

void LCD_Reset(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
    Delay100ms(1);
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);   //L电平复位
    Delay100ms(1);
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
    Delay100ms(1);
}

void LCD_Init(void)
{
    LCD_PortInit();
//	LCD_Reset();
    LcmWriteCommandWith2Par(0x00,0x00,0x40); //设置文本显示区首地址
    LcmWriteCommandWith2Par(LCM_XMAX/8,0x00,0x41); //设置文本显示区宽度
    LcmWriteCommandWith2Par(0x00,0x08,0x42);//设置图形显示区首地址
    LcmWriteCommandWith2Par(LCM_XMAX/8,0x00,0x43); //设置图形显示区宽度
    LcmWriteCommand(0xA0);// 设置光标形状1X1方块
    LcmWriteCommand(0x80);// 显示方式设置，文本or图形（或）
    LcmWriteCommand(0x9C); // 显示开关设置，文本开，图形开，光标闪烁关
//#endif


//	LcmWriteCommand(0x90|0x08);				  	// 图形显示层开，其它关
//	Clear(0x0f);

    LcmClear(0x00);//清除文本区
}

// FillByte:0x00 清屏

void LcmClear(uchar FillByte)
{
    uint i=0x2000; // 此处仅清8K RAM，如果您32K RAM都使用，请修改i=32768

    if(FillByte)// 注意如果填充FF时，如果RAM区只使用了8K，那么8192次会写入文本区
    {
        // 会形成乱码。24064出厂时候默认将R7断开，即默认使用8K RAM
        i-=2*1024;	// 所以如果填入FF时候要扣除2K的文本区

        LcmWriteCommandWith2Par(0x00,0x08,0x24); // 填充0xFF时候在图形区
    }
    else
    {
        LcmWriteCommandWith2Par(0x00,0x00,0x24); // RAM区首地址
    }

    LcmWriteCommand(0xB0);// 进入自动写

    while(i--)
    {
        CheckAutoWrite();

        LcmWriteData(FillByte);

//		printf("%d\r\n",i);
    }

    LcmWriteCommand(0xB2); //退出自动写



}

// 设定显示地址函数
void LocateXY(uchar x,uchar y,uchar mode)
{
    uint temp;

    temp=LCM_XMAX/8*y+x;// 计算显示RAM地址

    if(mode) // mode=1为Graphic
    {
        //如果图形模式要加上图形区首地址0x0800
        temp+=0x0800;
    }
    LcmWriteCommandWith2Par(temp&0xff,temp/256,0x24);
}


#if 0
// 显示一个ASCII码函数
//x:列（0-29）y:(0-7)  字符写入子程序（文本属性显示方式）
/*非文本属性显示方式的西文字符写入:设置显示地址指针（24H),再写入
字符代码即可*/
/*该程序使用了字符代码和字符显示属性两个参数。将图形显示区域作为
文本属性区，在写入字符代码后将字符的属性参数写入相应的属性区*/
// 列、行、字符、属性
void ShowChar_ATTP(uchar x,uchar y,uchar CharByte,uint8_t ATTR)
{
    uint8_t  addr1,addr2;
    uint16_t addr;
    addr=y*30+x;
    addr1=addr&0x00ff;
    addr2=addr>>8;
    LcmWriteCommandWith2Par(addr1,addr2,0x24);
    LcmWriteCommandWith1Par(CharByte-0x20,0xC4);
    addr2+=8;
    LcmWriteCommandWith2Par(addr1,addr2,0x24);
    LcmWriteCommandWith1Par(ATTR,0xC4);
}
#endif


/*显示一个ASCII码函数*/
void ShowChar(uchar x,uchar y, char CharByte)
{
    LocateXY(x,y,0);

    LcmWriteCommandWith1Par(CharByte-0x20,0xC4); // 0xC4:数据写，地址不变
}

//显示一个字符串函数 ,8X8点阵



void ShowString(uchar x,uchar y, char *str)
{
    while(*str!=0)
    {
        if(x>29)  //自动换行 240*64
        {
            x=0;
            y++;
        }
        ShowChar(x,y,*str);
        ++x;
        ++str;
    }
}

#if 0
//显示一幅图片函数
void DisplayOneBmp(uchar *puts)
{
    uchar i,j;
    uint X=0;
    LocateXY(0,0,1);
    LcmWriteCommand(0xB0);//进入自动写
    for(i=0; i<LCM_YMAX; i++)
    {
        for(j=0; j<LCM_XMAX/8; j++)
        {
            CheckAutoWrite();
            LcmWriteData(puts[X]);
            X++;
        }
    }
    LcmWriteCommand(0xB2);//退出自动写
}

//反显一幅图片函数
void ReverseOneBmp( void )
{   // 利用数据一次读写操作
    uchar i,j,X;            // 只能对一段RAM取反
    LocateXY(0,0,Graphic);
    for(i=0; i<LCM_YMAX; i++)
    {
        for(j=0; j<LCM_XMAX/8; j++)
        {
            LcmWriteCommand(0xC5);      //数据读地址不变
            CheckRWCD();
            X = LcmReadData();
            X = ~X;
            LcmWriteCommandWith1Par(X,0xC0);//数据写地址加1
        }
    }
}







// 显示一个汉字16X16
// 显示1个汉字，x:列（0-14），y:行(0-3)
// hzcode: 汉字代码（自定义）
void showhz(uint8_t x,uint8_t y,uint8_t z)
{
    unsigned int Address;
    uchar m;

    Address=y*30*16+x+0x0800; //计算显示存储器地址


    for(m=0; m<16; m++) // 一个汉字占上下16行
    {
        LcmWriteCommandWith2Par((unsigned char)(Address), (unsigned char)(Address>>8), 0x24);
        LcmWriteCommandWith1Par(HZTable[z][m*2],0xc0); // 左半部，地址加1
        LcmWriteCommandWith1Par(HZTable[z][m*2+1],0xc0); // 右半部字模地址加1
        // 一个汉字横向取模为两个字节
        Address+=30;// 换行
    }
}

/*显示多个汉字，x:左右点阵间距 (8点阵倍数), y:上下点阵间距（16点阵倍数），从第i个汉字开始
显示，显示j-1个**/
void display_hanzi(uchar x,uchar y,uchar i,uchar j)
{
    for(; i<j; i++)
    {
        showhz(x,y,i);
        x=x+2;
    }
}

#endif


#if 0
/*显示两位数字，每一个8X8点阵，x:左右字符间隔，y:上下字符间隔*/
void display_num(uchar x,uchar y,uchar i)
{
    uint address;
    address=30*y+x;//文本显示，每行16字符
    LcmWriteCommandWith2Par((uchar)(address),(uchar)(address>>8),0x24); //地址指针位置;
    LcmWriteCommandWith1Par(num[i%10]-0x20,0xc0);
    LcmWriteCommandWith1Par(num[i/10]-0x20,0xc0);//写两位数据
}



/*显示240*64图形*/
void display_tuxing(void)
{
    uchar i,j;
    uint address,x;
    address=0x0800;//首地址，图形显示
    for(i=0; i<64; i++) //64行
    {
        LcmWriteCommandWith2Par((uchar)(address),(uchar)(address>>8),0x24);

        for(j=0; j<30; j++) // 每行30字节  240=30 * 8byte
        {
            LcmWriteCommandWith1Par(tuxing[x],0xc0);
            x++;
        }
        address+=30; //换行
    }
}

#endif



/************************LCM应用程序*******************************/

/*SET 按键下总共有七个界面*/





