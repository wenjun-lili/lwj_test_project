#include "GasRoad.h"
#include "bsp.h"
#include "DWIN.h"
#include "BME280.h"

extern uint16_t Warming_up_Cnt;

void Open_paiqi(void)
{
    EV_Open(1);
    Delay100ms(1);
    EV_Open(6);//20180322
    Delay100ms(1);
    GAS_PUB_OPEN();

}
void close_paiqi(void)
{
    GAS_PUB_Closed();
    EV_Close(6);//20180322
    Delay100ms(1);
    EV_Close(1);
    Delay100ms(1);
}

void Open_CO2_Paiqi(void)
{
    EV_Open(6);//20180322
    Delay100ms(1);
    EV_Open(2);
    Delay100ms(1);
    GAS_PUB_OPEN();

}

void Close_CO2_Paiqi(void)
{
    GAS_PUB_Closed();
    EV_Close(2);
    Delay100ms(1);
    EV_Close(6);//20180322
    Delay100ms(1);
}

/**
  *@breif  气缸复位
	*@param  none
	*@retval none
	*/
void QiGuang_Reset(void)
{
    EV_Open(1);
    Delay100ms(1);
    Motor1Start(1, 299);	//左到位为复位
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();
    EV_Close(1);
    Delay100ms(1);
}

void Get_CO2_GAS(void)
{
    EV_Open(3);  // 样气袋
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}


void Paiqi_R_PROM(void)  // 清洗CO气室
{
    Open_paiqi();

    Daiji(100);//  清洗气室100S

    close_paiqi( );

}
void Paiqi_RRR_PROM(void)  // 清洗CO气室
{
    Open_paiqi();

    Daiji(30);//  清洗气室30S

    close_paiqi( );

}

// 这里CO2_RCONC 为台阶数
void Celiang_ZhunBei( uint16_t *ptr)
{
    QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S清洗CO2气室

    Close_CO2_Paiqi();

    Get_CO2_GAS();// 从样气袋抽气带入CO2气室

    Daiji(30);//  打入CO2气室，倒计时30S

    ADC(ptr);// AD 采样

}
/**
  *@breif 测量CO2零气电平和样气浓度
	*@param zero：零气电平
	        ptr：样气电平
	*@return none
*/
void Celiang_ZhunBeiNew(uint16_t *Zero, uint16_t *ptr)
{
    QiGuang_Reset();

    Open_CO2_Paiqi();

    Daiji(60);// 60S清洗CO2气室

    Close_CO2_Paiqi();

    ADC2_ValConvt(Zero);

    Get_CO2_GAS();// 从样气袋抽气带入CO2气室

    Daiji(30);//  打入CO2气室，倒计时30S

    ADC2_ValConvt(ptr);

}



void DaoQi_Y(void)
{
    EV_Open(3);  // 样气袋
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}
void DaoQi_B(void)
{
    EV_Open(4);  // 样气袋
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(6);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);
}
/**
  *@breif  清洗CO2传感器
	*@param  none
	*@retval none
  */
void Paiqi_RR_PROM(void)
{
    Open_CO2_Paiqi();
    Daiji(10);// 待机10S,静置
    Close_CO2_Paiqi( );
}


void Daiji(uint16_t sec)
{
    Warming_up_Cnt = sec;
    TIM_Cmd(TIM7, ENABLE);
    while(1)
    {
        if(Warming_up_Cnt == 0)
        {
            TIM_Cmd(TIM7, DISABLE);
            break;
        }
    }
}

void Chou_BGas(void)
{
    EV_Open(5);  //环境气倒气袋 电磁阀由6改为5
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

    EV_Open(1); // CO气室
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}
void Chou_YGas(void)
{
    EV_Open(4);  //A标准气CO通道
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(1); // CO气室
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}

void Get_BGas(void)
{
    EV_Open(3);  // 样气袋
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}

void Get_ZGas(void)
{
    EV_Open(4);  // 样气袋
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

}

void Get_Zerro_Gas(void)
{
    EV_Open(6);  // 样气袋   改为电磁阀6  20180323
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}

void CO2_CO_SensorZERO(void)
{
    uint16_t CO2_PB_1, CO_PB;
    int16_t CO_cha;
    float CO2_CONC;
    uint8_t temp[2];

    ADC2_ValConvt(&CO2_PB_1);//利用32自带ADC

    if(F_DC==1)
    {
        printf("CO2电平=%d\r\n", CO2_PB_1);
    }

    ADC(&CO_PB);
    CO_cha = abs(CO_PB - CO_ZERO);
    if(F_DC==1)
    {
        printf("电平差=%d\r\n", CO_cha);
    }

    CO2_CONC = ((float)(CO2_PB_1 - CO2_ZERO)) / CO2_Taijie;
    if(F_DC==1)
    {
        printf("CO2浓度=%f\r\n", CO2_CONC);
    }

    if(((CO2_PB_1 < 300) || (CO2_PB_1 > 700)) || (CO_cha > 1000))
    {
        if(((CO2_PB_1 < 300) || (CO2_PB_1 > 700)) && (CO_cha > 1000))
        {
            if(CO2_CONC >= 0.000001)
            {
                temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
                temp[1] = (uint16_t)(CO2_CONC * 100);
                Wr_82_Scr(0x0022, temp, 2); //将CO2值输出
            }
            temp[0] = CO_cha >> 8;
            temp[1] = CO_cha;
            Wr_82_Scr(0x0002, temp, 2); //将CO值输出
            temp[0] = 0x00;
            temp[1] = 0x3A;
            Wr_80_Scr(0x03, temp, 2); //从02预热界面跳到58CO和CO2零点同时报错界面
            if(F_DC==1)
            {
                printf("预热未通过\r\n");
            }
        }
        else if((CO2_PB_1 < 300) || (CO2_PB_1 > 700))
        {
            if(CO2_CONC >= 0.000001)
            {
                temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
                temp[1] = (uint16_t)(CO2_CONC * 100);
                Wr_82_Scr(0x0022, temp, 2); //将CO2值输出
            }
            temp[0] = 0x00;
            temp[1] = 0x39;
            Wr_80_Scr(0x03, temp, 2); //从02预热界面跳到57CO2零点报错界面
            if(F_DC==1)
            {
                printf("预热未通过\r\n");
            }
        }
        else if(CO_cha > 1000)
        {
            temp[0] = CO_cha >> 8;
            temp[1] = CO_cha;
            Wr_82_Scr(0x0002, temp, 2); //将CO值输出
            temp[0] = 0x00;
            temp[1] = 0x37;
            Wr_80_Scr(0x03, temp, 2); //从02预热界面跳到55CO预热零点报错界面
            if(F_DC==1)
            {
                printf("预热未通过\r\n");
            }
        }
        while(1)
        {
            if(BtnPressedFlag == 0xAA) //按下确认键后直接跳到预热界面
            {
                BtnPressedFlag = 0;
                break;
            }
        }
    }
}
/**
  *@breif  橡胶圈使用次数预警
	*@detail 使用次数超过2000则报错
	*@param  none
	*@retval none
*/
void Rubber_Ring_Fault(void)
{
    uint8_t temp[2];
    if(Ring_Num > 2000)
    {
        temp[0] = 0x00;
        temp[1] = 0x3F;
        Wr_80_Scr(0x03, temp, 2); //从02预热界面跳到63橡胶圈报错界面
        if(F_DC==1)
        {
            printf("橡胶圈使用次数预警\r\n");
        }
        while(1)
        {
            if(BtnPressedFlag == 0xAA) //按下确认键后直接跳到预热界面
            {
                BtnPressedFlag = 0;
                break;
            }
        }
    }
}

/**
  *@breif  三合一传感器自检和干燥剂自检
  *@datail 三合一传感器故障预警，主要处理机制是：（温度单位：℃；湿度单位：%；气压单位：hPa）
           （1）当气压不在[500 1100]范围内，则再次测量还不在范围内则报三合一传感器错；
           （2）当温度不在(0 40)之间，则再次测量还是处于不在范围内则报三合一传感器错；
           （3）当湿度不在(0,100]之间，则再次测量还是不在范围内则报三合一传感器错。
           干燥剂干燥效果自检
		 （1）当湿度大于40的情况下，则会提示干燥剂失效
  *@prarm  none
  *@retval none
  */
void Bme280Fault(void)
{
    uint8_t temp[2];

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity); //测量湿度、温度、气压值

    if(F_DC == 1)
    {
        printf("BME280\r\n");
        printf("预后湿度=%f\r\n", BME_Humidity);
        printf("预后温度=%f\r\n", BME_Temperature);
        printf("预后气压=%f\r\n", BME_Pressure);
    }
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x000A, temp, 2);

    if((BME_Pressure > 1100) || (BME_Pressure < 500))
    {
        Delay100ms(10);//等待1s

        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity); //再一次测量湿度、温度、气压值

        if((BME_Pressure > 1100) || (BME_Pressure < 500))
        {
            temp[0] = 0x00;
            temp[1] = 0x4C;
            Wr_80_Scr(0x03, temp, 2); //跳到76气压自检错误
            if(F_DC == 1)
            {
                printf("气压报错\r\n");
            }

            while(1)
            {
                if(BtnPressedFlag == 0xAA) //按下确认键
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }

        }
    }
    if((BME_Temperature > 35) || (BME_Temperature < 0.0001)) ///温度故障判断
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        if((BME_Temperature > 40) || (BME_Temperature < 0.0001))
        {
            temp[0] = 0x00;
            temp[1] = 0x4B;
            Wr_80_Scr(0x03, temp, 2); //跳到75温度自检错误
            if(F_DC == 1)
            {
                printf("温度报错\r\n");
            }

            while(1)
            {
                if(BtnPressedFlag == 0xAA) //按下确认键
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }
        }
    }

    if((BME_Humidity > 99) || (BME_Humidity < 0.0001)) ///湿度故障判断
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        if((BME_Humidity > 100) || (BME_Humidity < 0.0001)) ///湿度故障判断
        {
            temp[0] = 0x00;
            temp[1] = 0x4A;
            Wr_80_Scr(0x03, temp, 2); //跳到74湿度自检错误
            if(F_DC == 1)
            {
                printf("湿度报错\r\n");
            }

            while(1)
            {
                if(BtnPressedFlag == 0xAA) //按下确认键
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }
        }
    }
}

/**
  *@breif  气缸清洗流程
	*@param  none
	*@retval none
	*/
void Purge_Cylinder(void)
{
    EV_Open(7);  // 打开第七个电磁阀
    Delay100ms(1);

    Motor1Start(0, 299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(7);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1, 299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}
/**
  *@breif  采集零气
	*@param  none
	*@retval none
  */
void DaoQi_Z(uint16_t sec)
{
    EV_Open(6);
    Delay100ms(1);
    EV_Open(5);
    Delay100ms(1);
    GAS_PUB_OPEN();
    Daiji(sec);
    GAS_PUB_Closed();
    EV_Close(5);
    Delay100ms(1);
    EV_Close(6);
    Delay100ms(1);
}
