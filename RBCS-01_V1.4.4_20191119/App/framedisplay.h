#ifndef _FRAMEDISPLAY_H
#define _FRAMEDISPLAY_H

#include "stm32f10x.h"


void framediaplay0(void);

void framedisplay1(void);

void framedisplay_ok(void);

void framedisplay3(void);

void framedisplay3_sub(void);//��ʾ


// SET����
void framedisplayDate(void);

void framedisplaytime(void);

void framedisplayZERO(void);

void framedisplayCO2(void);// CO2�ж�ֵ (0~5%)

void framedisplayX_B(void);

void framedisplayGasDefault(void);

void framedisplayGasFactor(void);

void framedisplayCO2Coeff(void);

void framedisplayATMCoeff(void);  //��ѹ����ϵ����ʾ

void framedisplayRubberRing(void);//��Ȧʹ�ô���

void framedisplayZEROCO2(void);// CO2���̨����

void framedisplayVersion(void);//�汾��

void framedisplayH0(void);//����

void framedisplayT0(void);//�¶�

// RUN
void framedisplayRun(void);

void Dis_Sample_Test(void);

void framedisplayQC(void);

void framedisplayQCCO2(void);

void framdisplayCurve(void);

void framdisplayZeroFault(void);//������ʾ����

void framedisplayHumidity(void);//ʪ����ֵ���ý���

void framedisplayAirTight(void);//�����Լ������

void framedisplaySex(uint8_t m);//�����Ա����
#endif

