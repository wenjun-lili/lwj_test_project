/**
  * @file GasRoad.c
	* @brief ��·��ŷ������ļ�
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
    IN_Select(1);// ѡ��COģ���ź�ͨ��
    TIM6Delay_f(3);

}

void CO2_Test(void)
{
    IN_Select(2);// ѡ��CO2ģ���ź�ͨ��
    TIM6Delay_f(3);
}


void QiGuang_Reset(void)
{
    // ���׸�λ
    EV_Open(1);
    Delay100ms(1);
    Motor1Start(1,299);	/* ��λΪ ��λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();
    EV_Close(1);
    Delay100ms(1);
}

void Get_CO2_GAS(uint8_t Select_Valve)
{
    EV_Open(Select_Valve);  // ������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(Select_Valve);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);

}

void Purge_Cylinder(void)
{
    EV_Open(4);  // ����������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}

void Paiqi_R_PROM(void)  // ��ϴCO����
{
//	 QiGuang_Reset();
    Open_paiqi();

    Daiji(100);//  ��ϴ����100S

    close_paiqi();

}

void Paiqi_RRR_PROM(void)  // ��ϴCO����
{
//	 QiGuang_Reset();
    Open_paiqi();

    Daiji(30);//  ��ϴ����30S

    close_paiqi( );

}

void Celiang_ZhunBei( uint16_t *ptr)
{
    QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();

    Get_CO2_GAS(3);// ����������������CO2����

    ShowString(0x01,0x06,"Sample_Identify.....");

    Daiji(30);//  ����CO2���ң�����ʱ30S

    CO2_Test();  //CO2ģ���ź����뵽AD��

    ADC(ptr);// AD ����

    IN_Select(0);//�رյ�����������

}

uint16_t Get_CO2(void)
{
    uint8_t t;
    uint16_t Nd;
    uint8_t nu=0;//��ȡ���ڴ���
    uint8_t nu1=0;//У��ʹ������
    uint8_t a,b;
    uint8_t Checksum=0;//У���
    int8_t sum=0;
    while(1)
    {
//			  printf("1");
        USART_Cmd(USART3,ENABLE);//����ʹ��
        Delay100ms(20);
        if(Rec3FinishF==1) //��������ж�
        {
            for(t=0; t<Index3End-4; t++) //�����ݺ�
            {
                sum+=(int8_t)Com3RBuf[t];
            }
            Checksum=(uint8_t)(-sum);//�õ�У���
            a=CharToDec(Com3RBuf[Index3End-3]);
            b=CharToDec(Com3RBuf[Index3End-2]);
            if(Checksum==(a*16+b))
            {
#if 0
                for(t=0; t<=Index3End; t++)
                {
                    USART_SendData(USART1, Com3RBuf[t]);//�򴮿�1��������
                    while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//�ȴ����ͽ���
                }
#endif
//                printf("YES\r\n");
                if((Com3RBuf[3]<0x30)||(Com3RBuf[3]>0x39))//���С����ڶ�λû�������������ô�ѭ��
                {
                    Rec3FinishF=0;
                    sum=0;
                    if(nu1==5) //�����5�ν��ղ���ȷ
                    {
                        CO2_Fault_Code=2;
                        break;
                    }
                    nu1++;
                    continue;
                }
                if(Com3RBuf[0]==0x2D) //����������ֵ��ʾΪ��ֵʱ����Ϊ0
                {
                    Nd=0;
                }
                else
                {
                    Nd=(uint16_t)(Com3RBuf[0]-0x30)*100+(uint16_t)(Com3RBuf[2]-0x30)*10+(uint16_t)(Com3RBuf[3]-0x30);//����Ũ��ֵ
                }
//                printf("Ũ��Ϊ��%d\r\n",Nd);//
                Com3Index=0;//����������
                Rec3FinishF=0;
                break;
            }
            else
            {
//                printf("NO");
                Com3Index=0;//����������
                Rec3FinishF=0;
                sum=0;
                if(nu1==9) //�����10��У��Ͳ�ͨ��������ʾ
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
            if(nu==9)  //�����10��δ�ܶ�ȡ��������Ϣ������ʾ
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

    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();

    Daiji(30);

    ADC2_ValConvt(Zero);//20180606  ADCģʽ

    Get_CO2_GAS(3);// ����������������CO2����

    ShowString(0x01,0x06,"Sample_Identify.....");

    Daiji(30);//  ����CO2���ң�����ʱ30S

    ADC2_ValConvt(ptr);//20180606 ����ADC

}

uint16_t Celiang_CO2(uint8_t i)
{
    uint16_t Nd_Yang,Nd_Background;
    Open_CO2_Paiqi();
    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();
    Nd_Background=Get_CO2();

    Get_CO2_GAS(i);// ��i����������CO2����

    ShowString(0x01,0x06,"Sample_Identify.....");

    Daiji(30);//  ����CO2���ң�����ʱ30S

    Nd_Yang=Get_CO2();

    Nd_Yang-=Nd_Background;//��ȥ����ֵ

    return Nd_Yang;
}

void DaoQi_Y(void)
{
    EV_Open(3);  // ������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}
void DaoQi_B(void)
{
    EV_Open(4);  // ������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(6);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
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

void Paiqi_RR_PROM(void)// ��ϴCO2����
{
//	 QiGuang_Reset();
    Open_CO2_Paiqi();
    Daiji(10);// ����10S,����
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
    EV_Open(6);  //������������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);

    EV_Open(1); // CO����
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}
void Chou_YGas(void)
{
    EV_Open(5);  //������������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

    EV_Open(1); // CO����
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}

void Get_BGas(void)
{
    EV_Open(3);  // ������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}

void Get_ZGas(void)
{
    EV_Open(4);  // ������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

}

void Get_Zerro_Gas(void)
{
    EV_Open(4);  // ������
    Delay100ms(1);

    Motor1Start(0,299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1,299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}

/**
  * @brief CO2_CO_Sensorzero(void)
  * @datail CO2��CO��ֵ�ж�
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
            LcmClear(0x00);// ����
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
            LcmClear(0x00);// ����
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
            LcmClear(0x00);// ����
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
  * @datail ��Ȧʹ�ô���Ԥ��
  * @prarm none
  * @return none
  */
void Rubber_Ring_Fault(void)
{
    if(Ring_Num>2000)
    {
//	    printf("Rubber_Ring num=%d\r\n",Ring_Num);
        LcmClear(0x00);// ����
        ShowString(4,1,"Rubber Ring fault!");
        ShowString(4,3,"Please replace it!");
        ShowString(1,5,"Press RUN Key to Ready");
        while(1)
        {
            if(BtnPressedFlag==4)//����run������Ԥ��
            {
                BtnPressedFlag=-1;
                break;
            }
            else if(BtnPressedFlag==5)//�����Ҽ�����
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
  * @datail �Զ�����̼������������ʾ
  * @prarm none
  * @return none
  */
void CO2_Sensor_Fault(void)
{
    LcmClear(0x00);// ����
    ShowString(4,1,"CO2 Sensor Fault!");
    ShowString(4,3,"Please replace it!");
    ShowString(1,5,"Press RUN Key to Ready");
    while(1)
    {
        if(BtnPressedFlag==4)//����run������Ԥ��
        {
            BtnPressedFlag=-1;
            break;
        }
    }
}

/**
  * @brief Humidity_Fault(void)
  * @datail �Ը����ʪ��Ԥ��
  * @prarm none
  * @return none
  */
void Humidity_Fault(void)
{
    if(BME_Humidity>HumidityThreshold)
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if(BME_Humidity>HumidityThreshold)///��һ��ʪ�ȴ���40
        {
            LcmClear(0x00);// ����
            ShowString(6,1,"Humidity fault!");///��ʾʪ�ȱ���
            ShowString(3,3,"Please replace dry pipe!");///��ʾ���������
            ShowString(4,5,"Press RUN Key to Ready");///����run�����������Ԥ��
            while(1)
            {
                if(BtnPressedFlag==4)///����run������Ԥ��
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
  *@datail ����һ����������Ԥ������Ҫ��������ǣ�
           ��1������ѹ����[500 1100]��Χ�ڣ����ٴβ��������ڷ�Χ����ֱ����ʾ��ѹ���ڷ�Χ�ڣ�
           ��2�����¶Ȳ���(0 40)֮�䣬���ٴβ������Ǵ��ڲ��ڷ�Χ����ֱ����ʾ�¶Ȳ��ڷ�Χ�ڣ�
           ��3����ʪ�Ȳ���(0,100]֮�䣬���ٴβ������ǲ��ڷ�Χ����ֱ����ʾʪ�Ȳ��ڷ�Χ�ڣ�
           ��4�����������߾������㣬��ֱ����ʾ���������ϡ�
  *@prarm  none
  *@retval none
  */
void Bme280Fault(void)
{
    uint8_t FlagHumi=0;///ʪ�ȹ���
    uint8_t FlagTemp=0;///�¶ȹ���
    uint8_t FlagAtmo=0;///��ѹ����
    uint8_t FlagFault=0;///������ʾ��־��1Ϊ���������ϣ�0Ϊ�޹���

    if((BME_Pressure>1100)||(BME_Pressure<500))///��ѹ�����ж�
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if((BME_Pressure>1100)||(BME_Pressure<500))
        {
            FlagAtmo=1;
            FlagFault=1;
            LcmClear(0x00);/// ����
            ShowString(2,1,"Pressure is out of range!");///��ʾ��ѹ����
            ShowString(4,5,"Press RUN Key to Ready");///����run�����������Ԥ��
        }
    }

    if((BME_Temperature>35)||(BME_Temperature<0.0001))///�¶ȹ����ж�
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if((BME_Temperature>40)||(BME_Temperature<0.0001))
        {
            FlagFault=1;
            FlagTemp=1;
            LcmClear(0x00);/// ����
            ShowString(1,1,"Temperature is out of range!");///��ʾ�¶ȱ���
            ShowString(4,5,"Press RUN Key to Ready");///����run�����������Ԥ��
        }
    }

    if((BME_Humidity>99)||(BME_Humidity<0.0001))///ʪ�ȹ����ж�
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure,&BME_Temperature,&BME_Humidity);
        if((BME_Humidity>100)||(BME_Humidity<0.0001))///ʪ�ȹ����ж�
        {
            FlagHumi=1;
            FlagFault=1;
            LcmClear(0x00);/// ����
            ShowString(2,1,"Humidity is out of range!");///��ʾʪ�ȱ���
            ShowString(4,5,"Press RUN Key to Ready");///����run�����������Ԥ��
        }
    }

    if((FlagAtmo==1)&&(FlagHumi==1)&&(FlagTemp==1))///��������ж�
    {
        LcmClear(0x00);/// ����
        ShowString(2,1,"Three-in-one sensor fault!");///��ʾ����һ����������
        ShowString(4,5,"Press RUN Key to Ready");///����run�����������Ԥ��
    }

    /**
      ����Ԥ�����棬�ص���������
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
  *@detail �����Լ�飬�򿪵�ŷ�1��3��4��5��6
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
