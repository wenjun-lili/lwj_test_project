#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"
#include "utility.h"

void beepInit(void);//��������ʼ��
void beepDoing(void);//����������

void beepMode(uint8_t select);//����������ģʽѡ��
#endif

