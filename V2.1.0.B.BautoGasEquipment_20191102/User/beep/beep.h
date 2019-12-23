#ifndef __BEEP_H
#define __BEEP_H

#include "stm32f10x.h"
#include "utility.h"

void beepInit(void);//蜂鸣器初始化
void beepDoing(void);//蜂鸣器响起

void beepMode(uint8_t select);//蜂鸣器工作模式选择
#endif

