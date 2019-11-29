
#ifndef _GLOBALVAL_H_
#define _GLOBALVAL_H_

#include "stm32f10x.h"

extern int8_t BtnPressedFlag;
extern uint8_t ADC_ConvtComplFlag;

extern uint16_t  CO_ZERO; // CO的零点
extern uint16_t   CO2_PB; //0~5%CO2 判断值
extern uint16_t  R_XB; //   曲线系数
extern uint16_t  CO2_Taijie; //1%的台阶数
extern uint16_t  CO2_Coeff;
extern uint16_t  CO2_ZERO;// CO2零点台阶数
extern uint16_t  Ring_Num;//20180621
extern uint16_t   HB;

extern uint16_t QC_CONC;//质控气的CO浓度差
extern uint16_t QC_CO2_CONC;// 质控气的CO2浓度5.00ppmX100

extern uint16_t Curve_CONC;// 作曲线的CO 浓度差


extern uint16_t CO2_B; // CO2的标气浓度

// 串口接受用
extern uint8_t RBC_pc_ComR[];
extern uint8_t RBC_pc_ComT[6];

extern  uint8_t Write_Receive[];



extern uint8_t F_PC;
extern uint8_t F_Version;//版本请求标志， 0：未请求， 1：请求
extern uint8_t F_Zero;//零点稳定性测试标志，0：未测试标志，1：测试标志
extern uint8_t F_tiaoji;
extern uint8_t F_RUN2;
extern uint8_t F_PC_Ctrl;

extern uint8_t ReceiveFlag;
extern uint8_t ReceiveFlag1;
extern uint8_t RevSDFlag;

extern uint8_t  Com3TBuf[ ];
extern uint8_t  Com3RBuf[ ];
extern uint8_t  Buf_Zero[ ];
extern uint8_t  Com3Index;
extern uint8_t  Index3End;
extern uint8_t  Rec3FinishF;
// 光标位置
extern uint8_t cursorx;
extern uint8_t cursory;
extern float Version;
extern uint8_t CO2_Fault_Code;
extern uint16_t USART1Timeout;
extern uint8_t  F_Timeout;

//海拔校正参数
extern uint16_t H0;//生产地的首次标定时的海拔
extern float T0;//生产地的首次标定时的温度
extern uint16_t T0_Var;
extern uint16_t CO2_Taijie_Calib;//CO2标定系数*100
extern uint16_t R_XB_Final;//CO标定系数*100
extern uint16_t QC_Calib;//质控海拔修正系数*100
extern uint8_t F_HT;//生产地首次标定是否输入T和H标
extern uint16_t ATM_Coeff;//气压修正参数
extern uint8_t Gender;//性别

extern float BME_Temperature;//三合一传感器获取温度
extern float BME_Humidity;//三合一传感器获取的湿度
extern float BME_Pressure;//三合一传感器获取的气压
extern uint8_t F_DC;
extern uint16_t HumidityThreshold;///湿度阈值
extern uint8_t  F_CO2_Num;//CO2测量值小于阈值的次数
extern uint8_t  F_Zero_Num;//CO测量零点异常的次数
extern uint8_t  ReceiveFlag6;
extern uint8_t  F_Sex;
extern uint8_t  F_CO2;
extern uint8_t  F_GasType;
extern uint8_t  gasTypeFactor;
#endif

