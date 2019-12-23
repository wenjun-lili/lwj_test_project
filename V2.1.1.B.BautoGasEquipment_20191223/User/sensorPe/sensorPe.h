#ifndef _SENSORPE_H
#define _SENSORPE_H

#include "stm32f10x.h"
#include "utility.h"
#include "globalVar.h"
#include "sensorCO2.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

#define breathVol  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//读取胸腔式传感器的电平

void breathScan(void);//呼吸检测传感器状态扫描
void breathSensorInit(void);//呼吸检测引脚初始化

#endif
