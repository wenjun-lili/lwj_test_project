#ifndef _BSP_LED_H
#define _BSP_LED_H

#include "stm32f10x.h"
#include "bsp.h"

#define LEDRun PDout(9)
void errorLedInit(void);//故障灯初始化
void errorLedDoing(uint8_t select);//故障灯工作

void CO2LedInit(void);//CO2状态灯初始化
void CO2LedDoing(uint8_t select);//CO2状态等工作

void runLedInit(void);//运行灯初始化
void runLedDoing(uint8_t select);//运行状态工作

void ledInit(void);//所有灯初始化

#endif
