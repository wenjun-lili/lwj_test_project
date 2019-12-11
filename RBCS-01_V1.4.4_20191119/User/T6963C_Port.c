//#include "T6963C_Port.h"

///***************************************************************************************
//2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
//64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
//64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
//64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
//64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

//ѡ��BANK1-NORSRAM1 ���� TFT����ַ��ΧΪ0X6000 0000 ~ 0X63FF FFFF
//FSMC_A0 ��LCD��DC(�Ĵ���/����ѡ��)��
//�Ĵ�������ַ = 0X6000 0000
//RAM����ַ = 0X6000 0001 = 0X6000 0000+2^0 = 0X6000 0000 + 0X0000 0001 = 0X6000 0001
//��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���
//****************************************************************************************/
//#define T6963C_BASE		((uint32_t)0x60000000)
//#define T6963C_REG		(*(__IO uint8_t *)(T6963C_BASE + 1))	/* д������ */
//#define T6963C_RAM		(*( __IO uint8_t *)(T6963C_BASE))		/* д������ */
//#define T6963C_RAM_ADDR		T6963C_BASE


///*
//*********************************************************************************************************
//*	�� �� ��: T6963C_WriteCmd
//*	����˵��: дT6963Cָ��Ĵ���
//*	��    ��: ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void T6963C_WriteCmd(uint8_t _ucRegAddr)
//{
//	T6963C_REG = _ucRegAddr;	/* ���üĴ�����ַ */
//}

///*
//*********************************************************************************************************
//*	�� �� ��: T6963C_ReadStatus
//*	����˵��: ��T6963C״̬�Ĵ���
//*	��    ��:  ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//uint8_t T6963C_ReadStatus(void)
//{
//	uint8_t value;
//	value = T6963C_REG;
//	return value;
//}

///*
//*********************************************************************************************************
//*	�� �� ��: T6963C_WriteData
//*	����˵��: дT6963C���ݼĴ���
//*	��    ��:  ��
//*	�� �� ֵ: ��
//*********************************************************************************************************
//*/
//void T6963C_WriteData(uint8_t _ucRegValue)
//{
//	T6963C_RAM = _ucRegValue;	/* ���üĴ�����ַ */
//}

///*
//*********************************************************************************************************
//*	�� �� ��: T6963C_ReadData
//*	����˵��: ��T6963C�Ĵ���ֵ
//*	��    ��: ��
//*	�� �� ֵ: �Ĵ���ֵ
//*********************************************************************************************************
//*/
//uint8_t T6963C_ReadData(void)
//{
//	uint8_t value;
//	value = T6963C_RAM;		/* ��ȡ�Ĵ���ֵ */
//	return value;
//}

//uint8_t LCD_TestStatus(uint8_t bitMatch)
//{
//		uint16_t Error_Counter=0;
//	while((T6963C_ReadStatus()&bitMatch)!=bitMatch)
//	{
//		Error_Counter++;
//		if(Error_Counter>=5000)
//		return 0;
//	}
//	return 1;//return 1  as test status bit ok
//}


// //ָ��&���ݶ�д״̬
//void StatusRW_Check(void)
//{
//	while((T6963C_ReadStatus()&0x03)!=0x03);// STA0��STA1=1:1
//}

//// �����Զ�д״̬
//void CheckAutoWrite(void)
//{
// while((T6963C_ReadStatus()&0x08)==0);
//
//}

//// ����/����������״̬

//void CheckScreen(void)
//{
//  while(T6963C_ReadStatus()&0x40);
//}

//
//// һ������д�뺯��
//
//void LcmWriteData(uint8_t uData)
//{
//    T6963C_WriteData(uData);
//}




//// �޲���ָ��д��
//void LcmWriteCommand(uint8_t Command)
//{
//  	if(LCD_TestStatus(0x03))//���ָ���д״̬
//
//	  T6963C_WriteCmd(Command);
//}

//// ������ָ��д�뺯��
//void LcmWriteCommandWith1Par(uint8_t Parameter,uint8_t Command)
//{
//  	if(LCD_TestStatus(0x03))//���ָ���д״̬

//  T6963C_WriteData(Parameter);

//  	if(LCD_TestStatus(0x03))
//
//  T6963C_WriteCmd(Command);
//}

////

//void LcmWriteCommandWith2Par(uint8_t Parameter1,uint8_t Parameter2,uint8_t Command)
//{
//  	if(LCD_TestStatus(0x03))

//     T6963C_WriteData(Parameter1);

//  	if(LCD_TestStatus(0x03))

//    T6963C_WriteData(Parameter2);

//	  if(LCD_TestStatus(0x03))

//    T6963C_WriteCmd(Command);

//}


#include "T6963C_Port.h"

/***************************************************************************************
2^26 =0X0400 0000 = 64MB,ÿ�� BANK ��4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

ѡ��BANK1-NORSRAM1 ���� TFT����ַ��ΧΪ0X6000 0000 ~ 0X63FF FFFF
FSMC_A0 ��LCD��DC(�Ĵ���/����ѡ��)��
�Ĵ�������ַ = 0X6000 0000
RAM����ַ = 0X6000 0001 = 0X6000 0000+2^0 = 0X6000 0000 + 0X0000 0001 = 0X6000 0001
��ѡ��ͬ�ĵ�ַ��ʱ����ַҪ���¼���
****************************************************************************************/
//#define T6963C_BASE		((uint32_t)0x60000000)
//#define T6963C_REG		(*(__IO uint8_t *)(T6963C_BASE + 1))	/* д������ */
//#define T6963C_RAM		(*( __IO uint8_t *)(T6963C_BASE))		/* д������ */
//#define T6963C_RAM_ADDR		T6963C_BASE
//A0  dat
#define LCD_DAT_ADDR  *((__IO uint8_t *)((uint32_t)0x60000000))  // C/D(A16)=1
//A0  cmd
#define LCD_CMD_ADDR  *((__IO uint8_t *)((uint32_t)0x60000001))

//#define Lcd_Cmd_Reg *((__IO uint8_t *)((uint32_t)0x60020001)) // ?? ?? C/D(A16)=1
//#define Lcd_Data_Reg *((__IO uint8_t *)((uint32_t)0x60000001)) //?? C/D(A16)=0

/*
*********************************************************************************************************
*	�� �� ��: T6963C_WriteCmd
*	����˵��: дT6963Cָ��Ĵ���
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void T6963C_WriteCmd(uint8_t _ucRegAddr)
{
    LCD_CMD_ADDR = _ucRegAddr;	/* ���üĴ�����ַ */
}

/*
*********************************************************************************************************
*	�� �� ��: T6963C_ReadStatus
*	����˵��: ��T6963C״̬�Ĵ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t T6963C_ReadStatus(void)
{
    uint8_t value;
    value = LCD_CMD_ADDR;
    return value;
}

/*
*********************************************************************************************************
*	�� �� ��: T6963C_WriteData
*	����˵��: дT6963C���ݼĴ���
*	��    ��:  ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void T6963C_WriteData(uint8_t _ucRegValue)
{
    LCD_DAT_ADDR= _ucRegValue;	/* ���üĴ�����ַ */
}

/*
*********************************************************************************************************
*	�� �� ��: T6963C_ReadData
*	����˵��: ��T6963C�Ĵ���ֵ
*	��    ��: ��
*	�� �� ֵ: �Ĵ���ֵ
*********************************************************************************************************
*/
uint8_t T6963C_ReadData(void)
{
    uint8_t value;
    value = LCD_DAT_ADDR;		/* ��ȡ�Ĵ���ֵ */
    return value;
}

//uint8_t LCD_TestStatus(uint8_t bitMatch)
//{
//		uint16_t Error_Counter=0;
//	while((T6963C_ReadStatus()&bitMatch)!=bitMatch)
//	{
//		Error_Counter++;
//		if(Error_Counter>=5000)
//		return 0;
//	}
//	return 1;//return 1  as test status bit ok
//}


//ָ��&���ݶ�д״̬
void StatusRW_Check(void)
{
    while((LCD_CMD_ADDR&0x03)!=0x03);// STA0��STA1=1:1
    // �͵�ƽæ
}

// �����Զ�д״̬
void CheckAutoWrite(void)
{
    while((LCD_CMD_ADDR&0x08)==0); // �͵�ƽæ

}

// ����/����������״̬

void CheckScreen(void)
{
    while(LCD_CMD_ADDR&0x40);  // 1������0��ȷ
}


// һ������д�뺯��

void LcmWriteData(uint8_t uData)
{
//	  StatusRW_Check();
    T6963C_WriteData(uData);
}

unsigned char ReadDATA(void)
{
    StatusRW_Check();
    return LCD_DAT_ADDR;
}


// �޲���ָ��д��
void LcmWriteCommand(uint8_t Command)
{
    StatusRW_Check();//���ָ���д״̬

    T6963C_WriteCmd(Command);
}

// ������ָ��д�뺯��
void LcmWriteCommandWith1Par(uint8_t Parameter,uint8_t Command)
{
    StatusRW_Check();//���ָ���д״̬

    T6963C_WriteData(Parameter);

    StatusRW_Check();

    T6963C_WriteCmd(Command);
}

//

void LcmWriteCommandWith2Par(uint8_t Parameter1,uint8_t Parameter2,uint8_t Command)
{
    StatusRW_Check();

    T6963C_WriteData(Parameter1);

    StatusRW_Check();

    T6963C_WriteData(Parameter2);

    StatusRW_Check();

    T6963C_WriteCmd(Command);

}
