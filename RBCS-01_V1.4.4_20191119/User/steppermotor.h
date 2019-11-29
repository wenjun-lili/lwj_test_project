#ifndef _STEPPERMOTOR_H
#define _STEPPERMOTOR_H


#include "stm32f10x.h"

#define CW_M1()  GPIO_ResetBits(GPIOG, GPIO_Pin_8)
#define CCW_M1() GPIO_SetBits(GPIOG, GPIO_Pin_8)


void Motor1Start(uint8_t dir, uint16_t speed_us);
void Motor1Stop(void);
void MotorInit(void);

#endif



