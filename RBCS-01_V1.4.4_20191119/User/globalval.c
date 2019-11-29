#include "globalval.h"

int8_t   BtnPressedFlag=-1;
uint8_t  ADC_ConvtComplFlag = 0;  /// ADC Bit0置1是光强信号转换完成 Bit1位置1是温度信号转换完成 Bit2是压力信号转换完成
uint16_t CO_ZERO=2645;/// 设置零点初值为2645台阶
uint16_t CO2_PB=0; /// 气袋中CO2含量的最小百分比（1%~5%）
uint16_t R_XB=400;/// 系数B (0.04*10000)
uint16_t CO2_Taijie=350; /// 1%CO2的电平数
uint16_t CO2_Coeff=100;///CO2修正系数，=传感器发送浓度值/标准气浓度值
uint16_t CO2_ZERO=500;/// CO2零点台阶数
uint16_t HB=150; ///病人的血红蛋白数(正常人：110g/L)
uint16_t Ring_Num=0;///橡胶圈使用次数 20180621
uint16_t QC_CONC=400;///质控气的CO浓度4.99ppmX100
uint16_t QC_CO2_CONC=500;/// 质控气的CO2浓度5.00ppmX100
uint16_t Curve_CONC=400; /// 5ppm X100
uint16_t CO2_B=500; /// CO2的标气浓度
uint8_t  RBC_pc_ComR[255]= {0};///串口接收缓冲
uint8_t  RBC_pc_ComT[6];///串口发送缓冲
uint8_t  Write_Receive[255];
uint8_t  F_PC=0; ///联机标志，0：脱机，1：联机
uint8_t  F_Version=0;///版本请求标志， 0：未请求， 1：请求
uint8_t  F_Zero=0;///零点稳定性测试标志，0：未测试标志，1：测试标志
uint8_t  F_tiaoji=0; /// 调机标志，0： 用户程序 1：调机程序
uint8_t  F_RUN2=0; /// 用户程序下 0： 一般测量  1：精确测量
uint8_t  F_PC_Ctrl=0;///PC控制下位机标志  0：未控制，1：控制
uint8_t  ReceiveFlag=0;/// 1:表示接受完毕
uint8_t  ReceiveFlag1=0;///1:表示DD接受正确
uint8_t  RevSDFlag=0; /// 接受到上位机上SD卡上要存储的信息
uint8_t  Com3TBuf[200];       /// 串口3数据发送缓冲区
uint8_t  Com3RBuf[200];       ///串口3数据接收缓冲区
uint8_t  Buf_Zero[200]= {0};  ///缓冲区清零
uint8_t  Com3Index;           ///串口3数据数组索引变量
uint8_t  Index3End=0;         ///串口3索引结束变量
uint8_t  Rec3FinishF=0;       /// 串口3接收完成标志,1为接收完成标志
uint8_t  cursorx;///光标x坐标
uint8_t  cursory;///光标y坐标
float    Version=3.0;///版本号
uint8_t  CO2_Fault_Code=0;///CO2传感器错误代码，0：正常   1：无通讯  2：校验和不通过
uint16_t USART1Timeout=40000;///串口1通讯超时等待时40s
uint8_t  F_Timeout=0;///串口1通讯超时标志
uint16_t ATM_Coeff=10000;///气压修正系蔄TM_Coeff=(uint16_t)(10000*pow(1-H0/44330,5.255)/pow(1-H_Var/44330,5.255));，转化为5位数
uint16_t H0=65;///基准海拔65m
float    T0=28.0;///基准温度28℃，301开尔文
uint16_t T0_Var=280;
uint16_t Calib=100;///标定系数*100
uint16_t CO2_Taijie_Calib=100;///CO2标定系数*100
uint16_t R_XB_Final=100;///CO标定系数*100
uint16_t QC_Calib=100;///质控海拔修正系数*100
//uint8_t F_HT=0;///生产地海拔和温度是否输入标志位，0为未写入，1为写入
uint8_t  F_HT=0;///是否输入T0和H0
uint8_t  Gender=0;///性别，0为女性，1为男性
float    BME_Temperature;///三合一传感器获取温度
float    BME_Humidity;///三合一传感器获取的湿度
float    BME_Pressure;///三合一传感器获取的气压
uint8_t  F_DC=0;///电平打印模式 1代表进入打印模式
uint16_t HumidityThreshold;///湿度阈值

uint8_t  F_CO2_Num=0;//CO2测量值小于阈值的次数
uint8_t  F_Zero_Num=0;//CO测量零点异常的次数
uint8_t  ReceiveFlag6=0;//六字节数据接收标志 1标志是接收成功。
uint8_t  F_Sex=0;//性别值接收情况，1为接收到
uint8_t  F_CO2=0;//co2值接收情况，1为接收到数值
uint8_t  gasTypeFactor=135;//自动采气类型修正系数，该数值是实际值*100
uint8_t  F_GasType=0;
