 
#ifndef __BSP_BUTTON_H
#define __BSP_BUTTON_H

#include "stm32f10x.h"

/* �����˲�ʱ��50ms, ��λ10ms
 ֻ��������⵽50ms״̬�������Ϊ��Ч����������Ͱ��������¼�
 ��ʹ������·����Ӳ���˲������˲�����Ҳ���Ա�֤�ɿ��ؼ�⵽�����¼� 
*/
#define BUTTON_FILTER_TIME 	5
#define BUTTON_LONG_TIME  100  /*����1S��Ϊ����ʱ��*/ 

/*
	ÿ��������Ӧ1��ȫ�ֵĽṹ�������
	���Ա������ʵ���˲��Ͷ��ְ���״̬�������
*/
typedef struct
{
	/* ������һ������ָ�룬ָ���жϰ����ַ��µĺ��� */
	uint8_t (*IsKeyDownFunc)(void); /* ����ָ�� �������µ��жϺ���,1��ʾ���� */
	uint8_t Count;			/* �˲��������� */
	uint8_t FilterTime;		/* �˲�ʱ��(���255,��ʾ2550ms) */
	uint16_t LongCount; /*����������*/
	uint16_t LongTime;/*�������³���ʱ�䣬0��ʾ����ⳤ��*/
	uint8_t State;			/* ������ǰ״̬�����»��ǵ��� */
  
	uint8_t KeyCodeUp ;/*��������ļ�ֵ���룬0��ʾ����ⰴ������*/
	uint8_t KeyCodeDown;/*�������µļ�ֵ���룬0��ʾ����ⰴ������*/ 
	uint8_t KeyCodeLong; /*���������ļ�ֵ���룬0��ʾ����ⳤ��*/ 
	uint8_t RepeatSpeed;/*������������*/ 
	uint8_t RepeatCount;/*��������������*/ 
}BUTTON_T;

/* �����ֵ����
	�Ƽ�ʹ��enum, ����#define��ԭ��
	(1) ����������ֵ,�������˳��ʹ���뿴���������
	(2)	�������ɰ����Ǳ����ֵ�ظ���
*/
typedef enum
{
  KEY_NONE = 0,	/* 0 ��ʾ�ް����¼� */
	
	KEY_DOWN_RUN, 	/* RUN������ */
	KEY_UP_RUN,  		/* RUN������ */
	
	KEY_DOWN_PHOTOL,/*PHOTOL������*/
	KEY_UP_PHOTOL,/*PHOTOL������*/
	
	KEY_DOWN_DEC,/*DEC������*/
	KEY_UP_DEC,/*DEC������*/
	KEY_LONG_DEC,/*DEC������*/
	
	KEY_DOWN_INC,/*INC������*/
	KEY_UP_INC,/*INC������*/
	KEY_LONG_INC,/*INC������*/
	
	KEY_DOWN_SET,/*SET������*/
	KEY_UP_SET,/*SET������*/
	
	KEY_DOWN_PHOTOR,/*PHOTOR������*/
	KEY_UP_PHOTOR,/*PHOTOR������*/
	
} KEY_ENUM;

/* ����ID */
enum
{
	RUN_KEY=0,       // ID 0
	PHOTOL_KEY,      //    1
	DEC_KEY,         //    2
	INC_KEY,         //    3
	SET_KEY,         //    4
	PHOTOR_KEY,      //    5
 
	
	/*����������������������*/
//	NA1_KEY,      //    7
//  NA2_KEY,      //    8
//	NA3_KEY       //    9
};

/* ����FIFO�õ����� */
#define KEY_FIFO_SIZE	 20
typedef struct
{ 
	////uint8_t Id[KEY_FIFO_SIZE];
	uint8_t Buf[KEY_FIFO_SIZE];		/* ��ֵ������ */
	uint8_t Read;					/* ��������ָ�� */
	uint8_t Write;				/* ������дָ�� */
}KEY_FIFO_T;

/* ���ⲿ���õĺ������� */
void bsp_InitButton(void);
void bsp_PutKey(uint8_t _KeyCode);
uint8_t bsp_GetKey(void);
uint8_t MK_KeyValid(uint8_t fifokeyid, uint8_t matchedkeyid);

void bsp_KeyScan(void);
uint8_t bsp_KeyState(uint8_t _ucKeyID);
void MKeyProcess(void);
#endif
