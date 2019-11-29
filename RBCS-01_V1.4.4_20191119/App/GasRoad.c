/**
  * @file GasRoad.c
	* @brief 气路电磁阀控制文件
	*/

#include "GasRoad.h"
#include "bsp.h"
#include "CO2_Serial.h"
#include "BME280.h"

extern uint16_t Warming_up_Cnt;

void Open_paiqi(void)
{
    EV_Open(1);
    Delay100ms(1);
    EV_Open(7);
    Delay100ms(1);
    GAS_PUB_OPEN();

}
void close_paiqi(void)
{
    GAS_PUB_Closed();
    EV_Close(7);
    Delay100ms(1);
    EV_Close(1);
    Delay100ms(1);
}

void Open_CO2_Paiqi(void)
{
    EV_Open(7);
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
    EV_Close(7);
    Delay100ms(1);
}

void SELET_CO(void)
{
    IN_Select(1);// 选择CO模拟信号通道
    TIM6Delay_f(3);

}

void CO2_Test(void)
{
    IN_Select(2);// 选择CO2模拟信号通道
    TIM6Delay_f(3);
}


void QiGuang_Reset(void)
{
    // 气缸复位
    EV_Open(1);
    Delay100ms(1);
    Motor1Start(1,299);	/* 左到位为 复位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();
    EV_Close(1);
    Delay100ms(1);
}

void Get_CO2_GAS(uint8_t Select_Valve)
{
    EV_Open(Select_Valve);  // 样气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(Select_Valve);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);

}

void Purge_Cylinder(void)
{
    EV_Open(4);  // 环境本底气
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}

void Paiqi_R_PROM(void)  // 清洗CO气室
{
//	 QiGuang_Reset();
    Open_paiqi();

    Daiji(100);//  清洗气室100S

    close_paiqi();

}

void Paiqi_RRR_PROM(void)  // 清洗CO气室
{
//	 QiGuang_Reset();
    Open_paiqi();

    Daiji(30);//  清洗气室30S

    close_paiqi( );

}

void Celiang_ZhunBei( uint16_t *ptr)
{
    QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S清洗CO2气室

    Close_CO2_Paiqi();

    Get_CO2_GAS(3);// 从样气袋抽气带入CO2气室

    ShowString(0x01,0x06,"Sample_Identify.....");

    Daiji(30);//  打入CO2气室，倒计时30S

    CO2_Test();  //CO2模拟信号输入到AD处

    ADC(ptr);// AD 采样

    IN_Select(0);//关闭单刀八掷开关

}

uint16_t Get_CO2(void)
{
    uint8_t t;
    uint16_t Nd;
    uint8_t nu=0;//读取串口次数
    uint8_t nu1=0;//校验和错误次数
    uint8_t a,b;
    uint8_t Checksum=0;//校验和
    int8_t sum=0;
    while(1)
    {
//			  printf("1");
        USART_Cmd(USART3,ENABLE);//串口使能
        Delay100ms(20);
        if(Rec3FinishF==1) //接收完成判断
        {
            for(t=0; t<Index3End-4; t++) //求数据和
            {
                sum+=(int8_t)Com3RBuf[t];
            }
            Checksum=(uint8_t)(-sum);//得到校验和
            a=CharToDec(Com3RBuf[Index3End-3]);
            b=CharToDec(Com3RBuf[Index3End-2]);
            if(Checksum==(a*16+b))
            {
#if 0
                for(t=0; t<=Index3End; t++)
                {
                    USART_SendData(USART1, Com3RBuf[t]);//向串口1发送数据
                    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
                }
#endif
//                printf("YES\r\n");
                if((Com3RBuf[3]<0x30)||(Com3RBuf[3]>0x39))//如果小数点第二位没有数，则跳出该次循环
                {
                    Rec3FinishF=0;
                    sum=0;
                    if(nu1==5) //如果第5次接收不正确
                    {
                        CO2_Fault_Code=2;
                        break;
                    }
                    nu1++;
                    continue;
                }
                if(Com3RBuf[0]==0x2D) //传感器测量值显示为负值时，则赋为0
                {
                    Nd=0;
                }
                else
                {
                    Nd=(uint16_t)(Com3RBuf[0]-0x30)*100+(uint16_t)(Com3RBuf[2]-0x30)*10+(uint16_t)(Com3RBuf[3]-0x30);//计算浓度值
                }
//                printf("浓度为：%d\r\n",Nd);//
                Com3Index=0;//索引量归零
                Rec3FinishF=0;
                break;
            }
            else
            {
//                printf("NO");
                Com3Index=0;//索引量归零
                Rec3FinishF=0;
                sum=0;
                if(nu1==9) //如果第10次校验和不通过，则提示
                {
                    CO2_Fault_Code=2;
                    break;
                }
                nu1++;
                continue;
            }

        }
        else
        {
            if(nu==9)  //如果第10次未能读取到串口信息，则提示
            {
                CO2_Fault_Code=1;
                break;
            }
            nu++;
        }
    }
    return Nd;
}

void Celiang_ZhunBeiNew(uint16_t *Zero, uint16_t *ptr)
{
//	 QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S清洗CO2气室

    Close_CO2_Paiqi();

    Daiji(30);

    ADC2_ValConvt(Zero);//20180606  ADC模式

    Get_CO2_GAS(3);// 从样气袋抽气带入CO2气室

    ShowString(0x01,0x06,"Sample_Identify.....");

    Daiji(30);//  打入CO2气室，倒计时30S

    ADC2_ValConvt(ptr);//20180606 新增ADC

}

uint16_t Celiang_CO2(uint8_t i)
{
    uint16_t Nd_Yang,Nd_Background;
    Open_CO2_Paiqi();
    Daiji(60);// 60S清洗CO2气室

    Close_CO2_Paiqi();
    Nd_Background=Get_CO2();

    Get_CO2_GAS(i);// 从i袋抽气带入CO2气室

    ShowString(0x01,0x06,"Sample_Identify.....");

    Daiji(30);//  打入CO2气室，倒计时30S

    Nd_Yang=Get_CO2();

    Nd_Yang-=Nd_Background;//减去本底值

    return Nd_Yang;
}

void DaoQi_Y(void)
{
    EV_Open(3);  // 样气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}
void DaoQi_B(void)
{
    EV_Open(4);  // 样气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(6);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);
}

void DaoQi_Z(uint16_t sec)
{
    EV_Open(7);
    Delay100ms(1);
    EV_Open(4);
    Delay100ms(1);
    GAS_PUB_OPEN();
    Daiji(sec);
    GAS_PUB_Closed();
    EV_Close(4);
    Delay100ms(1);
    EV_Close(7);
    Delay100ms(1);
}

void Paiqi_RR_PROM(void)// 清洗CO2气室
{
//	 QiGuang_Reset();
    Open_CO2_Paiqi();
    Daiji(10);// 待机10S,静置
    Close_CO2_Paiqi( );
}


void Daiji(uint16_t sec)
{
    ShowString(0x17,0x07,"(    )");
    Warming_up_Cnt=sec;
    ShowString(0x18,0x07, Num2StrI(Warming_up_Cnt, "%04d"));
    TIM_Cmd(TIM7, ENABLE);
    while(1)
    {
        if(Warming_up_Cnt==0)
        {
            TIM_Cmd(TIM7, DISABLE);
            break;
        }
    }
    framedisplay_ok();
}

void Chou_BGas(void)
{
    EV_Open(6);  //环境气倒气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);

    EV_Open(1); // CO气室
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}
void Chou_YGas(void)
{
    EV_Open(5);  //环境气倒气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

    EV_Open(1); // CO气室
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}

void Get_BGas(void)
{
    EV_Open(3);  // 样气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}

void Get_ZGas(void)
{
    EV_Open(4);  // 样气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

}

void Get_Zerro_Gas(void)
{
    EV_Open(4);  // 样气袋
    Delay100ms(1);

    Motor1Start(0,299);	/*  抽气 右到位 */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1,299);	/* 打气 左到位*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}

/**
  * @brief CO2_CO_Sensorzero(void)
  * @datail CO2和CO阈值判断
  * @prarm none
  * @return none
  */
void CO2_CO_SensorZERO(void)
{
    uint16_t CO2_PB_1,CO_PB;
    int16_t CO_cha;
    float CO2_CONC;

    ADC2_ValConvt(&CO2_PB_1);

    ADC(&CO_PB);
    CO_cha=abs(CO_PB-CO_ZERO);

    CO2_CONC=((float)(CO2_PB_1-CO2_ZERO))/CO2_Taijie;

    if(((CO2_PB_1<300)||(CO2_PB_1>700))||(CO_cha>1000))
    {
        if(((CO2_PB_1<300)||(CO2_PB_1>700))&&(CO_cha>1000))
        {
            LcmClear(0x00);// 清屏
            ShowString(0x04,0x01,"CO_ZERO  fault!");
            ShowString(0x04,0x02,"CO2_ZERO fault!");
            ShowString(0x13,0x01,Num2StrI(CO_cha,"%06d"));
            if(CO2_CONC>=0.000001)
            {
                ShowString(0x13,0x02,Num2StrF(CO2_CONC,"%2.1f"));
                ShowString(0x17,0x02,"%");
                ShowString(0x00,0x06,"Press RUN Key to Ready");

            }
            while(1)
            {
                if(BtnPressedFlag==4)
                {
                    BtnPressedFlag=-1;
                    break;
                }
            }
        }
        else if((CO2_PB_1<300)||(CO2_PB_1>700))
        {
            LcmClear(0x00);// 清屏
            ShowString(4,1,"CO2 Zero fault!");
            if(CO2_CONC>=0.000001)
            {
                ShowString(0x13,0x01,Num2StrF(CO2_CONC,"%2.1f"));
                ShowString(0x17,0x01,"%");
                ShowString(0x00,0x06,"Press RUN Key to Ready");
            }

            while(1)
            {
                if(BtnPressedFlag==4)
                {
                    BtnPressedFlag=-1;
                    break;
                }
            }
        }
        else if(CO_cha>1000)
        {
            LcmClear(0x00);// 清屏
            ShowString(4,1,"CO Zero fault!");
            ShowString(0x13,0x01,Num2StrI(CO_cha,"%06d"));
            ShowString(0x00,0x06,"Press RUN Key to Ready");
            while(1)
            {
                if(BtnPressedFlag==4)
                {
                    BtnPressedFlag=-1;
                    break;
                }
            }
        }

    }
}
/**
  * @brief Rubber_Ring_Fault(void)
  * @datail 橡胶圈使用次数预警
  * @prarm none
  * @return none
  */
void Rubber_Ring_Fault(void)
{
    if(Ring_Num>2000)
    {
//	    printf("Rubber_Ring num=%d\r\n",Ring_Num);
        LcmClear(0x00);// 清屏
        ShowString(4,1,"Rubber Ring fault!");
        ShowString(4,3,"Please replace it!");
        ShowString(1,5,"Press RUN Key to Ready");
        while(1)
        {
            if(BtnPressedFlag==4)//按下run键跳过预警
            {
                BtnPressedFlag=-1;
                break;
            }
            else if(BtnPressedFlag==5)//按下右键清零
            {
                BtnPressedFlag=-1;
                Ring_Num=0;
                ee_WriteBytes((uint8_t *)&Ring_Num,10,2);
                break;
            }
        }
    }
}
/**
  * @brief CO2_Sensor_Fault(void)
  * @datail 对二氧化碳传感器故障提示
  * @prarm none
  * @return none
  */
void CO2_Sensor_Fault(void)
{
    LcmClear(0x00);// 清屏
    ShowString(4,1,"CO2 Sensor Fault!");
    ShowString(4,3,"Please replace it!");
    ShowString(1,5,"Press RUN Key to Ready");
    while(1)
    {
        if(BtnPressedFlag==4)//按下run键跳过预警
        {
            BtnPressedFlag=-1;
            break;
        }
    }
}

/**
  * @brief Humidity_Fault(void)
  * @datail 对干燥管湿度预警
  * @prarm none
  * @return none
  */
void Humidity_Fault(void)
{
    if(BME_Humidity>HumidityThreshold)
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if(BME_Humidity>HumidityThreshold)///再一次湿度大于40
        {
            LcmClear(0x00);// 清屏
            ShowString(6,1,"Humidity fault!");///提示湿度报错
            ShowString(3,3,"Please replace dry pipe!");///提示更换干燥管
            ShowString(4,5,"Press RUN Key to Ready");///按下run键则可以跳过预警
            while(1)
            {
                if(BtnPressedFlag==4)///按下run键跳过预警
                {
                    BtnPressedFlag=-1;
                    break;
                }
            }
        }
    }
}
/**
  *@breif  Bme280Fault(void)
  *@datail 三合一传感器故障预警，主要处理机制是：
           （1）当气压不在[500 1100]范围内，则再次测量还不在范围内则直接提示气压不在范围内；
           （2）当温度不在(0 40)之间，则再次测量还是处于不在范围内则直接提示温度不在范围内；
           （3）当湿度不在(0,100]之间，则再次测量还是不在范围内则直接提示湿度不在范围内；
           （4）当以上三者均不满足，则直接提示传感器故障。
  *@prarm  none
  *@retval none
  */
void Bme280Fault(void)
{
    uint8_t FlagHumi=0;///湿度故障
    uint8_t FlagTemp=0;///温度故障
    uint8_t FlagAtmo=0;///气压故障
    uint8_t FlagFault=0;///故障提示标志，1为发生过故障，0为无故障

    if((BME_Pressure>1100)||(BME_Pressure<500))///气压故障判断
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if((BME_Pressure>1100)||(BME_Pressure<500))
        {
            FlagAtmo=1;
            FlagFault=1;
            LcmClear(0x00);/// 清屏
            ShowString(2,1,"Pressure is out of range!");///提示气压报错
            ShowString(4,5,"Press RUN Key to Ready");///按下run键则可以跳过预警
        }
    }

    if((BME_Temperature>35)||(BME_Temperature<0.0001))///温度故障判断
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if((BME_Temperature>40)||(BME_Temperature<0.0001))
        {
            FlagFault=1;
            FlagTemp=1;
            LcmClear(0x00);/// 清屏
            ShowString(1,1,"Temperature is out of range!");///提示温度报错
            ShowString(4,5,"Press RUN Key to Ready");///按下run键则可以跳过预警
        }
    }

    if((BME_Humidity>99)||(BME_Humidity<0.0001))///湿度故障判断
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if((BME_Humidity>100)||(BME_Humidity<0.0001))///湿度故障判断
        {
            FlagHumi=1;
            FlagFault=1;
            LcmClear(0x00);/// 清屏
            ShowString(2,1,"Humidity is out of range!");///提示湿度报错
            ShowString(4,5,"Press RUN Key to Ready");///按下run键则可以跳过预警
        }
    }

    if((FlagAtmo==1)&&(FlagHumi==1)&&(FlagTemp==1))///整体故障判断
    {
        LcmClear(0x00);/// 清屏
        ShowString(2,1,"Three-in-one sensor fault!");///提示三合一传感器故障
        ShowString(4,5,"Press RUN Key to Ready");///按下run键则可以跳过预警
    }

    /**
      跳出预警界面，回到待机界面
      */
    if(FlagFault==1)
    {
        while(1)
        {
            if(BtnPressedFlag==4)
            {
                BtnPressedFlag=-1;
                break;
            }
        }
    }
}
/**
  *@breif  airTightTest()
  *@detail 气密性检查，打开电磁阀1、3、4、5、6
  *@param  void
  *@retval void
  */
void airTightTest(void)
{
    EV_Open(1);
    EV_Open(3);
    EV_Open(4);
    EV_Open(5);
    EV_Open(6);
    while(1);
}
