#include "globalVar.h"

/*
按键相关参数
*/
uint8_t F_Key=0;//继续键 0为初始化值，1为短按复位键；2为长按复位键；3为本底气采集键；4为肺泡气采集键。

/*
CO2传感器通讯相关参数
*/
uint8_t  Com3TBuf[200];       //串口3数据发送缓冲区
uint8_t  Com3RBuf[50];       //串口3数据接收缓冲区
uint8_t  Buf_Zero[200]= {0};  //缓冲区清零
uint8_t  Com3Index;           //串口3数据数组索引变量
uint8_t  Index3End=0;         //串口3索引结束变量
uint8_t  Rec3FinishF=0;       //串口3接收完成标志,1为接收完成标志
uint16_t CO2Con=0;            //CO2浓度值，扩大100倍
uint16_t CO2CONC[1200]={0};
uint16_t CO2Threshold;
uint8_t  USART3_DMA=1;        //CO2串口读取数据是否使用DMA，为1时利用DMA，为0时不利用DMA
uint16_t breathPeriod;        //呼吸周期
uint16_t diffCO2=0;           //采样过程中CO2浓度平均差值
uint16_t minCO2=0;            //采样数据得到呼出气体CO2平均最低浓度值
uint16_t maxCO2=0;            //采样数据得到呼出气体CO2平均最大浓度值

/*
三合一传感器相关参数
*/
float  BME_Temperature;//三合一传感器获取温度
float  BME_Humidity;//三合一传感器获取的湿度
float  BME_Pressure;//三合一传感器获取的气压

/*
调试相关参数
*/
uint8_t F_Debug=1;//调试标志位，0为不调试，1为调试

/*
错误代码参数
*/
uint8_t CO2_Fault_Code;//co2读取错误

/*
中断参数
*/
uint16_t timeCnt;//定时数，是1s的倍数


/*
  串口发送数据，默认情况下运行的是成人模式
 */
uint16_t delayBegin=500;//每次启动前延时时间，单位：ms
uint16_t delayAfter=500;//每次结束后延时关闭时间，单位：ms
uint8_t  PWMDuty=100;//气泵运行占空比，默认0%

/*
串口1相关参数
*/
uint8_t  Com1TBuf[200];       //串口1数据发送缓冲区
uint8_t  Com1RBuf[200];       //串口1数据接收缓冲区
uint8_t  Com1Index;           //串口1数据数组索引变量
uint8_t  Index1End=0;         //串口1索引结束变量
uint8_t  Rec1FinishF=0;       //串口1接收完成标志,1为接收完成标志
uint32_t inputTemp=0;         //串口1接收数据暂存变量
uint8_t  onLineFlag=0;        //串口联机标志 0：无需联机  1：联机


/*
运行标志
*/
uint8_t flagLungRun=0;//肺泡气采气进程标志
uint8_t flagAgeMode=1;//人群模式选择，1默认为成人，0默认为儿童。
uint8_t flagOKGas=0;//采集到合格肺泡气标志，0是在采集过程中还未采集到，1是在采集过程中已经采集到过
uint8_t flagOKSec=0;//过了五秒的标志，若是5s的倍数，则赋值为1
uint16_t overTimeCnt=0;//采气合格时间计数值
uint8_t flagInit=0;//初始化完成标志，1为初始化完成

/*
错误标志
*/
uint8_t flagError=0;//故障标志，出现采气超时，CO2模块数据错误时报错等标志置1

float peakDeter=0.5;//波峰判断系数
float peakInter=0.5;//波峰间距判断系数
float vallcyDeter=0.25;//波谷判断系数
float thresholdCoeff=0.50;//采气始发点阈值系数
float cutOffCoeff=0.333333;//截止二氧化碳系数
uint16_t cutOffCO2;//截止CO2浓度
uint16_t CO2LowExp=80;//CO2经验低阈值80
uint16_t CO2HighExp=180;//CO2经验高阈值180
float getTimeCoeff=0.5;//采气时长系数


