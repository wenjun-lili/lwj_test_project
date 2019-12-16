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
  *@breif  ���׸�λ
	*@param  none
	*@retval none
	*/
void QiGuang_Reset(void)
{
    EV_Open(1);
    Delay100ms(1);
    Motor1Start(1, 299);	//��λΪ��λ
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();
    EV_Close(1);
    Delay100ms(1);
}

void Get_CO2_GAS(void)
{
    EV_Open(3);  // ������
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}


void Paiqi_R_PROM(void)  // ��ϴCO����
{
    Open_paiqi();

    Daiji(100);//  ��ϴ����100S

    close_paiqi( );

}
void Paiqi_RRR_PROM(void)  // ��ϴCO����
{
    Open_paiqi();

    Daiji(30);//  ��ϴ����30S

    close_paiqi( );

}

// ����CO2_RCONC Ϊ̨����
void Celiang_ZhunBei( uint16_t *ptr)
{
    QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();

    Get_CO2_GAS();// ����������������CO2����

    Daiji(30);//  ����CO2���ң�����ʱ30S

    ADC(ptr);// AD ����

}
/**
  *@breif ����CO2������ƽ������Ũ��
	*@param zero��������ƽ
	        ptr��������ƽ
	*@return none
*/
void Celiang_ZhunBeiNew(uint16_t *Zero, uint16_t *ptr)
{
    QiGuang_Reset();

    Open_CO2_Paiqi();

    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();

    ADC2_ValConvt(Zero);

    Get_CO2_GAS();// ����������������CO2����

    Daiji(30);//  ����CO2���ң�����ʱ30S

    ADC2_ValConvt(ptr);

}



void DaoQi_Y(void)
{
    EV_Open(3);  // ������
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}
void DaoQi_B(void)
{
    EV_Open(4);  // ������
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(6);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);
}
/**
  *@breif  ��ϴCO2������
	*@param  none
	*@retval none
  */
void Paiqi_RR_PROM(void)
{
    Open_CO2_Paiqi();
    Daiji(10);// ����10S,����
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
    EV_Open(5);  //������������ ��ŷ���6��Ϊ5
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

    EV_Open(1); // CO����
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}
void Chou_YGas(void)
{
    EV_Open(4);  //A��׼��COͨ��
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(1); // CO����
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(1);
    Delay100ms(1);
}

void Get_BGas(void)
{
    EV_Open(3);  // ������
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(3);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);
}

void Get_ZGas(void)
{
    EV_Open(4);  // ������
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(4);
    Delay100ms(1);

    EV_Open(5);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(5);
    Delay100ms(1);

}

void Get_Zerro_Gas(void)
{
    EV_Open(6);  // ������   ��Ϊ��ŷ�6  20180323
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(6);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
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

    ADC2_ValConvt(&CO2_PB_1);//����32�Դ�ADC

    if(F_DC==1)
    {
        printf("CO2��ƽ=%d\r\n", CO2_PB_1);
    }

    ADC(&CO_PB);
    CO_cha = abs(CO_PB - CO_ZERO);
    if(F_DC==1)
    {
        printf("��ƽ��=%d\r\n", CO_cha);
    }

    CO2_CONC = ((float)(CO2_PB_1 - CO2_ZERO)) / CO2_Taijie;
    if(F_DC==1)
    {
        printf("CO2Ũ��=%f\r\n", CO2_CONC);
    }

    if(((CO2_PB_1 < 300) || (CO2_PB_1 > 700)) || (CO_cha > 1000))
    {
        if(((CO2_PB_1 < 300) || (CO2_PB_1 > 700)) && (CO_cha > 1000))
        {
            if(CO2_CONC >= 0.000001)
            {
                temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
                temp[1] = (uint16_t)(CO2_CONC * 100);
                Wr_82_Scr(0x0022, temp, 2); //��CO2ֵ���
            }
            temp[0] = CO_cha >> 8;
            temp[1] = CO_cha;
            Wr_82_Scr(0x0002, temp, 2); //��COֵ���
            temp[0] = 0x00;
            temp[1] = 0x3A;
            Wr_80_Scr(0x03, temp, 2); //��02Ԥ�Ƚ�������58CO��CO2���ͬʱ�������
            if(F_DC==1)
            {
                printf("Ԥ��δͨ��\r\n");
            }
        }
        else if((CO2_PB_1 < 300) || (CO2_PB_1 > 700))
        {
            if(CO2_CONC >= 0.000001)
            {
                temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
                temp[1] = (uint16_t)(CO2_CONC * 100);
                Wr_82_Scr(0x0022, temp, 2); //��CO2ֵ���
            }
            temp[0] = 0x00;
            temp[1] = 0x39;
            Wr_80_Scr(0x03, temp, 2); //��02Ԥ�Ƚ�������57CO2��㱨�����
            if(F_DC==1)
            {
                printf("Ԥ��δͨ��\r\n");
            }
        }
        else if(CO_cha > 1000)
        {
            temp[0] = CO_cha >> 8;
            temp[1] = CO_cha;
            Wr_82_Scr(0x0002, temp, 2); //��COֵ���
            temp[0] = 0x00;
            temp[1] = 0x37;
            Wr_80_Scr(0x03, temp, 2); //��02Ԥ�Ƚ�������55COԤ����㱨�����
            if(F_DC==1)
            {
                printf("Ԥ��δͨ��\r\n");
            }
        }
        while(1)
        {
            if(BtnPressedFlag == 0xAA) //����ȷ�ϼ���ֱ������Ԥ�Ƚ���
            {
                BtnPressedFlag = 0;
                break;
            }
        }
    }
}
/**
  *@breif  ��Ȧʹ�ô���Ԥ��
	*@detail ʹ�ô�������2000�򱨴�
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
        Wr_80_Scr(0x03, temp, 2); //��02Ԥ�Ƚ�������63��Ȧ�������
        if(F_DC==1)
        {
            printf("��Ȧʹ�ô���Ԥ��\r\n");
        }
        while(1)
        {
            if(BtnPressedFlag == 0xAA) //����ȷ�ϼ���ֱ������Ԥ�Ƚ���
            {
                BtnPressedFlag = 0;
                break;
            }
        }
    }
}

/**
  *@breif  ����һ�������Լ�͸�����Լ�
  *@datail ����һ����������Ԥ������Ҫ��������ǣ����¶ȵ�λ���棻ʪ�ȵ�λ��%����ѹ��λ��hPa��
           ��1������ѹ����[500 1100]��Χ�ڣ����ٴβ��������ڷ�Χ��������һ��������
           ��2�����¶Ȳ���(0 40)֮�䣬���ٴβ������Ǵ��ڲ��ڷ�Χ��������һ��������
           ��3����ʪ�Ȳ���(0,100]֮�䣬���ٴβ������ǲ��ڷ�Χ��������һ��������
           ���������Ч���Լ�
		 ��1����ʪ�ȴ���40������£������ʾ�����ʧЧ
  *@prarm  none
  *@retval none
  */
void Bme280Fault(void)
{
    uint8_t temp[2];

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity); //����ʪ�ȡ��¶ȡ���ѹֵ

    if(F_DC == 1)
    {
        printf("BME280\r\n");
        printf("Ԥ��ʪ��=%f\r\n", BME_Humidity);
        printf("Ԥ���¶�=%f\r\n", BME_Temperature);
        printf("Ԥ����ѹ=%f\r\n", BME_Pressure);
    }
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x000A, temp, 2);

    if((BME_Pressure > 1100) || (BME_Pressure < 500))
    {
        Delay100ms(10);//�ȴ�1s

        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity); //��һ�β���ʪ�ȡ��¶ȡ���ѹֵ

        if((BME_Pressure > 1100) || (BME_Pressure < 500))
        {
            temp[0] = 0x00;
            temp[1] = 0x4C;
            Wr_80_Scr(0x03, temp, 2); //����76��ѹ�Լ����
            if(F_DC == 1)
            {
                printf("��ѹ����\r\n");
            }

            while(1)
            {
                if(BtnPressedFlag == 0xAA) //����ȷ�ϼ�
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }

        }
    }
    if((BME_Temperature > 35) || (BME_Temperature < 0.0001)) ///�¶ȹ����ж�
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        if((BME_Temperature > 40) || (BME_Temperature < 0.0001))
        {
            temp[0] = 0x00;
            temp[1] = 0x4B;
            Wr_80_Scr(0x03, temp, 2); //����75�¶��Լ����
            if(F_DC == 1)
            {
                printf("�¶ȱ���\r\n");
            }

            while(1)
            {
                if(BtnPressedFlag == 0xAA) //����ȷ�ϼ�
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }
        }
    }

    if((BME_Humidity > 99) || (BME_Humidity < 0.0001)) ///ʪ�ȹ����ж�
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        if((BME_Humidity > 100) || (BME_Humidity < 0.0001)) ///ʪ�ȹ����ж�
        {
            temp[0] = 0x00;
            temp[1] = 0x4A;
            Wr_80_Scr(0x03, temp, 2); //����74ʪ���Լ����
            if(F_DC == 1)
            {
                printf("ʪ�ȱ���\r\n");
            }

            while(1)
            {
                if(BtnPressedFlag == 0xAA) //����ȷ�ϼ�
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }
        }
    }
}

/**
  *@breif  ������ϴ����
	*@param  none
	*@retval none
	*/
void Purge_Cylinder(void)
{
    EV_Open(7);  // �򿪵��߸���ŷ�
    Delay100ms(1);

    Motor1Start(0, 299);	/*  ���� �ҵ�λ */
    while(!GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_7));
    Motor1Stop();

    EV_Close(7);
    Delay100ms(1);

    EV_Open(2);
    Delay100ms(1);

    Motor1Start(1, 299);	/* ���� ��λ*/
    while(!GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5));
    Motor1Stop();

    EV_Close(2);
    Delay100ms(1);
}
/**
  *@breif  �ɼ�����
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
