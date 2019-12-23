#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f10x.h"
#include "bsp.h"

#define LEDRun PDout(9)
void errorLedInit(void);//���ϵƳ�ʼ��
void errorLedDoing(uint8_t select);//���ϵƹ���

void CO2LedInit(void);//CO2״̬�Ƴ�ʼ��
void CO2LedDoing(uint8_t select);//CO2״̬�ȹ���

void runLedInit(void);//���еƳ�ʼ��
void runLedDoing(uint8_t select);//����״̬����

void ledInit(void);//���еƳ�ʼ��

#endif
