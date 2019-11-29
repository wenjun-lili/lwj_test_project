#ifndef _FRAMEDISPLAY_H
#define _FRAMEDISPLAY_H

#include "stm32f10x.h"


void framediaplay0(void);

void framedisplay1(void);

void framedisplay_ok(void);

void framedisplay3(void);

void framedisplay3_sub(void);//显示


// SET界面
void framedisplayDate(void);

void framedisplaytime(void);

void framedisplayZERO(void);

void framedisplayCO2(void);// CO2判定值 (0~5%)

void framedisplayX_B(void);

void framedisplayGasDefault(void);

void framedisplayGasFactor(void);

void framedisplayCO2Coeff(void);

void framedisplayATMCoeff(void);  //气压修正系数显示

void framedisplayRubberRing(void);//橡胶圈使用次数

void framedisplayZEROCO2(void);// CO2零点台阶数

void framedisplayVersion(void);//版本号

void framedisplayH0(void);//海拔

void framedisplayT0(void);//温度

// RUN
void framedisplayRun(void);

void Dis_Sample_Test(void);

void framedisplayQC(void);

void framedisplayQCCO2(void);

void framdisplayCurve(void);

void framdisplayZeroFault(void);//错误显示界面

void framedisplayHumidity(void);//湿度阈值设置界面

void framedisplayAirTight(void);//气密性检验界面

void framedisplaySex(uint8_t m);//调整性别界面
#endif

