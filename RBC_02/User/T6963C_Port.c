//#include "T6963C_Port.h"

///***************************************************************************************
//2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
//64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
//64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
//64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
//64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

//选择BANK1-NORSRAM1 连接 TFT，地址范围为0X6000 0000 ~ 0X63FF FFFF
//FSMC_A0 接LCD的DC(寄存器/数据选择)脚
//寄存器基地址 = 0X6000 0000
//RAM基地址 = 0X6000 0001 = 0X6000 0000+2^0 = 0X6000 0000 + 0X0000 0001 = 0X6000 0001
//当选择不同的地址线时，地址要重新计算
//****************************************************************************************/
//#define T6963C_BASE		((uint32_t)0x60000000)
//#define T6963C_REG		(*(__IO uint8_t *)(T6963C_BASE + 1))	/* 写命令用 */
//#define T6963C_RAM		(*( __IO uint8_t *)(T6963C_BASE))		/* 写数据用 */
//#define T6963C_RAM_ADDR		T6963C_BASE


///*
//*********************************************************************************************************
//*	函 数 名: T6963C_WriteCmd
//*	功能说明: 写T6963C指令寄存器
//*	形    参: 无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void T6963C_WriteCmd(uint8_t _ucRegAddr)
//{
//	T6963C_REG = _ucRegAddr;	/* 设置寄存器地址 */
//}

///*
//*********************************************************************************************************
//*	函 数 名: T6963C_ReadStatus
//*	功能说明: 读T6963C状态寄存器
//*	形    参:  无
//*	返 回 值: 无
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
//*	函 数 名: T6963C_WriteData
//*	功能说明: 写T6963C数据寄存器
//*	形    参:  无
//*	返 回 值: 无
//*********************************************************************************************************
//*/
//void T6963C_WriteData(uint8_t _ucRegValue)
//{
//	T6963C_RAM = _ucRegValue;	/* 设置寄存器地址 */
//}

///*
//*********************************************************************************************************
//*	函 数 名: T6963C_ReadData
//*	功能说明: 读T6963C寄存器值
//*	形    参: 无
//*	返 回 值: 寄存器值
//*********************************************************************************************************
//*/
//uint8_t T6963C_ReadData(void)
//{
//	uint8_t value;
//	value = T6963C_RAM;		/* 读取寄存器值 */
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


// //指令&数据读写状态
//void StatusRW_Check(void)
//{
//	while((T6963C_ReadStatus()&0x03)!=0x03);// STA0：STA1=1:1
//}

//// 数据自动写状态
//void CheckAutoWrite(void)
//{
// while((T6963C_ReadStatus()&0x08)==0);
//
//}

//// 屏读/屏拷贝出错状态

//void CheckScreen(void)
//{
//  while(T6963C_ReadStatus()&0x40);
//}

//
//// 一个数据写入函数
//
//void LcmWriteData(uint8_t uData)
//{
//    T6963C_WriteData(uData);
//}




//// 无参数指令写入
//void LcmWriteCommand(uint8_t Command)
//{
//  	if(LCD_TestStatus(0x03))//检查指令读写状态
//
//	  T6963C_WriteCmd(Command);
//}

//// 单参数指令写入函数
//void LcmWriteCommandWith1Par(uint8_t Parameter,uint8_t Command)
//{
//  	if(LCD_TestStatus(0x03))//检查指令读写状态

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
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-NORSRAM1 连接 TFT，地址范围为0X6000 0000 ~ 0X63FF FFFF
FSMC_A0 接LCD的DC(寄存器/数据选择)脚
寄存器基地址 = 0X6000 0000
RAM基地址 = 0X6000 0001 = 0X6000 0000+2^0 = 0X6000 0000 + 0X0000 0001 = 0X6000 0001
当选择不同的地址线时，地址要重新计算
****************************************************************************************/
//#define T6963C_BASE		((uint32_t)0x60000000)
//#define T6963C_REG		(*(__IO uint8_t *)(T6963C_BASE + 1))	/* 写命令用 */
//#define T6963C_RAM		(*( __IO uint8_t *)(T6963C_BASE))		/* 写数据用 */
//#define T6963C_RAM_ADDR		T6963C_BASE
//A0  dat
#define LCD_DAT_ADDR  *((__IO uint8_t *)((uint32_t)0x60000000))  // C/D(A16)=1
//A0  cmd
#define LCD_CMD_ADDR  *((__IO uint8_t *)((uint32_t)0x60000001))

//#define Lcd_Cmd_Reg *((__IO uint8_t *)((uint32_t)0x60020001)) // ?? ?? C/D(A16)=1
//#define Lcd_Data_Reg *((__IO uint8_t *)((uint32_t)0x60000001)) //?? C/D(A16)=0

/*
*********************************************************************************************************
*	函 数 名: T6963C_WriteCmd
*	功能说明: 写T6963C指令寄存器
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void T6963C_WriteCmd(uint8_t _ucRegAddr)
{
    LCD_CMD_ADDR = _ucRegAddr;	/* 设置寄存器地址 */
}

/*
*********************************************************************************************************
*	函 数 名: T6963C_ReadStatus
*	功能说明: 读T6963C状态寄存器
*	形    参:  无
*	返 回 值: 无
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
*	函 数 名: T6963C_WriteData
*	功能说明: 写T6963C数据寄存器
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void T6963C_WriteData(uint8_t _ucRegValue)
{
    LCD_DAT_ADDR= _ucRegValue;	/* 设置寄存器地址 */
}

/*
*********************************************************************************************************
*	函 数 名: T6963C_ReadData
*	功能说明: 读T6963C寄存器值
*	形    参: 无
*	返 回 值: 寄存器值
*********************************************************************************************************
*/
uint8_t T6963C_ReadData(void)
{
    uint8_t value;
    value = LCD_DAT_ADDR;		/* 读取寄存器值 */
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


//指令&数据读写状态
void StatusRW_Check(void)
{
    while((LCD_CMD_ADDR&0x03)!=0x03);// STA0：STA1=1:1
    // 低电平忙
}

// 数据自动写状态
void CheckAutoWrite(void)
{
    while((LCD_CMD_ADDR&0x08)==0); // 低电平忙

}

// 屏读/屏拷贝出错状态

void CheckScreen(void)
{
    while(LCD_CMD_ADDR&0x40);  // 1出错，0正确
}


// 一个数据写入函数

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


// 无参数指令写入
void LcmWriteCommand(uint8_t Command)
{
    StatusRW_Check();//检查指令读写状态

    T6963C_WriteCmd(Command);
}

// 单参数指令写入函数
void LcmWriteCommandWith1Par(uint8_t Parameter,uint8_t Command)
{
    StatusRW_Check();//检查指令读写状态

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

