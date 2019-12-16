
#ifndef _GLOBALVAL_H_
#define _GLOBALVAL_H_

#include "stm32f10x.h"

extern uint8_t BtnPressedFlag;
extern uint8_t ADC_ConvtComplFlag;  /* ADC Bit1置1是光强信号转换完成 Bit2位置1是温度信号转换完成 Bit3是压力信号转换完成 */
extern uint8_t time;
extern uint16_t ATM_Coeff;//气压修正系数

extern uint16_t  CO_ZERO; // CO的零点
extern uint16_t   CO2_PB; //0~5%CO2 判断值
extern uint16_t  R_XB; //   曲线系数
extern uint16_t  CO2_Taijie; //1%的台阶数
extern uint16_t  CO2_ZERO;// CO2零点台阶数
extern uint16_t   HB;
extern uint16_t   Ring_Num;//20180228
extern uint16_t  Eeprom_Init;//eeprom是否有数值判断
extern uint16_t QC_CONC;//质控气的CO浓度差
extern uint16_t QC_CO2_CONC;// 质控气的CO2浓度5.00ppmX100

extern uint16_t Curve_CONC;// 作曲线的CO 浓度差
extern uint16_t Cal_CO_A;
extern uint16_t Cal_CO_B;
extern uint16_t BGas_Num;
extern uint16_t ZGas_Num;

extern uint16_t CO2_B; // CO2的标气浓度
extern uint16_t H0;
extern uint16_t T0;
extern uint16_t H_Var;//海拔输入变量20180817
extern uint16_t T_Var;//温度输入变量20180817
extern uint16_t Version;//版本号
extern uint16_t Password;//密码
extern uint16_t Key_Input;//输入的密码
extern uint16_t Patient_ID;//患者ID
extern uint8_t HB_H;//血红蛋白浓度高位
extern uint8_t HB_L;
// 串口接受用
extern uint8_t RBC_pc_ComR[];
extern uint8_t RBC_pc_ComT[6];

extern  uint8_t Write_Receive[];

#endif

extern  uint8_t F_PC;
extern  uint8_t F_tiaoji;
extern  uint8_t F_RUN2;

extern  uint8_t ReceiveFlag;
extern  uint8_t RevSDFlag;
extern  uint8_t PC_ctr_Flag;
extern  uint8_t ReceiveFlag1;
extern  uint8_t  ReceiveFlag6;

//DWIN屏相关参数 lcy 20180317
extern uint8_t  Com3TBuf[ ];      // 串口3数据发送缓冲区
extern uint8_t  Com3RBuf[ ];      //串口3数据接收缓冲区
extern uint8_t  Com3Index;         //串口3数据数组索引变量
extern uint8_t  Index3End;         //串口3索引结束变量
extern uint8_t  Rec3FinishF; // 串口3接收完成标志

extern uint8_t Sex;
extern uint8_t F_CO2_Low;
extern uint8_t F_Zero_Error;
extern uint8_t F_Zero;
extern uint16_t USART1Timeout;
extern uint8_t F_Timeout;
extern uint8_t F_DC;
extern float BME_Temperature;//三合一传感器获取温度
extern float BME_Humidity;//三合一传感器获取的湿度
extern float BME_Pressure;//三合一传感器获取的气压
extern uint8_t  HumidityThreshold;///湿度阈值
extern uint8_t  F_Sex;//性别值接收情况，1为接收到
extern uint8_t  F_CO2;//co2值接收情况，1为接收到数值
extern uint8_t  Gender;///性别，0为女性，1为男性
extern uint8_t  F_CO2_Num;//CO2测量值小于阈值的次数
extern uint8_t  F_Zero_Num;//CO测量零点异常的次数
extern uint8_t RetFrame3Flag;

