#include "process.h"
#include "rtc.h"
#include "globalval.h"
#include "DWIN.h"
#include "bsp.h"

extern uint8_t BtnPressedFlag;
extern uint16_t DGUS_Layout_Flag;
extern uint16_t Warming_up_Cnt;

/**
  *@breif 事件处理
	*@param none
	*@retval none
	*/
void Event_Process(void)
{
    //预热界面判断刷新
    if( (DGUS_Layout_Flag == 2) && ((BtnPressedFlag == 0xAA) || (Warming_up_Cnt == 0)) )
    {
        DGUS_Layout_Flag = 1000;//1000代表页面刷新
        BtnPressedFlag = 0;
    }
    //联机界面判断刷新
    if( (DGUS_Layout_Flag == 1) && (BtnPressedFlag == 0xAA))
    {
        DGUS_Layout_Flag = 1000;
        BtnPressedFlag = 0;
    }

}

/**
  *@breif 按键处理
	*@param none
	*@retval none
	*/
void Touch_Process(void)
{
    /*键值配置 20180320
    键值      功能
    0x0011    CO2标定键
    0x0022    质控键
    0x0033    配气键
    0x0044    密封圈更换清零键
    0x0055    气密性键
    0x0066    日期/时间配置键或者 日期/时间/密封圈配置键
    0x0077    关闭电磁阀
    0x0088    版本号键
    0x0099    参数配置键
    0x00AA    返回预热界面/返回待机界面/取消键
    0x00BB    跳到工程师输入密码界面/测量键/标定键/开启电磁阀键/确认键/开始键
    0x00CC    标准测量键
    0x00DD	  精确测量键
    0x00EE    返回设置键
    0x00FF    CO标定键
    */
    if((Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x20) && (Com3RBuf[5] == 0x00))
    {
        while(!Rec3FinishF);
        Rec3FinishF = 0;
        BtnPressedFlag = Com3RBuf[8];
        Com3RBuf[4] = 0;
        Com3RBuf[5] = 0;
        Com3RBuf[8] = 0;
//        printf("键值是%02x\r\n", BtnPressedFlag);
    }
    /*参数输入触控配置 20180320
    主要参数：密码、CO系数、CO2系数、CO2零点、CO零点阈值、CO2浓度阈值、患者ID、HB、A测试气CO浓度、B测试气CO 浓度
    				CO2标准气浓度
              配气肺泡气次数、配气本底气次数
    */
    /*密码输入配置 20180320
    指令:5A A5 06 83 00 21 01 XX XX
    */
#if 0
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x21) && (Com3RBuf[6] == 0x01))
    {
        Key_Input = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("key is %x\r\n", Key_Input);
    }
    /*R_XB输入配置 20180320
    指令:5A A5 06 83 00 05 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x05) && (Com3RBuf[6] == 0x01))
    {
        R_XB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("R_XB is	%x\r\n", R_XB);
    }
    /*CO2_Taijie输入配置 20180320
    指令:5A A5 06 83 00 06 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x06) && (Com3RBuf[6] == 0x01))
    {
        CO2_Taijie = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_Taijie is	%d\r\n", CO2_Taijie);
    }
    /*CO2_ZERO输入配置 20180320
    指令:5A A5 06 83 00 07 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x07) && (Com3RBuf[6] == 0x01))
    {
        CO2_ZERO = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_ZERO is	%d\r\n", CO2_ZERO);
    }
    /*CO2_PB输入配置 20180320
    指令:5A A5 06 83 00 07 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x03) && (Com3RBuf[6] == 0x01))
    {
        CO2_PB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_PB is	%d\r\n", CO2_PB);
    }
    /*CO2_PB输入配置 20180320
    指令:5A A5 12 83 00 0A 04 XX XX XX XX XX XX XX XX
    */
    if((Com3RBuf[2] == 0x12) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0A) && (Com3RBuf[6] == 0x01))
    {
        Patient_ID = (uint16_t)Com3RBuf[7] << 8 | (uint16_t)Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("Patient_ID is	%lld\r\n", Patient_ID);
    }
    /*HB输入配置 20180320
    指令:5A A5 06 83 00 0E 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0E) && (Com3RBuf[6] == 0x01))
    {
        HB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("HB is	%d\r\n", HB);
    }
    /*CO2_B(co2标准气浓度)输入配置 20180320
    指令:5A A5 06 83 00 12 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
    {
        CO2_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("CO2_B is	%d\r\n", CO2_B);
    }
    /*Cal_CO_A(A标准气CO浓度)输入配置 20180320
    指令:5A A5 06 83 00 13 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x13) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("Cal_CO_A is	%d\r\n", Cal_CO_A);
    }
    /*Cal_CO_B(B标准气CO浓度)输入配置 20180320
    指令:5A A5 06 83 00 15 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x15) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        printf("Cal_CO_B is	%d\r\n", Cal_CO_B);
    }
    /*BGas_Num(配气肺泡气次数)输入配置 20180320
    指令:5A A5 06 83 00 1E 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1E) && (Com3RBuf[6] == 0x01))
    {
        BGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("BGas_Num is	%d\r\n", BGas_Num);
    }
    /*ZGas_Num(配气肺泡气次数)输入配置 20180320
    指令:5A A5 06 83 00 1F 01 XX XX
    */
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1F) && (Com3RBuf[6] == 0x01))
    {
        ZGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        for(i = 2; i < 9; i++)
        {
            Com3RBuf[i] = 0;
        }
        printf("ZGas_Num is	%d\r\n", ZGas_Num);
    }
#endif
}
