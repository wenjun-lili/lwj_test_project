/*
*********************************************************************************************************
*
*	ģ������ : ����EEPROM 24xx02����ģ��
*	�ļ����� : bsp_eeprom_24xx.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	�޸ļ�¼ :
*		�汾��  ����       ����    ˵��
*		v1.0    2016-2-12 armfly  ST�̼���汾 V2.1.0
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
#define AT24C01   //����ѡ�õ�EEPROMƬ����AT24C01 
//#define AT24C128

#define EEPROM

#ifdef AT24C01
#define EE_MODEL_NAME		"AT24C01"
#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
#define EE_PAGE_SIZE		8			/* ҳ���С(�ֽ�) */
#define EE_SIZE				128			/* ������(�ֽ�) */
#define EE_ADDR_BYTES		1			/* �ֽڵ�ַ���� */
#define EE_ADDR_A8			0			/* ��ַ�ֽڵĸ�8bit�������ֽ�*/
#endif

#ifdef AT24C02
#define EE_MODEL_NAME		"AT24C02"
#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
#define EE_PAGE_SIZE		8			/* ҳ���С(�ֽ�) */
#define EE_SIZE				256			/* ������(�ֽ�) */
#define EE_ADDR_BYTES		1			/* �ֽڵ�ַ���� */
#define EE_ADDR_A8			0			/* ��ַ�ֽڵĸ�8bit�������ֽ�*/
#endif

#ifdef AT24C04
#define EE_MODEL_NAME		"AT24C04"
#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
#define EE_PAGE_SIZE		8			/* ҳ���С(�ֽ�) */
#define EE_SIZE				512		/* ������(�ֽ�) */
#define EE_ADDR_BYTES		1			/* �ֽڵ�ַ���� */
#define EE_ADDR_A8			1			/* ��ַ�ֽڵĸ�8bit�������ֽ�*/
#endif

#ifdef AT24C16
#define EE_MODEL_NAME		"AT24C16"
#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
#define EE_PAGE_SIZE		16		/* ҳ���С(�ֽ�) */
#define EE_SIZE				2048			/* ������(�ֽ�) */
#define EE_ADDR_BYTES		1			/* �ֽڵ�ַ���� */
#define EE_ADDR_A8			1			/* ��ַ�ֽڵĸ�8bit�������ֽ�*/
#endif



#ifdef AT24C128
#define EE_MODEL_NAME		"AT24C128"
#define EE_DEV_ADDR			0xA0		/* �豸��ַ */
#define EE_PAGE_SIZE		64			/* ҳ���С(�ֽ�) */
#define EE_SIZE				(16*1024)	/* ������(�ֽ�) */
#define EE_ADDR_BYTES		2			/* �ֽڵ�ַ���� */
#define EE_ADDR_A8			0		/* ��ַ�ֽڵĸ�8bit�������ֽ�*/
#endif

uint8_t ee_CheckOk(void);
uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize);
uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize);

void I2CEEPROM_Init(void);
#endif

/***************************** ���������� www.armfly.com (END OF FILE) *********************************/
