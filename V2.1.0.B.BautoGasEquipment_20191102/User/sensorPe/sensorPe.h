#ifndef _SENSORPE_H
#define _SENSORPE_H

#include "stm32f10x.h"
#include "utility.h"
#include "globalVar.h"
#include "sensorCO2.h"
#include "bsp_usart.h"
#include "bsp_SysTick.h"

#define breathVol  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_2)//��ȡ��ǻʽ�������ĵ�ƽ

void breathScan(void);//������⴫����״̬ɨ��
void breathSensorInit(void);//����������ų�ʼ��

#endif
