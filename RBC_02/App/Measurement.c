#include "Measurement.h"
#include "bsp.h"
#include "DWIN.h"
#include "globalval.h"
#include "BME280.h"


extern struct rtc_time systmtime;
extern uint8_t RetFrame3Flag;

/**
  *@brief 上下位机联机程序
  *@param  none
  *@retval none
  */
void OnlinePC (void)
{
    uint8_t temp[2] = {0x00, 0x01}; //页面1
    uint16_t numm = 0;

    Delay100ms(20);//2s

    if(ReceiveFlag1 == 0) //若上位机未发送请求联机信号，则发送0xAA联络信号
    {
        RBCtoPC_Cmd_AA();//发送0xAA联络信号
        while(!ReceiveFlag)
        {
            numm++;
            Delay10ms(1);
            if(numm > 150) //1.5s未接收到回复，则跳过
            {
                break;
            }
        }
    }
    if(ReceiveFlag == 1) // 接收完了整包数据
    {
        ReceiveFlag = 0;

        if(ReceiveFlag1 == 0)
        {
            switch(RBC_pc_ComR[0])
            {
            case 0xBB://联机应答
                RBC_pc_ComR[0] = 0; // 清0
                F_PC = 1; //联机标志
                temp[0] = 0x00;
                temp[1] = 0x01;
                Wr_82_Scr(0x0070, temp, 2);
                break;
            case 0x99://维护模式应答，可以查看流程中数值
                RBC_pc_ComR[0] = 0;
                F_DC = 1;
                USART_SendData(USART1, (uint8_t)0xAD);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
                break;
            case 0xEE://电平打印模式应答
                RBC_pc_ComR[0] = 0;
                F_Zero = 1;
                break;
            default:
                F_PC = 0;
                temp[0] = 0x00;
                temp[1] = 0x00;
                Wr_82_Scr(0x0070, temp, 2);
                break;

            }
        }
    }
    ReceiveFlag1 = 0;
    TIM6Delay_f(100);// 延时0.1S
}
/**
  *@breif  参数设置
	*@detail 因为各个参数使用的是按键数据自动上传功能，所以无需判断 20180329
	*@param  none
	*@retval none
	*/
void Set_Para(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x09;
    Wr_80_Scr(0x03, temp, 2);
    while(1)
    {
        if(BtnPressedFlag == 0xAA)
        {
            BtnPressedFlag = 0;
// 20180411修改，原始问题：修改完成后重启数值被刷新为原始数值
            //读取CO2_PB输入值
            Rd_83_Scr(0x0003, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x03) && (Com3RBuf[6] == 0x01))
                CO2_PB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO2_PB=%d\r\n", CO2_PB);

            //读取CO_ZERO输入值
            Rd_83_Scr(0x0004, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x04) && (Com3RBuf[6] == 0x01))
                CO_ZERO = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO_ZERO=%d\r\n", CO_ZERO);

            //读取R_XB输入值
            Rd_83_Scr(0x0005, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x05) && (Com3RBuf[6] == 0x01))
                R_XB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("Kco(x-b)=%d\r\n", R_XB);

            //读取CO2Taijie输入值
            Rd_83_Scr(0x0006, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x06) && (Com3RBuf[6] == 0x01))
                CO2_Taijie = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO2_Taijie=%d\r\n", CO2_Taijie);

            //读取CO2_ZERO输入值
            Rd_83_Scr(0x0007, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x07) && (Com3RBuf[6] == 0x01))
                CO2_ZERO = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO2_ZERO=%d\r\n", CO2_ZERO);

            //读取ATM_Coeff值
            Rd_83_Scr(0x0030, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x30) && (Com3RBuf[6] == 0x01))
                ATM_Coeff = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("气压修正系数=%d\r\n", ATM_Coeff);

            ee_WriteBytes((uint8_t *)&CO_ZERO, 0, 2);
            ee_WriteBytes((uint8_t *)&CO2_PB, 2, 2);
            ee_WriteBytes((uint8_t *)&R_XB, 4, 2);
            ee_WriteBytes((uint8_t *)&CO2_Taijie, 6, 2);
            ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);
            ee_WriteBytes((uint8_t *)&ATM_Coeff, 14, 2);
            break;
        }
    }
}
/**
  *@breif  标准测量程序
  *@param  none
  *@retval none
  */
void RUN_Program(void)//标准测量
{
    uint8_t i;
    uint8_t Zero_Num = 0; //零点出现错误的次数
    uint8_t temp[2];
    uint8_t temp1[4];
    float CO_Cha;
    float CO_AD2_Z, CO_AD1_Z; //采样的CO电平数与零点电平的差值，临时变量
    float PcoR, Pco; // CO实测值
    float CO2_CONC; // CO2浓度
    uint16_t Diff_Zero;
    uint16_t RBC;
    uint16_t CO2_AD, CO2_ZTest; // 实测CO2台阶数 以及测试CO2零点
    uint16_t CO_ZERO1, CO_ZERO2;
    uint16_t CO_AD1, CO_AD2;
    uint16_t nnnn = 0;

    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
    temp[0] = Ring_Num >> 8;
    temp[1] = Ring_Num;
    Wr_82_Scr(0x0009, temp, 2);

    /*测量样气的CO2*/
    if(F_PC == 1)
    {
        FS_RUN_step1_ON();
    }
    temp[0] = 0x00;
    temp[1] = 0x0D;
    Wr_80_Scr(0x03, temp, 2); //跳到13标准测量02界面

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);    //测量温度和气压
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x000A, temp, 2);

    Purge_Cylinder();
    Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);// 清洗CO2气室并测试零点
    Paiqi_RR_PROM();// 清洗CO2气室10 S

    CO2_CONC = (float)(CO2_AD - CO2_ZTest) / CO2_Taijie;

    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);

    if(F_DC == 1)
    {
        printf("气压校正系数=%d\r\n", ATM_Coeff);
        printf("气压=%f百帕\r\n", BME_Pressure);
        printf("温度=%f摄氏度\r\n", BME_Temperature);
        printf("CO2零点值=%d,CO2电平值=%d\r\n", CO2_ZTest, CO2_AD);
        printf("CO2浓度M/V=%f\r\n", CO2_CONC);
    }

    CO2_CONC = CO2_CONC * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / 301;

    if(F_DC == 1)
    {
        printf("CO2浓度V/V=%f\r\n", CO2_CONC);
    }
    temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
    temp[1] = (uint16_t)(CO2_CONC * 100);
    Wr_82_Scr(0x0022, temp, 2);

    if(CO2_CONC < 0.50) //实测值小于等于0.5%
    {
        temp[0] = 0x00;
        temp[1] = 0x3C;
        Wr_80_Scr(0x03, temp, 2); //跳到60CO2浓度过低错误界面
        if(F_PC == 1)
        {
            FS_CO2_ERRO();
        }
        goto LAGE1;
    }
    if((CO2_CONC < 2.0000) || (CO2_CONC >= 7.0000))
    {
        F_CO2_Num++;
        if(F_DC == 1)
        {
            printf("F_CO2_Num=%d\r\n", F_CO2_Num);
        }
    }
    else if(CO2_CONC < 3.0000)
    {
        F_CO2_Low = 1; //二氧化碳过低标志
        F_CO2_Num = 0;
    }
    else
    {
        F_CO2_Num = 0;
    }
    temp[0] = 0x00;
    temp[1] = 0x0E;
    Wr_80_Scr(0x03, temp, 2);	//跳到14标准测量4界面
    if(F_PC == 1)
    {
        FS_RUN_step1_OK();
    }
LAGE_TEST:
    temp[0] = 0x00;
    temp[1] = 0x0F;
    Wr_80_Scr(0x03, temp, 2); //	跳到15标准测量5界面
    if(F_PC == 1)
    {
        FS_RUN_step2_ON();
    }

    /*测量底气CO台阶*/
    Purge_Cylinder();
    Paiqi_R_PROM();//清洗CO气室100S
    ADC(&CO_ZERO1); //测B气CO零点ZERO1
    if(F_DC == 1)
    {
        printf("底气零点值=%d\r\n", CO_ZERO1);
    }
    for(i = 0; i < 2; i++)
    {
        Chou_BGas();// 从环境倒气袋抽环境气2次E5->E1
    }
    Daiji(30);// 倒计时30S
    ADC(&CO_AD1); //  测量环境气B气的台阶
    if(F_DC == 1)
    {
        printf("底气电平值=%d\r\n", CO_AD1);
    }
    if(F_PC == 1)
    {
        FS_RUN_step2_OK();
    }

    temp[0] = 0x00;
    temp[1] = 0x10;
    Wr_80_Scr(0x03, temp, 2); //	跳到16标准测量6界面

    /*测量样气CO台阶*/
    if(F_PC == 1)
    {
        FS_RUN_step3_ON();
    }
    temp[0] = 0x00;
    temp[1] = 0x11;
    Wr_80_Scr(0x03, temp, 2); //	跳到17标准测量7界面
    Paiqi_R_PROM();//清洗CO气室100S

    ADC(&CO_ZERO2); //Y气CO零点
    if(F_DC == 1)
    {
        printf("样气零点值=%d\r\n", CO_ZERO2);
    }
    //零点错误判定
    Diff_Zero = abs(CO_ZERO2 - CO_ZERO1);
    if(Diff_Zero > 10)
    {
        if(Diff_Zero > 20)
        {
            if(Zero_Num == 1)
            {
                F_Zero_Num++;
                if(F_DC == 1)
                {
                    printf("F_Zero_Num=%d\r\n", F_Zero_Num);
                }
            }
            else
            {
                Zero_Num++;
                Daiji(180);//待机180s
                goto LAGE_TEST;//跳到第二步：底气零点测试

            }
        }
        else
        {
            F_Zero_Error = 1; //提示标志置1
            F_Zero_Num = 0; //因为异常不连续则归零
        }
    }
    else
    {
        F_Zero_Num = 0; //因为异常不连续则归零
    }

    for(i = 0; i < 2; i++)
    {
        Chou_YGas();// 从环境倒气袋抽环境气2次  E4->E1
    }

    Daiji(30);// 倒计时30S
    ADC(&CO_AD2); // 测量Y气的台阶
    if(F_DC == 1)
    {
        printf("样气电平值=%d\r\n", CO_AD2);
    }
    CO_AD2_Z = CO_AD2 - CO_ZERO2;
    if(CO_AD2_Z <= 0.00001) //用于判定样气比零点还低的情况
    {
        CO_AD2_Z = 0.0;
    }
    //计算CO底气的台阶差
    CO_AD1_Z = CO_AD1 - CO_ZERO1;
    if(CO_AD1_Z <= 0.0001)
    {
        CO_AD1_Z = 0.0;
    }

    CO_Cha = CO_AD2 - CO_AD1;
    PcoR = CO_Cha * (float)R_XB / 10000; // ppm  该数值是M/V模式
    Pco = PcoR * ATM_Coeff * 0.0001 * (273 + BME_Temperature) / (273 + T0); //进行气压和温度校正
    if(F_DC == 1)
    {
        printf("CO_Cha=%f\r\n", CO_Cha);
        printf(" M/V校正前 PcoR=%f\r\n", PcoR);
        printf("气压和温度校正后Pco=%f\r\n", Pco);
    }
    Pco = Pco * (1 - CO2_CONC * 0.01) - CO2_CONC * 0.01 * R_XB / 10000 * CO_AD1_Z; //进行CO2吸收浓缩校正

    if(F_DC == 1)
    {
        printf("CO2浓缩校正后Pco=%f\r\n", Pco);
    }
    if(F_PC == 1)
    {
        FS_RUN_step3_OK();
    }

    /*性别校正*/
    if(Sex == 1)
    {
        if(CO2_CONC < 5.3)
        {
            Pco = Pco * 5.3 / CO2_CONC;

        }
        else if(CO2_CONC > 6.0)
        {
            Pco = Pco * 6 / CO2_CONC;
        }
        else
        {
            Pco = Pco;
        }
        if(F_DC == 1)
        {
            printf("男性稀释校正Pco=%f\r\n", Pco);
        }
    }
    else
    {
        if(CO2_CONC < 5.1)
        {
            Pco = Pco * 5.1 / CO2_CONC;

        }
        else if(CO2_CONC > 5.8)
        {
            Pco = Pco * 5.8 / CO2_CONC;
        }
        else
        {
            Pco = Pco;
        }
        if(F_DC == 1)
        {
            printf("女性稀释校正Pco=%f\r\n", Pco);
        }
    }

    if(Pco <= 0.0001)
    {
        if(F_PC == 1)
        {
            FS_TEST_ERRO();
        }
        temp[0] = 0x00;
        temp[1] = 0x3B;
        Wr_80_Scr(0x03, temp, 2); //	跳到59测试报错界面
        goto LAGE1;

    }
    temp1[0] = (uint32_t)(Pco * 10000) >> 24;
    temp1[1] = (uint32_t)(Pco * 10000) >> 16;
    temp1[2] = (uint32_t)(Pco * 10000) >> 8;
    temp1[3] = (uint32_t)(Pco * 10000);
    Wr_82_Scr(0x000F, temp1, 4);

    RBC = (uint16_t)((1.38 * HB) / Pco) ; // 计算出RBC
    if(F_DC == 1)
    {
        printf("RBC=%d\r\n", RBC);
    }
    temp[0] = RBC >> 8;
    temp[1] = RBC;
    Wr_82_Scr(0x0011, temp, 2);

    /*判断提示情况*/
    if(F_Zero_Error == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0082, temp, 2); //提示*号
    }
    if(F_Zero_Num != 0)
    {
        temp[0] = 0x00;
        temp[1] = 0x02;
        Wr_82_Scr(0x0082, temp, 2); //提示**号
    }
    if(F_CO2_Low == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0081, temp, 2); //提示*号

    }
    if(F_CO2_Num != 0)
    {
        temp[0] = 0x00;
        temp[1] = 0x02;
        Wr_82_Scr(0x0081, temp, 2); //提示**号
    }

    temp[0] = 0x00;
    temp[1] = 0x12;
    Wr_80_Scr(0x03, temp, 2); //	跳到18标准测量界面07

    if(RBC < 251)
    {
        temp[0] = 0x00;
        temp[1] = 0x13;
        Wr_80_Scr(0x03, temp, 2); //	跳到19标准测量界面08
    }
    else
    {
        temp[0] = 0x00;
        temp[1] = 0x47;
        Wr_80_Scr(0x03, temp, 2); //跳到71标准结果大于250天
    }
    while(F_PC == 1)
    {
        FS_RUN_OK();
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        if(F_Zero_Error == 1)
        {
            FS_Diff_ZERO(&Diff_Zero);
        }
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        if(F_Zero_Num != 0)
        {
            FS_ZERO_Fault(&Diff_Zero);
        }
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        if(F_CO2_Low == 1)
        {
            FS_CO2_WARNING();
        }
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        if((F_CO2_Num != 0) && (CO2_CONC < 7.0000))
        {
            FS_CO2_Fault();
        }
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        FS_SEX(&Sex);
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        FS_RBC(&RBC);
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        FS_Pco(&Pco);
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        FS_CO2(&CO2_CONC);
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        FS_HB(&HB);
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }
        break;
    }

LAGE1:
    Paiqi_RRR_PROM();
    if(F_PC == 1)
    {
        while(RevSDFlag != 1)
        {
            Delay1ms(1);
            nnnn++;
            if(nnnn > 5000)
            {
                break;
            }
        }
        if(RevSDFlag == 1)
        {
            RevSDFlag = 0;
            SD_SAVE();   ///SD卡解析保存
        }
    }
    Wr_80_Scr(0x02, &time, 1);
    while(1)
    {
        if(is_gasbag_exit(0) == 0)
        {
            if(F_PC == 1)
            {
                FS_GasNo(0);
                Delay10ms(1);
            }
            PC_ctr_Flag = 0; //PC控制标志初始化
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            if(F_PC == 1)
            {
                FS_GasNo(0);
                Delay10ms(1);
            }
            PC_ctr_Flag = 0; //PC控制标志初始化
            break;
        }

    }

    if((F_CO2_Num == 3) || (F_Zero_Num == 2)) ///如果CO2连续出现3次异常或者zero异常2次则直接拒绝下一步运行
    {
        if(F_CO2_Num == 3)
        {
            if(F_Zero_Num == 2)
            {
                temp[0] = 0x00;
                temp[1] = 0x52;
                Wr_80_Scr(0x03, temp, 2); //跳转至82CO和CO2测量系统故障界面

            }
            else
            {
                temp[0] = 0x00;
                temp[1] = 0x51;
                Wr_80_Scr(0x03, temp, 2); //跳转至81CO2测量系统故障界面
            }

        }
        else
        {
            temp[0] = 0x00;
            temp[1] = 0x50;
            Wr_80_Scr(0x03, temp, 2); //跳转至80CO测量系统故障界面
        }
        while(1);
    }

    /*参数归零*/
    HB = 0; //HB归零
    Zero_Num = 0;
    F_Zero_Error = 0;
    F_Timeout = 0; //超时变量归零
    F_CO2_Low = 0;
    temp[0] = 0x00;
    temp[1] = 0x00;
    temp1[0] = 0x00;
    temp1[1] = 0x00;
    temp1[2] = 0x00;
    temp1[3] = 0x00;
    Wr_82_Scr(0x000E, temp, 2);
    Wr_82_Scr(0x0060, temp, 2);
    Wr_82_Scr(0x000F, temp1, 4);
    Wr_82_Scr(0x0022, temp, 2);
    Wr_82_Scr(0x0011, temp, 2);
    Wr_82_Scr(0x0080, temp, 2);
    Wr_82_Scr(0x0081, temp, 2);
    Wr_82_Scr(0x0082, temp, 2);

    temp[0] = 0x00;
    temp[1] = 0x0B;
    Wr_80_Scr(0x03, temp, 2); //返回11标准测量1界面
}
/**
  *@breif  精确测量主程序
	*@param  none
	*@retval none
*/
void Adj_RUN_Program(void)
{
    uint8_t i;
    uint16_t CO2_AD, CO2_ZTest; // 实测CO2台阶数 以及测试CO2零点
    float CO2_CONC; // CO2浓度
    uint16_t CO_ZERO1;
    uint16_t CO_ZERO2;
    uint16_t CO_AD1;
    uint16_t CO_AD2;
    float CO_Cha[2];
    float CO_Cha_Ave;
    float CO_AD2_Zsub[2];
    float CO_AD1_Zsub[2];
    float PcoR, Pco; // CO实测值
    uint16_t RBC;
    uint8_t temp[2];
    uint8_t temp1[4];
    uint8_t m;
    uint16_t Diff_ZERO;
    uint16_t nnnn;

    /*测量样气的CO2*/
    if(F_PC == 1)
    {
        FS_RUN_step1_ON();
    }
    temp[0] = 0x00;
    temp[1] = 0x16;
    Wr_80_Scr(0x03, temp, 2); //跳到22精确测量02界面
    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity); //测量温度和气压
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x03, temp, 2);
    Purge_Cylinder();
    Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);// 清洗CO2气室并测试零点
    Paiqi_RR_PROM();// 清洗CO2气室10 S
    CO2_CONC = (float)(CO2_AD - CO2_ZTest) / CO2_Taijie;
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    if(F_DC == 1)
    {
        printf("气压校正系数=%d\r\n", ATM_Coeff);
        printf("气压=%f百帕\r\n", BME_Pressure);
        printf("温度=%f摄氏度\r\n", BME_Temperature);
        printf("CO2零点值=%d,CO2电平值=%d\r\n", CO2_ZTest, CO2_AD);
        printf("CO2浓度M/V=%f\r\n", CO2_CONC);
    }
    CO2_CONC = CO2_CONC * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / 301;
    if(F_DC == 1)
    {
        printf("CO2浓度V/V=%f\r\n", CO2_CONC);
    }
    temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
    temp[1] = (uint16_t)(CO2_CONC * 100);
    Wr_82_Scr(0x0022, temp, 2);

    if(CO2_CONC < 0.50) //实测值小于0.5
    {
        temp[0] = 0x00;
        temp[1] = 0x3C;
        Wr_80_Scr(0x03, temp, 2); //跳到60CO2浓度过低错误界面
        if(F_PC == 1)
        {
            FS_CO2_ERRO();
        }
        goto ADJ_LAGE1;
    }
    if((CO2_CONC < 2.0000) || (CO2_CONC >= 7.0000))
    {
        F_CO2_Num++;
        if(F_DC == 1)
        {
            printf("F_CO2_Num=%d\r\n", F_CO2_Num);
        }
    }
    else if(CO2_CONC < 3.0000)
    {
        F_CO2_Low = 1; //二氧化碳过低标志
        F_CO2_Num = 0;
    }
    else
    {
        F_CO2_Num = 0;
    }
    temp[0] = 0x00;
    temp[1] = 0x17;
    Wr_80_Scr(0x03, temp, 2);	//跳到23精确测量03界面
    if(F_PC == 1)
    {
        FS_RUN_step1_OK();
    }

    /*开始CO测量*/
    for(m = 0; m < 2; m++)
    {
        if(F_PC == 1)
        {
            FS_RUN_step2_ON();
        }

        temp[0] = 0x00;
        temp[1] = 0x18;
        Wr_80_Scr(0x03, temp, 2); //跳到24精确测量04界面
        /*测量底气CO台阶*/
        Purge_Cylinder();
        Paiqi_R_PROM();//清洗CO气室100S
        ADC(&CO_ZERO1); //测B气CO零点ZERO1
        for(i = 0; i < 2; i++)
        {
            Chou_BGas();// 从环境倒气袋抽环境气2次E5->E1
        }
        Daiji(30);// 倒计时30S
        ADC(&CO_AD1); //  测量环境气B气的台阶
        if(F_DC == 1)
        {
            printf(" 第%d次底气零点=%d\r\n", m + 1, CO_ZERO1);
            printf(" 第%d次底气电平=%d\r\n", m + 1, CO_AD1);
        }
        if(F_PC == 1)
        {
            FS_RUN_step2_OK();
        }
        temp[0] = 0x00;
        temp[1] = 0x19;
        Wr_80_Scr(0x03, temp, 2); //跳到25精确测量05界面

        /*测量CO台阶*/
        if(F_PC == 1)
        {
            FS_RUN_step3_ON();
        }
        temp[0] = 0x00;
        temp[1] = 0x1A;
        Wr_80_Scr(0x03, temp, 2); //	跳到26精确测量06界面

        Paiqi_R_PROM( );//清洗CO气室100S
        ADC(&CO_ZERO2); //测Y气CO零点ZERO1
        if(F_DC == 1)
        {
            printf(" 第%d次样气零点%d\r\n", m + 1, CO_ZERO2);
        }
        //零点错误判定
        Diff_ZERO = abs(CO_ZERO2 - CO_ZERO1);
        if(Diff_ZERO > 10)
        {
            if(Diff_ZERO > 20)
            {
                temp[0] = Diff_ZERO >> 8;
                temp[1] = Diff_ZERO;
                Wr_82_Scr(0x0035, temp, 2); //将零点写入到报错界面
                temp[0] = 0x00;
                temp[1] = 0x36;
                Wr_80_Scr(0x03, temp, 2); //	跳到54零点报错界面
                if(F_PC == 1)
                {
                    FS_ZERO_ERRO();
                }
                goto ADJ_LAGE1;
            }
            else
            {
                F_Zero_Error = 1; //结果标记
            }

        }

        for(i = 0; i < 2; i++)
        {
            Chou_YGas();// 从样气倒气袋抽环境气2次
        }

        Daiji(30);// 倒计时30S
        ADC(&CO_AD2); // 测量Y气的台阶
        if(F_DC == 1)
        {
            printf("第%d次样气电平=%d\r\n", m + 1, CO_AD2);
        }
        CO_AD2_Zsub[m] = CO_AD2 - CO_ZERO2;
        if(CO_AD2_Zsub[m] <= 0.00001)
        {
            CO_AD2_Zsub[m] = 0.0;
        }
        CO_AD1_Zsub[m] = CO_AD1 - CO_ZERO1;
        if(CO_AD1_Zsub[m] <= 0.00001)
        {
            CO_AD1_Zsub[m] = 0.0;
        }
        CO_Cha[m] = CO_AD2 - CO_AD1;
        Paiqi_RRR_PROM();

    }
    CO_Cha_Ave = (CO_Cha[0] + CO_Cha[1]) / 2;
    PcoR = CO_Cha_Ave * R_XB / 10000.0; // ppm  该数值是M/V模式
    Pco = PcoR * ATM_Coeff * 0.0001 * (273 + BME_Temperature) / (273 + T0); //进行气压和温度校正
    if(F_DC == 1)
    {
        printf("CO_Cha=%f\r\n", CO_Cha_Ave);
        printf(" M/V校正前 PcoR=%f\r\n", PcoR);
        printf("气压和温度校正后Pco=%f\r\n", Pco);
    }
    Pco = Pco * (1 - CO2_CONC * 0.01) - CO2_CONC * 0.01 * R_XB / 10000 * ((CO_AD1_Zsub[0] + CO_AD1_Zsub[1]) / 2); //进行CO2吸收浓缩校正
    if(F_DC == 1)
    {
        printf("CO2浓缩校正后Pco=%f\r\n", Pco);
    }
    if(F_PC == 1)
    {
        FS_RUN_step3_OK();
    }

    /*稀释校正 分男女差异*/
    if(Sex == 1)
    {
        if(CO2_CONC < 5.3)
        {
            Pco = Pco * 5.3 / CO2_CONC;

        }
        else if(CO2_CONC > 6.0)
        {
            Pco = Pco * 6 / CO2_CONC;
        }
        else
        {
            Pco = Pco;
        }
        if(F_DC == 1)
        {
            printf("男性稀释校正Pco=%f\r\n", Pco);
        }
    }
    else
    {
        if(CO2_CONC < 5.1)
        {
            Pco = Pco * 5.1 / CO2_CONC;

        }
        else if(CO2_CONC > 5.8)
        {
            Pco = Pco * 5.8 / CO2_CONC;
        }
        else
        {
            Pco = Pco;
        }
        if(F_DC == 1)
        {
            printf("女性稀释校正Pco=%f\r\n", Pco);
        }
    }
    if(Pco <= 0.0001)
    {
        if(F_PC == 1)
        {
            FS_TEST_ERRO();
        }
        temp[0] = 0x00;
        temp[1] = 0x3B;
        Wr_80_Scr(0x03, temp, 2); //	跳到59测试报错界面
        goto ADJ_LAGE1;

    }
    temp1[0] = (uint32_t)(Pco * 10000) >> 24;
    temp1[1] = (uint32_t)(Pco * 10000) >> 16;
    temp1[2] = (uint32_t)(Pco * 10000) >> 8;
    temp1[3] = (uint32_t)(Pco * 10000);
    Wr_82_Scr(0x000F, temp1, 4);
    // 根据公式RBC=(HB*1.38)/PCO 计算得到，正常人（RBC=120天）
    RBC = (uint16_t)((1.38 * HB) / Pco) ; // 计算出RBC
    if(F_DC == 1)
    {
        printf("RBC=%d\r\n", RBC);
    }
    temp[0] = RBC >> 8;
    temp[1] = RBC;
    Wr_82_Scr(0x0011, temp, 2);

    if(F_Zero_Error == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0082, temp, 2); //提示*号
    }

    if(F_CO2_Low == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0081, temp, 2); //提示*号

    }
    if(F_CO2_Num != 0)
    {
        temp[0] = 0x00;
        temp[1] = 0x02;
        Wr_82_Scr(0x0081, temp, 2); //提示**号
    }

    temp[0] = 0x00;
    temp[1] = 0x1B;
    Wr_80_Scr(0x03, temp, 2); //	跳到27精确测量界面07

    if(RBC < 251)
    {
        temp[0] = 0x00;
        temp[1] = 0x1C;
        Wr_80_Scr(0x03, temp, 2); //	跳到28精确测量界面08
    }
    else
    {
        temp[0] = 0x00;
        temp[1] = 0x46;
        Wr_80_Scr(0x03, temp, 2); //跳到70精确测量界面结果大于250
    }
    if(F_PC == 1)
    {
        FS_RUN_OK();

        FS_RBC(&RBC);

        FS_Pco(&Pco);

        FS_CO2(&CO2_CONC);

        FS_HB(&HB);
    }


ADJ_LAGE1:
    Paiqi_RRR_PROM();
    if(F_PC == 1)
    {
        while(RevSDFlag != 1)
        {
            Delay1ms(1);
            nnnn++;
            if(nnnn > 5000)
            {
                break;
            }
        }
        if(RevSDFlag == 1)
        {
            RevSDFlag = 0;
            SD_SAVE();   ///SD卡解析保存
        }
    }
    Wr_80_Scr(0x02, &time, 1);

    while(1)
    {
        if(is_gasbag_exit(0) == 0)
        {
            if(F_DC == 1)
            {
                FS_GasNo(0);
            }
            PC_ctr_Flag = 0; //PC控制标志初始化
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            if(F_DC == 1)
            {
                FS_GasNo(0);
            }
            PC_ctr_Flag = 0; //PC控制标志初始化
            break;
        }

    }
    if((F_CO2_Num == 3) || (F_Zero_Num == 2)) ///如果CO2连续出现3次异常
    {
        if(F_CO2_Num == 3)
        {
            if(F_Zero_Num == 2)
            {
                temp[0] = 0x00;
                temp[1] = 0x52;
                Wr_80_Scr(0x03, temp, 2); //跳转至82CO和CO2测量系统故障界面

            }
            else
            {
                temp[0] = 0x00;
                temp[1] = 0x51;
                Wr_80_Scr(0x03, temp, 2); //跳转至81CO2测量系统故障界面
            }

        }
        else
        {
            temp[0] = 0x00;
            temp[1] = 0x50;
            Wr_80_Scr(0x03, temp, 2); //跳转至80CO测量系统故障界面
        }
        while(1);
    }
    HB = 0; //HB归零
    F_Timeout = 0; //超时变量归零
    F_CO2_Low = 0;
    temp[0] = 0x00;
    temp[1] = 0x00;
    temp1[0] = 0x00;
    temp1[1] = 0x00;
    temp1[2] = 0x00;
    temp1[3] = 0x00;
    Wr_82_Scr(0x000E, temp, 2);
    Wr_82_Scr(0x000A, temp, 2);
    Wr_82_Scr(0x0060, temp, 2);
    Wr_82_Scr(0x000F, temp1, 4);
    Wr_82_Scr(0x0022, temp, 2);
    Wr_82_Scr(0x0011, temp, 2);
    Wr_82_Scr(0x0081, temp, 2);
    Wr_82_Scr(0x0082, temp, 2);
    temp[0] = 0x00;
    temp[1] = 0x14;
    Wr_80_Scr(0x03, temp, 2); //返回20精确测量00界面

}
/**
  *@breif  质控流程
	*@param  none
	*@retval none
*/
void QC_PROM(void)
{
    uint16_t QCCO2_AD, QC_ZTest ; // 样品气的电平数
    uint8_t i, n;
    uint8_t m = 0;
    uint8_t temp[2];
    uint8_t Zero_Num = 0; //出现大于20的次数
    uint8_t sendNum = 0;
    uint16_t CO_ZERO1, CO_ZERO2, CO_AD1, CO_AD2; //CO_ZERO_V 动态零点
    uint16_t R_CO_Precision, R_CO_Accuracy; //最终结果值
    uint16_t runTimeOut = 0;
    float CO_AD2_Z;
    float A_CO2_MV[3], A_CO2_VV[3], SUM_ACO2_VV; //CO2浓度，主要是M/V和V/V两种
    float PcoR[3], Pco[3], CO_Cha;
    float CO2_Precision, CO2_Accuracy; //CO2测量的精密度和准确度
    float Diff_ZERO;
    float Pco_Temp[3];//用来盛放筛选后的Pco值
    float CO_CONC_ACT;//CO实际输入的值*100

    while(!is_gasbag_exit(0));
    if(F_PC == 1)
    {
        Delay10ms(5);
        FS_GasYes(0);//发送第一个气袋插入到位
        FS_GasYes(5);//发送全部插到位
        Delay10ms(1);
        if(F_PC == 0)
        {
            USART1_Timeout_UserCallback();
            PC_ctr_Flag = 0; //上位机控制下位机标志归零
        }
        if(PC_ctr_Flag == 1)
        {
            while(F_PC)
            {
                runTimeOut++;
                if(ReceiveFlag6 == 1)
                {
                    ReceiveFlag6 = 0;
                    PC_Val_Analy();
                }
                if(F_CO2 == 1) ///收到上位机软件发送 20190318
                {
                    F_CO2 = 0;
                    Delay10ms(5);
                    break;
                }
                Delay10ms(1);
                if(runTimeOut == 1000) //超时次数达到
                {
                    runTimeOut = 0; //初始化
                    sendNum++;
                    if(sendNum < 3) //冗余设计
                    {
                        RBCtoPC_Cmd_FF();
                        continue;
                    }
                    else
                    {
                        USART1_Timeout_UserCallback();
                        PC_ctr_Flag = 0; //上位机控制下位机标志归零
                        break;
                    }
                }
            }
            CO2_B = QC_CO2_CONC;
            CO_CONC_ACT = QC_CONC;
        }
    }

    /*开始质控*/
    temp[0] = 0x00;
    temp[1] = 0x26;
    Wr_80_Scr(0x03, temp, 2); //跳到38质控01界面
    Delay1ms(1);
    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
    temp[0] = Ring_Num >> 8;
    temp[1] = Ring_Num;
    Wr_82_Scr(0x0009, temp, 2);
    if(PC_ctr_Flag == 0)
    {
        /*读取输入值*/
        Rd_83_Scr(0x0013, 1); //读取标准气CO浓度
        Delay1ms(5);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x13) && (Com3RBuf[6] == 0x01))
        {
            Cal_CO_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            if(F_DC == 1)
            {
                printf("测试气CO浓度差值=%f\r\n", Cal_CO_A * 0.0001);
            }
        }
        CO_CONC_ACT = Cal_CO_A * 0.01;

        Rd_83_Scr(0x0012, 1); //读取标准气CO2浓度
        Delay1ms(5);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
        {
            CO2_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            if(F_DC == 1)
            {
                printf("测试气ACO2标称值%d\r\n", CO2_B);
            }
        }
    }
    else
    {
        temp[0] = (uint32_t)(CO_CONC_ACT * 100) >> 8;
        temp[1] = (uint32_t)(CO_CONC_ACT * 100);
        Wr_82_Scr(0x0013, temp, 2);

        temp[0] = (uint16_t)CO2_B >> 8;
        temp[1] = (uint16_t)CO2_B;
        Wr_82_Scr(0x0012, temp, 2);

    }
    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x000A, temp, 2);
    if(F_DC == 1)
    {
        printf("气压=%f百帕\r\n", BME_Pressure);
        printf("温度=%f摄氏度\r\n", BME_Temperature);
        printf("气压校正系数=%d\r\n", ATM_Coeff);
    }
    if(F_PC == 1)
    {
        Delay1ms(5);
        FS_QC_ON();
    }
    DaoQi_Z(150);//通过隔膜泵采集零气至本底气中

    for(i = 0; i < 3; i++)
    {
        temp[0] = 0x00;
        temp[1] = 0x27;
        Wr_80_Scr(0x03, temp, 2); //跳到39质控02界面
        if(F_PC == 1)
        {
            switch(i)
            {
            case 0:
                FS_QC_step1_ON();
                break;
            case 1:
                FS_QC_step2_ON();
                break;
            case 2:
                FS_QC_step3_ON();
                break;
            default:
                break;
            }
        }
        Purge_Cylinder();//清洗气缸

        /*CO2测量*/
        Celiang_ZhunBeiNew(&QC_ZTest, &QCCO2_AD);// 清洗CO2气室并测试CO2电平
        if(F_DC == 1)
        {
            printf("第%d次CO2测试零点=%d,电平=%d\r\n", i + 1, QC_ZTest, QCCO2_AD);
        }
        A_CO2_MV[i] = (float)(QCCO2_AD - QC_ZTest) / CO2_Taijie;
        if(A_CO2_MV[i] <= 0.0001)
        {
            A_CO2_MV[i] = 0.0;
        }
        if(F_DC == 1)
        {
            printf("第%d次CO2_MV测试浓度=%f\r\n", i + 1, A_CO2_MV[i]);
        }
        A_CO2_VV[i] = ATM_Coeff * 0.0001 * A_CO2_MV[i] * (BME_Temperature + 273) / (T0 + 273);
        if(F_DC == 1)
        {
            printf("第%d次CO2_VV浓度=%f\r\n", i + 1, A_CO2_VV[i]);
        }
        SUM_ACO2_VV += A_CO2_VV[i];

        /*CO测量*/
        temp[0] = 0x00;
        temp[1] = 0x28;
        Wr_80_Scr(0x03, temp, 2); //跳到40质控03界面
        Purge_Cylinder();//清洗气缸
        Paiqi_R_PROM();//清洗CO气室100S
        ADC(&CO_ZERO1); //测B气CO零点
        if(F_DC == 1)
        {
            printf("第%d次底气零点值=%d\r\n", i + 1, CO_ZERO1);
        }
        for(n = 0; n < 2; n++)
        {
            Chou_BGas();
        }
        Daiji(30);
        ADC(&CO_AD1);//测量环境气的台阶
        if(F_DC == 1)
        {
            printf("第%d次底气电平值=%d\r\n", m + 1, CO_AD1);
        }

        Paiqi_R_PROM();//清洗气室100s
        ADC(&CO_ZERO2); //Y气CO零点
        if(F_DC == 1)
        {
            printf("第%d次样气零点值=%d\r\n", i + 1, CO_ZERO2);
        }

        Diff_ZERO = abs(CO_ZERO2 - CO_ZERO1);//零点错误判定
        if(Diff_ZERO > 10)
        {
            if(Diff_ZERO > 20)
            {
                if(Zero_Num == 2)
                {
                    temp[0] = (uint16_t)Diff_ZERO >> 8;
                    temp[1] = (uint16_t)Diff_ZERO;
                    Wr_82_Scr(0x0035, temp, 2); //将零点写入到报错界面
                    temp[0] = 0x00;
                    temp[1] = 0x45;
                    Wr_80_Scr(0x03, temp, 2); //	跳到69零点报错界面
                    if(F_PC == 1)
                    {
                        FS_QC_RUN_ERR2();
                    }
                    goto QC_LAGE1;
                }
                else
                {
                    Zero_Num++;
                    Daiji(180);
                    Pco[i] = 0.0;
                    goto QC_LAGE2;
                }
            }
            else
            {
                F_Zero_Error = 1; //结果标记
            }

        }
        temp[0] = 0x00;
        temp[1] = 0x29;
        Wr_80_Scr(0x03, temp, 2); //返回41质控04界面

        for(n = 0; n < 2; n++)
        {
            Chou_YGas();// 从肺泡气2抽取气体2次
        }
        Daiji(30);// 倒计时30S
        ADC(&CO_AD2); // 测量Y气的台阶
        if(F_DC == 1)
        {
            printf(" 第%d次样气电平值=%d\r\n", i + 1, CO_AD2);
        }

        CO_AD2_Z = CO_AD2 - CO_ZERO2;
        if(CO_AD2_Z <= 0.00001)
        {
            CO_AD2_Z = 0.0;
        }

        CO_Cha = CO_AD2 - CO_AD1;
        if(F_DC == 1)
        {
            printf("第%d次电平差=%f\r\n", i + 1, CO_Cha);
        }

        PcoR[i] = (float)R_XB / 10000 * CO_Cha; // ppm


        Pco[i] = PcoR[i] * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / (T0 + 273) * (1 - A_CO2_VV[i] / 100); // 稀释校正Pco
        if(F_DC == 1)
        {
            printf("第%d次校正前pcoR=%f\r\n", i + 1, PcoR[i]);
            printf("第%d次校正后 Pco=%f\r\n", i + 1, Pco[i]);
        }
        if(Pco[i] <= 0.0001)
        {
            temp[0] = 0x00;
            temp[1] = 0x3B;
            Wr_80_Scr(0x03, temp, 2); //	跳到59测试报错界面
            goto QC_LAGE1;
        }
QC_LAGE2:
        if(i < 2)
        {
            Paiqi_RRR_PROM();
            Wr_80_Scr(0x02, &time, 1); //蜂鸣器响
            temp[0] = 0x00;
            temp[1] = 0x4D;
            Wr_80_Scr(0x03, temp, 2); //跳到77插入气袋提示界面
            while(is_gasbag_exit(0));
            Delay100ms(10);//延时防止拔袋子过程中电平波动
            while(!is_gasbag_exit(0));
        }
        if(F_PC == 1)
        {
            switch(m)
            {
            case 0:
                FS_QC_step1_OK();
                break;
            case 1:
                FS_QC_step2_OK();
                break;
            case 2:
                FS_QC_step3_OK();
                break;
            default:
                break;
            }
        }
    }
    CO2_Accuracy = 100 * StandardDeviation(A_CO2_VV, 3, 0, CO2_B); //准确度计算  结果是小数值*10  例如结果是1.0%，则数值结果是10
    CO2_Precision = 100 * StandardDeviation(A_CO2_VV, 3, 1, CO2_B); //精密度计算
    if(F_DC == 1)
    {
        printf("CO2准确度为：%f\r\n", CO2_Accuracy);
        printf("CO2精密度为：%f\r\n", CO2_Precision);
    }
    if((CO2_Accuracy > 4) || (CO2_Precision > 4))
    {
        temp[0] = (uint16_t)CO2_Precision >> 8;
        temp[1] = (uint16_t)CO2_Precision;
        Wr_82_Scr(0x0033, temp, 2);
        temp[0] = (uint16_t)CO2_Accuracy >> 8;
        temp[1] = (uint16_t)CO2_Accuracy;
        Wr_82_Scr(0x0034, temp, 2);
        temp[0] = 0x00;
        temp[1] = 0x43;
        Wr_80_Scr(0x03, temp, 2); //跳到67CO2精密度和准确度未达标界面
        if(F_PC == 1)
        {
            FS_QC_RUN_ERR1();
        }
        goto QC_LAGE1;
    }

    for(n = 0; n < 3; n++)
    {
        if(Pco[n] > 0.1)
        {
            Pco_Temp[m] = Pco[n];
            m++;
        }
    }
    R_CO_Precision = (uint16_t)(100 * StandardDeviation(Pco_Temp, m, 1, CO_CONC_ACT)); //精密度计算，该数值单位是%
    R_CO_Accuracy = (uint16_t)(100 * StandardDeviation(Pco_Temp, m, 0, CO_CONC_ACT)); //准确度计算
    if(F_DC == 1)
    {
        printf("CO精密度*100=%d\r\n", R_CO_Precision);
        printf("CO准确度*100=%d\r\n", R_CO_Accuracy);
    }
    temp[0] = R_CO_Precision >> 8;
    temp[1] = R_CO_Precision;
    Wr_82_Scr(0x001C, temp, 4);
    temp[0] = R_CO_Accuracy >> 8;
    temp[1] = R_CO_Accuracy;
    Wr_82_Scr(0x0032, temp, 2);

    if((R_CO_Precision <= 10) && (R_CO_Accuracy <= 10))
    {
        temp[0] = ATM_Coeff >> 8;
        temp[1] = ATM_Coeff;
        Wr_82_Scr(0x0030, temp, 2);
        if(F_PC == 1)
        {
            FS_RA(&R_CO_Precision);
            Delay10ms(1);
            FS_RP(&R_CO_Accuracy);
            Delay10ms(1);
        }
        temp[0] = 0x00;
        temp[1] = 0x2B;
        Wr_80_Scr(0x03, temp, 2); //返回43质控通过界面
        if(F_Zero_Error == 1)
        {
            temp[0] = 0x00;
            temp[1] = 0x01;
            Wr_82_Scr(0x0080, temp, 2);
            if(F_PC == 1)
            {
                FS_QC_RS_CRI_OK();//显示质控*
            }
        }
        else
        {
            if(F_PC == 1)
            {
                FS_QC_RS_OK();//显示质控通过
            }
        }

    }
    else
    {
        temp[0] = 0x00;
        temp[1] = 0x2A;
        Wr_80_Scr(0x03, temp, 2); //返回42质控未通过界面
        if(F_PC == 1)
        {
            FS_RA(&R_CO_Precision);
            Delay10ms(1);
            FS_RP(&R_CO_Accuracy);
            Delay10ms(1);
            FS_QC_RS_ERR();//显示质控未通过
        }
    }
QC_LAGE1:
    if(F_PC == 1)
    {
        Delay10ms(10);
        FS_QC_OK();
    }
    Paiqi_RRR_PROM();
    Wr_80_Scr(0x02, &time, 1); //蜂鸣器响

    while(1)
    {

        if(is_gasbag_exit(0) == 0)
        {
            if(F_PC == 1)
            {
                FS_GasNo(0);
                Delay1ms(5);
                FS_QC_BACK();
                Delay1ms(5);
            }
            temp[0] = 0x00;
            temp[1] = 0x25;
            Wr_80_Scr(0x03, temp, 2); //返回37质控00界面
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            if(F_PC == 1)
            {
                FS_GasNo(0);
                Delay1ms(5);
                FS_QC_BACK();
                Delay1ms(5);
            }
            break;
        }

    }
    PC_ctr_Flag = 0; //PC控制标志初始化
    F_Zero_Error = 0;
    temp[0] = 0x00;
    temp[1] = 0x00;
    Wr_82_Scr(0x0012, temp, 2);
    Wr_82_Scr(0x0013, temp, 2);
    Wr_82_Scr(0x000A, temp, 2);
    Wr_82_Scr(0x0032, temp, 2);
    Wr_82_Scr(0x001C, temp, 2);
    Wr_82_Scr(0x0080, temp, 2);
}

/**
  *@breif  CO标定流程
	*@param  none
	*@retval none
*/
void Curve_PROM(void)
{
    uint16_t CO_ZERO1, CO_ZERO2, CO_AD2, CO_ZERO_DIF, CO_AD1;
    uint16_t Cal_CO2_A;
    uint16_t Curve_CONC_MV;//标定CO的M/V浓度
    uint8_t i, m;
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};
    uint16_t CO2_ZTest, CO2_AD;
    float CO2_CONC_MV[3], CO2_CONC_VV[3];
    float CO2_CONC_MV_SUM = 0;
    float CO2_CONC_VV_SUM = 0;
    float CO_cha[3];
    float Sum_CO_cha = 0;
    float CO2_Accuracy, DCO_Precision;


    while(!is_gasbag_exit(0));
    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);

    temp[0] = Ring_Num >> 8;
    temp[1] = Ring_Num;
    Wr_82_Scr(0x0009, temp, 2);
    temp[0] = 0x00;
    temp[1] = 0x23;
    Wr_80_Scr(0x03, temp, 2); //进入35CO标定01界面
    Delay1ms(5);

    Rd_83_Scr(0x0013, 1); //读取测试气CO浓度差值
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x13) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        if(F_DC == 1)
        {
            printf("测试气CO浓度差值_V/V=%f\r\n", Cal_CO_A * 0.0001);
        }
    }

    Rd_83_Scr(0x0012, 1); //读取标准气CO2标称值
    Delay1ms(1);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO2_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        if(F_DC == 1)
        {
            printf("测试气A CO2标称值=%f\r\n", Cal_CO2_A * 0.01);
        }
    }

    Curve_CONC = Cal_CO_A * 0.01; //扩大了100倍

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    if(F_DC == 1)
    {
        printf("气压=%f百帕\r\n", BME_Pressure);
        printf("温度=%f摄氏度\r\n", BME_Temperature);
        printf("气压修正系数=%d\r\n", ATM_Coeff);
    }

    DaoQi_Z(150);//通过隔膜泵采集零气至倒气袋中

    /*开始测量*/
    for(i = 0; i < 3; i++)
    {
        temp[0] = 0x00;
        temp[1] = 0x23;
        Wr_80_Scr(0x03, temp, 2); //进入35CO标定01界面
        Purge_Cylinder();//清洗气缸

        /*CO2测量*/
        Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);// 清洗CO2气室并测试CO2电平
        if(F_DC == 1)
        {
            printf("第%d次CO2测试零点=%d,电平=%d\r\n", i + 1, CO2_ZTest, CO2_AD);
        }
        CO2_CONC_MV[i] = (float)(CO2_AD - CO2_ZTest) / CO2_Taijie;
        if(CO2_CONC_MV[i] <= 0.0001)
        {
            CO2_CONC_MV[i] = 0.0;
        }
        if(F_DC == 1)
        {
            printf("第%d次CO2_MV测试浓度=%f\r\n", i + 1, CO2_CONC_MV[i]);
        }
        CO2_CONC_MV_SUM += CO2_CONC_MV[i];
        CO2_CONC_VV[i] = ATM_Coeff * 0.0001 * CO2_CONC_MV[i] * (BME_Temperature + 273) / (T0 + 273);
        if(F_DC == 1)
        {
            printf("第%d次CO2_VV浓度=%f\r\n", i + 1, CO2_CONC_VV[i]);
        }
        CO2_CONC_VV_SUM += CO2_CONC_VV[i];

        /*本底气测试*/
        Purge_Cylinder();//清洗气缸
        Paiqi_R_PROM();//  100S排气清洗气室
        ADC(&CO_ZERO1); // 测量B气的零点
        if(F_DC == 1)
        {
            printf("第%d次底气零点=%d\r\n", i + 1, CO_ZERO1);
        }
        for(m = 0; m < 2; m++)
        {
            Chou_BGas();// 从环境倒气袋抽环境气2次E6->E1
        }
        Daiji(30);// 倒计时30S
        ADC(&CO_AD1); //  测量环境气B气的台阶
        if(F_DC == 1)
        {
            printf("第%d次底气电平值=%d\r\n", i + 1, CO_AD1);
        }

        /*样气测试*/
        Paiqi_R_PROM();//清洗CO气室100S
        ADC(&CO_ZERO2); // 测量Y气的台阶
        if(F_DC == 1)
        {
            printf("第%d次样气零点=%d\r\n", i + 1, CO_ZERO2);
        }
        CO_ZERO_DIF = abs(CO_ZERO2 - CO_ZERO1);//零点错误判定
        if(CO_ZERO_DIF > 10)
        {
            temp[0] = CO_ZERO_DIF >> 8;
            temp[1] = CO_ZERO_DIF;
            Wr_82_Scr(0x0002, temp, 2);
            temp[0] = 0x00;
            temp[1] = 0x37;
            Wr_80_Scr(0x03, temp, 2); //跳到55界面
            goto LAGE_CURVE;
        }
        for(m = 0; m < 2; m++)
        {
            Chou_YGas();// 样气抽取2次
        }
        Daiji(30);//静置30S
        ADC(&CO_AD2);
        CO_cha[i] = CO_AD2 - CO_AD1;
        if(F_DC == 1)
        {
            printf("第%d次样气电平=%d\r\n", i + 1, CO_AD2);
            printf("第%d次样气电平差=%f\r\n", i + 1, CO_cha[i]);
        }
        Sum_CO_cha += CO_cha[i];

        if(i < 2)
        {
            Paiqi_RRR_PROM();
            Wr_80_Scr(0x02, &time, 1); //蜂鸣器响
            temp[0] = 0x00;
            temp[1] = 0x4D;
            Wr_80_Scr(0x03, temp, 2); //跳到77插入气袋提示界面
            while(is_gasbag_exit(0));
            Delay100ms(2);//延时防止拔袋子过程中电平波动
            while(!is_gasbag_exit(0));
        }

    }

    /*统计测量数据*/
    CO2_Accuracy = 1000 * StandardDeviation(CO2_CONC_VV, 3, 0, Cal_CO2_A); //准确度计算
    if(F_DC == 1)
    {
        printf("CO2准确度为=%f\r\n", CO2_Accuracy * 0.1);
    }
    if(CO2_Accuracy > 40)
    {
        temp[0] = (uint16_t)CO2_Accuracy >> 8;
        temp[1] = (uint16_t)CO2_Accuracy;
        Wr_82_Scr(0x0033, temp, 2);
        Wr_82_Scr(0x0034, temp, 2);
        temp[0] = 0x00;
        temp[1] = 0x42;
        Wr_80_Scr(0x03, temp, 2); //跳到66标定未完成界面
        goto LAGE_CURVE;
    }

    DCO_Precision = 1000 * StandardDeviation(CO_cha, 3, 1, 1); //精密度计算
    if(F_DC == 1)
    {
        printf("CO电平精密度=%f\r\n", DCO_Precision * 0.1);
    }
    if(DCO_Precision > 100)
    {
        temp[0] = (uint16_t)DCO_Precision >> 8;
        temp[1] = (uint16_t)DCO_Precision;
        Wr_82_Scr(0x0036, temp, 2);
        temp[0] = 0x00;
        temp[1] = 0x44;
        Wr_80_Scr(0x03, temp, 2); //跳到68标定未完成界面
        goto LAGE_CURVE;
    }
    Curve_CONC_MV = Curve_CONC * 10000.0 / ATM_Coeff * (273 + T0) / (273 + BME_Temperature);    //求MV形式的CO浓度
    Curve_CONC_MV = Curve_CONC_MV / (1 - CO2_CONC_VV_SUM / 300);

    R_XB = 10000 * Curve_CONC_MV * 0.03 / Sum_CO_cha;
    if(F_DC == 1)
    {

        printf("M/V形式CO标准气浓度差=%f\r\n", Curve_CONC_MV * 0.01);
        printf("吸收校正M/V形式CO标准气浓度差=%f\r\n", Curve_CONC_MV * 0.01);
        printf("CO标定系数=%d\r\n", R_XB);
    }
    ee_WriteBytes((uint8_t *)&R_XB, 4, 2);
    ee_WriteBytes((uint8_t *)&ATM_Coeff, 14, 2);
    temp[0] = ATM_Coeff >> 8;
    temp[1] = ATM_Coeff;
    Wr_82_Scr(0x0030, temp, 2);
    temp[0] = R_XB >> 8;
    temp[1] = R_XB;
    Wr_82_Scr(0x0005, temp, 2);
    temp[0] = 0x00;
    temp[1] = 0x24;
    Wr_80_Scr(0x03, temp, 2);

LAGE_CURVE:
    Paiqi_RRR_PROM();
    Wr_80_Scr(0x02, &time, 1); //蜂鸣器响
    while(1)
    {
        if(is_gasbag_exit(0) == 0)
        {
            temp[0] = 0x00;
            temp[1] = 0x22;
            Wr_82_Scr(0x0013, temp1, 2);
            Wr_82_Scr(0x0012, temp1, 2);
            Wr_82_Scr(0x0031, temp1, 2);
            Wr_82_Scr(0x000A, temp1, 2);
            Wr_80_Scr(0x03, temp, 2);
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            temp[0] = 0x00;
            temp[1] = 0x22;
            Wr_82_Scr(0x0031, temp1, 2);
            Wr_82_Scr(0x000A, temp1, 2);
            Wr_80_Scr(0x03, temp, 2);
            break;
        }
    }
}

/**
  *@breif  配气流程
  *@param  none
  *@retval none
*/
void PeiGas_PROM(void)
{
    uint8_t i;
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};
    while(!is_gasbag_exit(0));
    temp[0] = 0x00;
    temp[1] = 0x30;
    Wr_80_Scr(0x03, temp, 2); //进入48配气01界面
    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180228-lcy
    Rd_83_Scr(0x001E, 1); //读取样气抽气次数
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1E) && (Com3RBuf[6] == 0x01))
    {
        BGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
    }
    Rd_83_Scr(0x001F, 1); //读取零气抽气次数
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1F) && (Com3RBuf[6] == 0x01))
    {
        ZGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
    }
    for(i = 0; i < BGas_Num; i++)
    {
        Get_BGas();
    }
    for(i = 0; i < ZGas_Num; i++)
    {
        Get_ZGas();
    }
    temp[0] = 0x00;
    temp[1] = 0x41;
    Wr_80_Scr(0x03, temp, 2); //进入65配气02界面
    Wr_80_Scr(0x02, &time, 1); //蜂鸣器响
    while(1)
    {
        if(is_gasbag_exit(0) == 0) //拔掉气袋，返回47配气00界面
        {
            temp[0] = 0x00;
            temp[1] = 0x2F;
            Wr_82_Scr(0x001E, temp1, 2);
            Wr_82_Scr(0x001F, temp1, 2);
            Wr_80_Scr(0x03, temp, 2);
            break;
        }
        if(BtnPressedFlag == 0xAA) //按下主页键，返回待机界面
        {
            Wr_82_Scr(0x001E, temp1, 2);
            Wr_82_Scr(0x001F, temp1, 2);
            break;
        }
    }

}

/**
  *@breif  CO2标定测量
	*@param  none
	*@retval none
*/
void CO2_Zero(void)
{
    uint8_t temp[2];
    uint8_t i;
    uint16_t CO2_ADVal;
    uint16_t CO2_ADVal_Sum = 0;
    uint16_t CO2_ZERO_Sub;
    uint16_t CO2_ZERO_Sum = 0;

    QiGuang_Reset();//气缸复位

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);//读取气压、温度和湿度值

    if(F_DC == 1)
    {
        printf("气压=%f百帕\r\n", BME_Pressure);
        printf("温度=%f摄氏度\r\n", BME_Temperature);
    }

    for(i = 0; i < 3; i++)
    {
        /*零气测试*/
        Purge_Cylinder();//气缸清洗
        Open_CO2_Paiqi();
        Daiji(60);// 60S清洗CO2气室
        Close_CO2_Paiqi();
        Daiji(30);
        ADC2_ValConvt(&CO2_ZERO_Sub);//测量零点电平
        if(F_DC == 1)
        {
            printf("第%d次CO2零点值=%d\r\n", i + 1, CO2_ZERO_Sub);
        }
        CO2_ZERO_Sum += CO2_ZERO_Sub;

        /*测试样气*/
        Get_CO2_GAS();//抽取样气
        Daiji(30);
        ADC2_ValConvt(&CO2_ADVal);
        if(F_DC == 1)
        {
            printf("第%d次CO2电平值=%d\r\n", i + 1, CO2_ADVal);
        }
        CO2_ADVal_Sum += CO2_ADVal;
        Paiqi_RR_PROM();//清洗CO2传感器10s
    }
    CO2_ZERO = CO2_ZERO_Sum / 3;  //求零点平均值
    temp[0] = CO2_ZERO >> 8;
    temp[1] = CO2_ZERO;
    Wr_82_Scr(0x0007, temp, 2);
    ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);

    if(F_DC == 1)
    {
        printf("零点平均值=%d\r\n", CO2_ZERO);
    }

    CO2_B *= BME_Pressure / 1013.25 * (T0 + 273) / (BME_Temperature + 273);; //对标准气从V/V转化为m/v
    if(F_DC == 1)
    {
        printf("海拔校正后标定气质量浓度(*100):%d\r\n", CO2_B);
    }
    CO2_Taijie = (uint16_t)(CO2_ADVal_Sum - CO2_ZERO_Sum) / (CO2_B * 0.03); //求CO2标定系数

    if(F_DC == 1)
    {
        printf("CO2标定系数=%d\r\n", CO2_Taijie);
    }
    ee_WriteBytes((uint8_t *)&CO2_Taijie, 6, 2);
    temp[0] = CO2_Taijie >> 8;
    temp[1] = CO2_Taijie;
    Wr_82_Scr(0x0006, temp, 2);

    temp[0] = 0x00;
    temp[1] = 0x21;
    Wr_80_Scr(0x03, temp, 2); //跳到33CO2标定03界面
    Open_CO2_Paiqi();
    Daiji(60); // 60S清洗CO2气室
    Close_CO2_Paiqi();
}
/**
  *@breif  CO2标定主流程
	*@param  none
	*@retval none
  */
void CO2_CON(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};

    while(!is_gasbag_exit(0));//气袋状态判定

    temp[0] = 0x00;
    temp[1] = 0x1F;
    Wr_80_Scr(0x03, temp, 2); //进入31CO2标定01界面
    Delay1ms(5);

    Rd_83_Scr(0x0012, 1); //读取标准气CO2浓度
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
    {
        CO2_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        if(F_DC == 1)
        {
            printf("CO2标准气浓度_V/V：%d\r\n", CO2_B);
        }
    }

    CO2_Zero();//进行测量
    Wr_80_Scr(0x02, &time, 1); //蜂鸣器响
    while(1)
    {
        if(is_gasbag_exit(0) == 0)
        {
            temp[0] = 0x00;
            temp[1] = 0x1E;
            Wr_82_Scr(0x0012, temp1, 2); //标准气浓度清零
            Wr_80_Scr(0x03, temp, 2); //返回30CO2标定00界面
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            temp[0] = 0x00;
            temp[1] = 0x1E;
            Wr_82_Scr(0x0012, temp1, 2); //标准气浓度清零
            break;
        }
    }
}

/*
求精密度和准确度
P[]是多次测试的结果
num是测量数据的次数
select是精密度和准确度的选择，为1时进行精密度计算，为0时进行准确度计算
stand  是实际数值乘以100
20180818
*/

float StandardDeviation(float * P, uint8_t num, uint8_t select, uint16_t Stand)
{
    float stdd;//标准偏差值
    float R_SD;
    float sum = 0; //求和
    float avg;//平均值
    float Sum_Dev = 0;
    uint8_t m;
    for(m = 0; m < num; m++) //求和
    {
        sum += P[m];
    }
    avg = sum / num; //求均值
//    printf("均值=%f\r\n", avg);
    if(select == 1) //精密度计算
    {
        for(m = 0; m < num; m++)
        {
            Sum_Dev += (P[m] - avg) * (P[m] - avg);
        }
    }
    else  //准确度计算
    {
        for(m = 0; m < num; m++)
        {
            Sum_Dev += (P[m] - Stand * 0.01) * (P[m] - Stand * 0.01);
        }
    }
    stdd = sqrt(Sum_Dev / num);
    if(select == 1)
    {
        R_SD = stdd / avg;
//        printf("精密度R_SD=%f\r\n", R_SD);
    }
    else
    {
        R_SD = stdd / (Stand * 0.01);
//        printf("准确度R_SD=%f\r\n", R_SD);
    }
    return R_SD;

}

