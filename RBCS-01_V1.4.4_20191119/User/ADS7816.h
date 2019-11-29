#ifndef _ADS7816_H_
#define _ADS7816_H_

#include "stm32f10x.h"

#define ADS7816_DOUT_READ()    GPIO_ReadInputDataBit(GPIOF, GPIO_Pin_12)


#define ADCS_L  GPIO_ResetBits(GPIOF,GPIO_Pin_11)
#define ADCS_H  GPIO_SetBits(GPIOF,GPIO_Pin_11)

#define DCLOCK_L  GPIO_ResetBits(GPIOF,GPIO_Pin_5)
#define DCLOCK_H  GPIO_SetBits(GPIOF,GPIO_Pin_5)

#define DOUT_L  GPIO_ResetBits(GPIOF,GPIO_Pin_12)
#define DOUT_H  SetBits(GPIOF,GPIO_Pin_12)

void ADS7816_GPIO_Init(void);

void ADC(uint16_t *p);

#endif