#ifndef _MEASUREMENT_H
#define _MEASUREMENT_H

#include "stm32f10x.h"
void OnlinePC (void);
void Set_Para(void);

void date_add(void);
void date_dec(void);

void time_add(void);
void time_dec(void);


void RUN_Program(void);
void QC_PROM(void);
void Curve_PROM(void);
void PeiGas_PROM(void);
void CO2_CON(void);
void CO2_Cal_Pro(void);
void CO2_Zero(void);
void CO2_percent1(void);

void Chu_Zhi(void);
float StandardDeviation(float *P,uint8_t num,uint8_t select,uint16_t Stand);

#endif
