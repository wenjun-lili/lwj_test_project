#include "globalval.h"

uint8_t BtnPressedFlag=0;//按键标志，0为初始标志
uint8_t ADC_ConvtComplFlag = 0;  /* ADC Bit0置1是光强信号转换完成 Bit1位置1是温度信号转换完成 Bit2是压力信号转换完成 */

uint16_t  CO_ZERO=2645;// 设置零点初值为2645台阶
uint16_t  CO2_PB=0; // 气袋中CO2含量的最小百分比（1%~5%）
uint16_t  R_XB=400;// 系数B (0.04*10000)
uint16_t  CO2_Taijie=150; // 1%CO2的电平数
uint16_t  CO2_ZERO=500;// CO2零点台阶数
uint16_t  HB=0; //病人的血红蛋白数(正常人：110g/L)
uint16_t  Ring_Num=0;//橡胶圈使用次数-20180228
uint16_t  Eeprom_Init=0;//eeprom写入标志
uint8_t   time=100;//完成蜂鸣器开时间

uint16_t  ATM_Coeff=10000;//气压修正系数 20180817

uint16_t QC_CONC=400;//质控气的CO浓度4.99ppmX100
uint16_t QC_CO2_CONC=500;// 质控气的CO2浓度5.00ppmX100

uint16_t Curve_CONC=400; // 5ppm X100
uint16_t BGas_Num=1;//配气肺泡气次数
uint16_t ZGas_Num=1;//配气本底气次数

uint16_t Cal_CO_A=0;//A标准气CO浓度
uint16_t Cal_CO_B=0;//B标准气CO浓度

uint16_t CO2_B=0; // CO2的标气浓度
uint16_t H0=65;//基准海拔65m
uint16_t T0=28;//基准温度28℃，301开尔文
uint16_t H_Var=0;//海拔输入变量20180817
uint16_t T_Var=0;//温度输入变量20180817
uint16_t Version=0;//版本号
uint16_t Password=6666;//工程师界面密码
uint16_t Key_Input=0;//密码输入值
uint16_t Patient_ID;//病人ID
//// 串口接受发送用

uint8_t RBC_pc_ComR[255]= {0};
uint8_t RBC_pc_ComT[6];
uint8_t Write_Receive[255];
uint8_t HB_H=0;//血红蛋白浓度高位
uint8_t HB_L=0;//血红蛋白浓度地位

uint8_t F_PC=0; //联机标志，0：脱机，1：联机
uint8_t F_tiaoji=0; // 调机标志，0： 用户程序 1：调机程序
uint8_t F_RUN2=0; // 用户程序下 0： 一般测量  1：精确测量

uint8_t ReceiveFlag=0;// 1:表示接受完毕
uint8_t RevSDFlag=0; // 接受到上位机上SD卡上要存储的信息
uint8_t PC_ctr_Flag=0;//pc控制标志，1：PC控制测量 0：不直接测量20180228
uint8_t ReceiveFlag1=0;//1表示上位机请求联机，0表示上位机未请求联机
uint8_t ReceiveFlag6=0;//六字节数据接收标志 1标志是接收成功。
/*
DWIN屏相关参数 lcy 20180317
*/
uint8_t  Com3TBuf[200];       // 串口3数据发送缓冲区
uint8_t  Com3RBuf[200];      //串口3数据接收缓冲区
uint8_t  Com3Index;         //串口3数据数组索引变量
uint8_t  Index3End;         //串口3索引结束变量
uint8_t  Rec3FinishF=0;       // 串口3接收完成标志

// 光标位置
uint8_t cursorx;
uint8_t cursory;

uint8_t Sex=0;
uint8_t F_CO2_Low=0;//CO2过低标志位
uint8_t F_Zero_Error;//zero不稳提示标志置1
uint8_t F_Zero=0;//观察零点通讯标志，0为不观察，1为观察
uint16_t USART1Timeout=5000;//串口1通讯超时等待时间1ms
uint8_t F_Timeout=0;//串口通讯超时标志
uint8_t  F_DC=0;///电平打印模式 1代表进入打印模式
uint8_t  F_Sex=0;//性别值接收情况，1为接收到
uint8_t  F_CO2=0;//co2值接收情况，1为接收到数值
uint8_t  Gender=0;///性别，0为女性，1为男性

float    BME_Temperature;///三合一传感器获取温度
float    BME_Humidity;///三合一传感器获取的湿度
float    BME_Pressure;///三合一传感器获取的气压
uint8_t  HumidityThreshold=40;///湿度阈值
uint8_t  F_CO2_Num=0;//CO2测量值小于阈值的次数
uint8_t  F_Zero_Num=0;//CO测量零点异常的次数
uint8_t RetFrame3Flag=1;// 主界面标志,是否要重新显示界面3标志位变量 1:要重新显示屏3 */
