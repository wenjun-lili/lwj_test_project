#ifndef _GASROAD_H
#define _GASROAD_H

#include "stm32f10x.h"

void Open_paiqi(void);
void close_paiqi(void);

void Open_CO2_Paiqi(void);
void Close_CO2_Paiqi(void);

void SELET_CO(void);
void CO2_Test(void);

void QiGuang_Reset(void);


void Daiji(uint16_t sec);/// ����ʱ��
void Purge_Cylinder(void);///������ϴ
void Paiqi_R_PROM(void);/// ��ϴCO����
void Paiqi_RRR_PROM(void);  /// ��ϴCO����
void DaoQi_Z(uint16_t sec);///�ɼ�����
void Paiqi_RR_PROM(void);/// ��ϴCO2����

void Get_CO2_GAS(uint8_t Select_Valve);

void Celiang_ZhunBei( uint16_t *ptr);
void Celiang_ZhunBeiNew(uint16_t *Zero, uint16_t *ptr);
uint16_t Get_CO2(void);
uint16_t Celiang_CO2(uint8_t i);
void DaoQi_Y(void);
void DaoQi_B(void);

void Chou_BGas(void);
void Chou_YGas(void);

void Get_BGas(void);
void Get_ZGas(void);

void Get_Zerro_Gas(void);

void CO2_CO_SensorZERO(void);
void Rubber_Ring_Fault(void);
void CO2_Sensor_Fault(void);
void Humidity_Fault(void);///�����ʹ��Ԥ��
void Bme280Fault(void);///����һ������Ԥ��
void airTightTest(void);//�����Լ���
#endif
