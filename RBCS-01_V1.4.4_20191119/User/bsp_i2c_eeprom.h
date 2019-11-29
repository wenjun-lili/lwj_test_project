/*
*********************************************************************************************************
*
*	模块名称 : 串行EEPROM 24xx02驱动模块
*	文件名称 : bsp_eeprom_24xx.h
*	版    本 : V1.0
*	说    明 : 头文件
*
*	修改记录 :
*		版本号  日期       作者    说明
*		v1.0    2016-2-12 armfly  ST固件库版本 V2.1.0
*
*	Copyright (C),   www.armfly.com
*
*********************************************************************************************************
*/

#ifndef _BSP_EEPROM__H
#define _BSP_EEPROM__H
#include "stm32f10x.h"
//#define AT24C02
//#define AT24C04
#define AT24C01   //我们选用的EEPROM片子是AT24C01 
//#define AT24C128

#define EEPROM

#ifdef AT24C01
#define EE_MODEL_NAME		"AT24C01"
#define EE_DEV_ADDR			0xA0		/* 设备地址 */
#define EE_PAGE_SIZE		8			/* 页面大小(字节) */
#define EE_SIZE				128			/* 总容量(字节) */
#define EE_ADDR_BYTES		1			/* 字节地址个数 */
#define EE_ADDR_A8			0			/* 地址字节的高8bit不在首字节*/
#endif

#ifdef AT24C02
#define EE_MODEL_NAME		"AT24C02"
#define EE_DEV_ADDR			0xA0		/* 设备地址 */
#define EE_PAGE_SIZE		8			/* 页面大小(字节) */
#define EE_SIZE				256			/* 总容量(字节) */
#define EE_ADDR_BYTES		1			/* 字节地址个数 */
#define EE_ADDR_A8			0			/* 地址字节的高8bit不在首字节*/
#endif

#ifdef AT24C04
#define EE_MODEL_NAME		"AT24C04"
#define EE_DEV_ADDR			0xA0		/* 设备地址 */
#define EE_PAGE_SIZE		8			/* 页面大小(字节) */
#define EE_SIZE				512		/* 总容量(字节) */
#define EE_ADDR_BYTES		1			/* 字节地址个数 */
#define EE_ADDR_A8			1			/* 地址字节的高8bit不在首字节*/
#endif

#ifdef AT24C16
#define EE_MODEL_NAME		"AT24C16"
#define EE_DEV_ADDR			0xA0		/* 设备地址 */
#define EE_PAGE_SIZE		16		/* 页面大小(字节) */
#define EE_SIZE				2048			/* 总容量(字节) */
#define EE_ADDR_BYTES		1			/* 字节地址个数 */
#define EE_ADDR_A8			1			/* 地址字节的高8bit不在首字节*/
#endif



#ifdef AT24C128
#define EE_MODEL_NAME		"AT24C128"
#define EE_DEV_ADDR			0xA0		/* 设备地址 */
#define EE_PAGE_SIZE		64			/* 页面大小(字节) */
#define EE_SIZE				(16*1024)	/* 总容量(字节) */
#define EE_ADDR_BYTES		2			/* 字节地址个数 */
#define EE_ADDR_A8			0		/* 地址字节的高8bit不在首字节*/
#endif

uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

void I2CEEPROM_Init(void);
#endif

/***************************** 安富莱电子 www.armfly.com (END OF FILE) *********************************/
