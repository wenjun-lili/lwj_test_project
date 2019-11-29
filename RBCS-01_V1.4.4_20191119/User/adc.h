#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

void ADC123_Init(void);

void ADC1_ValConvt(float *p);
void ADC2_ValConvt(uint16_t *p);
void ADC3_ValConvt(float *p);


#endif /* __ADC_H */
