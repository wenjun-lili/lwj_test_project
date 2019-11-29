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
 * @brief  LCD  FSMC ģʽ����
 * @param  ��
 * @retval ��
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
    GPIO_ResetBits(GPIOC, GPIO_Pin_6);   //L��ƽ��λ
    Delay100ms(1);
    GPIO_SetBits(GPIOC, GPIO_Pin_6);
    Delay100ms(1);
}

void LCD_Init(void)
{
    LCD_PortInit();
//	LCD_Reset();
    LcmWriteCommandWith2Par(0x00,0x00,0x40); //�����ı���ʾ���׵�ַ
    LcmWriteCommandWith2Par(LCM_XMAX/8,0x00,0x41); //�����ı���ʾ�����
    LcmWriteCommandWith2Par(0x00,0x08,0x42);//����ͼ����ʾ���׵�ַ
    LcmWriteCommandWith2Par(LCM_XMAX/8,0x00,0x43); //����ͼ����ʾ�����
    LcmWriteCommand(0xA0);// ���ù����״1X1����
    LcmWriteCommand(0x80);// ��ʾ��ʽ���ã��ı�orͼ�Σ���
    LcmWriteCommand(0x9C); // ��ʾ�������ã��ı�����ͼ�ο��������˸��
//#endif


//	LcmWriteCommand(0x90|0x08);				  	// ͼ����ʾ�㿪��������
//	Clear(0x0f);

    LcmClear(0x00);//����ı���
}

// FillByte:0x00 ����

void LcmClear(uchar FillByte)
{
    uint i=0x2000; // �˴�����8K RAM�������32K RAM��ʹ�ã����޸�i=32768

    if(FillByte)// ע��������FFʱ�����RAM��ֻʹ����8K����ô8192�λ�д���ı���
    {
        // ���γ����롣24064����ʱ��Ĭ�Ͻ�R7�Ͽ�����Ĭ��ʹ��8K RAM
        i-=2*1024;	// �����������FFʱ��Ҫ�۳�2K���ı���

        LcmWriteCommandWith2Par(0x00,0x08,0x24); // ���0xFFʱ����ͼ����
    }
    else
    {
        LcmWriteCommandWith2Par(0x00,0x00,0x24); // RAM���׵�ַ
    }

    LcmWriteCommand(0xB0);// �����Զ�д

    while(i--)
    {
        CheckAutoWrite();

        LcmWriteData(FillByte);

//		printf("%d\r\n",i);
    }

    LcmWriteCommand(0xB2); //�˳��Զ�д



}

// �趨��ʾ��ַ����
void LocateXY(uchar x,uchar y,uchar mode)
{
    uint temp;

    temp=LCM_XMAX/8*y+x;// ������ʾRAM��ַ

    if(mode) // mode=1ΪGraphic
    {
        //���ͼ��ģʽҪ����ͼ�����׵�ַ0x0800
        temp+=0x0800;
    }
    LcmWriteCommandWith2Par(temp&0xff,temp/256,0x24);
}


#if 0
// ��ʾһ��ASCII�뺯��
//x:�У�0-29��y:(0-7)  �ַ�д���ӳ����ı�������ʾ��ʽ��
/*���ı�������ʾ��ʽ�������ַ�д��:������ʾ��ַָ�루24H),��д��
�ַ����뼴��*/
/*�ó���ʹ�����ַ�������ַ���ʾ����������������ͼ����ʾ������Ϊ
�ı�����������д���ַ�������ַ������Բ���д����Ӧ��������*/
// �С��С��ַ�������
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


/*��ʾһ��ASCII�뺯��*/
void ShowChar(uchar x,uchar y, char CharByte)
{
    LocateXY(x,y,0);

    LcmWriteCommandWith1Par(CharByte-0x20,0xC4); // 0xC4:����д����ַ����
}

//��ʾһ���ַ������� ,8X8����



void ShowString(uchar x,uchar y, char *str)
{
    while(*str!=0)
    {
        if(x>29)  //�Զ����� 240*64
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
//��ʾһ��ͼƬ����
void DisplayOneBmp(uchar *puts)
{
    uchar i,j;
    uint X=0;
    LocateXY(0,0,1);
    LcmWriteCommand(0xB0);//�����Զ�д
    for(i=0; i<LCM_YMAX; i++)
    {
        for(j=0; j<LCM_XMAX/8; j++)
        {
            CheckAutoWrite();
            LcmWriteData(puts[X]);
            X++;
        }
    }
    LcmWriteCommand(0xB2);//�˳��Զ�д
}

//����һ��ͼƬ����
void ReverseOneBmp( void )
{   // ��������һ�ζ�д����
    uchar i,j,X;            // ֻ�ܶ�һ��RAMȡ��
    LocateXY(0,0,Graphic);
    for(i=0; i<LCM_YMAX; i++)
    {
        for(j=0; j<LCM_XMAX/8; j++)
        {
            LcmWriteCommand(0xC5);      //���ݶ���ַ����
            CheckRWCD();
            X = LcmReadData();
            X = ~X;
            LcmWriteCommandWith1Par(X,0xC0);//����д��ַ��1
        }
    }
}







// ��ʾһ������16X16
// ��ʾ1�����֣�x:�У�0-14����y:��(0-3)
// hzcode: ���ִ��루�Զ��壩
void showhz(uint8_t x,uint8_t y,uint8_t z)
{
    unsigned int Address;
    uchar m;

    Address=y*30*16+x+0x0800; //������ʾ�洢����ַ


    for(m=0; m<16; m++) // һ������ռ����16��
    {
        LcmWriteCommandWith2Par((unsigned char)(Address), (unsigned char)(Address>>8), 0x24);
        LcmWriteCommandWith1Par(HZTable[z][m*2],0xc0); // ��벿����ַ��1
        LcmWriteCommandWith1Par(HZTable[z][m*2+1],0xc0); // �Ұ벿��ģ��ַ��1
        // һ�����ֺ���ȡģΪ�����ֽ�
        Address+=30;// ����
    }
}

/*��ʾ������֣�x:���ҵ����� (8������), y:���µ����ࣨ16�����������ӵ�i�����ֿ�ʼ
��ʾ����ʾj-1��**/
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
/*��ʾ��λ���֣�ÿһ��8X8����x:�����ַ������y:�����ַ����*/
void display_num(uchar x,uchar y,uchar i)
{
    uint address;
    address=30*y+x;//�ı���ʾ��ÿ��16�ַ�
    LcmWriteCommandWith2Par((uchar)(address),(uchar)(address>>8),0x24); //��ַָ��λ��;
    LcmWriteCommandWith1Par(num[i%10]-0x20,0xc0);
    LcmWriteCommandWith1Par(num[i/10]-0x20,0xc0);//д��λ����
}



/*��ʾ240*64ͼ��*/
void display_tuxing(void)
{
    uchar i,j;
    uint address,x;
    address=0x0800;//�׵�ַ��ͼ����ʾ
    for(i=0; i<64; i++) //64��
    {
        LcmWriteCommandWith2Par((uchar)(address),(uchar)(address>>8),0x24);

        for(j=0; j<30; j++) // ÿ��30�ֽ�  240=30 * 8byte
        {
            LcmWriteCommandWith1Par(tuxing[x],0xc0);
            x++;
        }
        address+=30; //����
    }
}

#endif



/************************LCMӦ�ó���*******************************/

/*SET �������ܹ����߸�����*/





