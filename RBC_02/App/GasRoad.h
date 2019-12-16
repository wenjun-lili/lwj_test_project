#ifndef _GASROAD_H
#define _GASROAD_H

#include "stm32f10x.h"

void Open_paiqi(void);
void close_paiqi(void);
void Open_CO2_Paiqi(void);
void Close_CO2_Paiqi(void);
void QiGuang_Reset(void);
void Daiji(uint16_t sec);// 待机时间
void Paiqi_R_PROM(void);// 清洗CO气室
void Paiqi_RRR_PROM(void);  // 清洗CO气室
void Paiqi_RR_PROM(void);// 清洗CO2气室
void Get_CO2_GAS(void);
void Celiang_ZhunBei( uint16_t *ptr);
void Celiang_ZhunBeiNew(uint16_t *Zero, uint16_t *ptr);
void DaoQi_Y(void);
void DaoQi_B(void);
void Chou_BGas(void);
void Chou_YGas(void);
void Get_BGas(void);
void Get_ZGas(void);
void Get_Zerro_Gas(void);
void CO2_CO_SensorZERO(void);
void Rubber_Ring_Fault(void);
void Bme280Fault(void);///三合一传感器预警
void Purge_Cylinder(void);///气缸清洗
void DaoQi_Z(uint16_t sec);//采集零气
#endif
