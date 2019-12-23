#ifndef _GLOBALVAR_H
#define _GLOBALVAR_H

#include "stm32f10x.h"
/*
按键相关参数
*/
extern uint8_t F_Key;//按键状态


/*
CO2传感器相关参数
*/
extern uint8_t  Com3TBuf[ ]; // 串口3数据发送缓冲区
extern uint8_t  Com3RBuf[ ]; //串口3数据接收缓冲区
extern uint8_t  Buf_Zero[ ]; //缓冲区清零
extern uint8_t  Com3Index;   //串口3数据数组索引变量
extern uint8_t  Index3End;   //串口3索引结束变量
extern uint8_t  Rec3FinishF; // 串口3接收完成标志,1为接收完成标志
extern uint16_t CO2Con;      //CO2浓度值，扩大100倍
extern uint16_t CO2CONC[1200];
extern uint16_t CO2Threshold;
extern uint8_t  USART3_DMA;  //CO2读取数据，是否使用DMA
extern uint16_t breathPeriod;
extern uint16_t breathPeriod;        //呼吸周期
extern uint16_t diffCO2;           //采样过程中CO2浓度平均差值
extern uint16_t minCO2;
extern uint16_t maxCO2;
/*
三合一传感器相关参数
*/
extern float  BME_Temperature;//三合一传感器获取温度
extern float  BME_Humidity;//三合一传感器获取的湿度
extern float  BME_Pressure;//三合一传感器获取的气压

/*
调试相关参数
*/
extern uint8_t F_Debug;//调试标志，0为无需调试，1为调试状态

/*
错误代码参数
*/
extern uint8_t CO2_Fault_Code;//CO2读取错误代码

/*
中断参数
*/
extern uint16_t timeCnt;

/*
  串口发送数据
 */
extern uint16_t delayBegin;//每次启动前延时时间，单位：ms
extern uint16_t delayAfter;//每次结束后延时关闭时间，单位：ms
extern uint8_t PWMDuty;//气泵运行占空比，默认50%

/*
串口1相关参数
*/
extern uint8_t  Com1TBuf[200];       //串口1数据发送缓冲区
extern uint8_t  Com1RBuf[200];       //串口1数据接收缓冲区
extern uint8_t  Com1Index;           //串口1数据数组索引变量
extern uint8_t  Index1End;         //串口1索引结束变量
extern uint8_t  Rec1FinishF;       //串口1接收完成标志,1为接收完成标志
extern uint32_t inputTemp;         //串口1接收数据暂存变量
extern uint8_t  onLineFlag;        //联机标志 0：未联机  1联机

/*
运行标志
*/
extern uint8_t flagLungRun;        //肺泡气采气流程标志
extern uint8_t flagAgeMode;        //年龄模式的选择
extern uint8_t flagOKGas;
extern uint8_t flagOKSec;
extern uint16_t overTimeCnt;
extern uint8_t flagInit;

/*
错误标志
*/
extern uint8_t flagError;//故障标志，出现采气超时，CO2模块数据错误时报错等标志置1

extern float peakDeter;//波峰判断系数
extern float peakInter;//波峰间距判断系数
extern float vallcyDeter;//波谷判断系数
extern float thresholdCoeff;//始发CO2阈值系数
extern float cutOffCoeff;//截止CO2系数
extern uint16_t cutOffCO2;//截止CO2浓度
extern uint16_t CO2LowExp;//CO2经验低阈值80
extern uint16_t CO2HighExp;//CO2经验高阈值180
extern float getTimeCoeff;//采气时长系数

#endif
