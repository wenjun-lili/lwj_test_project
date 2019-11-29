#include "framedisplay.h"
#include "bsp.h"


uint16_t Warming_up_Cnt;
extern struct rtc_time systmtime;

/**
  * @brief  framediaplay0(void)
	* @datail Onilining��ʾ
	* @param  none
	* @return none
  */
void framediaplay0(void)
{
    uint8_t i;
    LcmClear(0x00);// ����
    ShowString( 0x01,0x01, "Onlining");
    for(i=0; i<16; i++)
    {
        ShowString( 0x0A+i,1, ".");
        Delay1ms(200); // 200ms
        if(i==8)
        {
            if(ReceiveFlag1==0)
            {
                RBCtoPC_Cmd_AA();//����0xAA�����ź�
            }
        }
    }
}
/**
  * @brief  framedisplay1(void)
	* @datail Ԥ�Ƚ�����ʾ
	* @param  none
	* @return none
  */
void framedisplay1(void)
{
    LcmClear(0x00);// ����
    ShowString( 1,1, "Warming up.......");
    ShowString(0x17,0x07,"(    )");
    if(F_PC==1)
    {
        FS_INT_ON();// ��ʼ��������
    }

    Warming_up_Cnt=1200;
    ShowString(0x17,0x07,"(    )");
    ShowString(0x18,0x07, Num2StrI(Warming_up_Cnt, "%4d"));
    TIM_Cmd(TIM7, ENABLE);

    Open_CO2_Paiqi(); // ����   ��E2��E7�����ù���

    while(1)
    {
        if(Warming_up_Cnt==0)
        {
            TIM_Cmd(TIM7, DISABLE);
            break;
        }
        if(Warming_up_Cnt==1170)// ����ʱ
        {
            EV_Open(1); //�ȿ�CO���ҵ�ŷ�
            Delay100ms(1);
            EV_Close(2);//�ٹ�CO2��ŷ�
            Delay100ms(1);
        }
        if(F_tiaoji==1) // �������л�����������
        {
            if(BtnPressedFlag!=-1)
            {
                BtnPressedFlag=-1;
                TIM_Cmd(TIM7, DISABLE);
                break;
            }
        }
    }
    framedisplay_ok();

    close_paiqi(); //�ر�CO����
    EV_Close(2); //�ڵ���������ȷ���ر�CO2����
    Delay100ms(1);

    if(F_PC==1)
    {
        Send_Gas_Com(); //����״̬�£�����������״̬����
        FS_INT_OK();// ��ʼ��OK
    }

}
/**
  * @brief  framedisplay_ok(void)
	* @datail OK��ʾ
	* @param  none
	* @return none
  */
void framedisplay_ok(void)
{
    ShowString(0x17,0x07,"( ok )");
}
/**
  * @brief  framedisplay3(void)
	* @datail ����������ʾ
	* @param  none
	* @return none
  */
void framedisplay3(void)
{

    LcmClear(0x00);// ����
    ShowString(0x00,0x03,"XYRBCS_01 RBCS_TEST_Instrument");

    if(F_tiaoji==1)
    {
        ShowString(0x15,0x07,"ST QU RUN");
    }
    else
    {
        ShowString(0x01,0x07,"press INPUT key to input HB");
    }
    ShowString(0x01,0x01,"ZERO:");
    ShowString(0x01,0x05,"    -  -     :  :  ");



}
/**
  * @brief  framedisplay3_sub(void)
	* @datail �������涯̬������ʾ
	* @param  none
	* @return none
  */
void framedisplay3_sub(void)
{
#if 0
    static  uint16_t  CO_RZERO;
    uint16_t  CO_RZERO_temp;
    static uint8_t k=0;
    int16_t CO_cha;

    RTCTimeTransfer(&systmtime);
    ShowString(0x01,0x05,Num2StrI(systmtime.tm_year,"%04d"));// 0x0E������ĸ�λ
    ShowString(0x06,0x05,Num2StrI(systmtime.tm_mon,"%02d"));
    ShowString(0x09,0x05,Num2StrI(systmtime.tm_mday,"%02d"));

    ShowString(0x0C,0x05,Num2StrI(systmtime.tm_hour,"%02d"));
    ShowString(0x0F,0x05,Num2StrI(systmtime.tm_min,"%02d"));
    ShowString(0x12,0x05,Num2StrI(systmtime.tm_sec,"%02d"));


    SELET_CO();//ģ�⿪�ش�COģ���ź�

    if(k==0) // ��һ�β�������CO_RZERO��ֵ
    {
        ADC(&CO_RZERO);
        k++;
    }
    CO_RZERO_temp=CO_RZERO;
    ADC(&CO_RZERO);

    if(abs(CO_RZERO_temp-CO_RZERO)>5)
    {
        ShowString(0x0E,0x01,"!!");
    }
    else
    {
        ShowString(0x0E,0x01,"  ");
    }

//  printf("CO zero real test value:%d\r\n",CO_RZERO);// ������

    CO_cha=abs(CO_RZERO-CO_ZERO);
    ShowString(0x06,0x01,Num2StrI(CO_cha,"%06d"));

    if(F_PC==1)
    {
        ShowString(0x1A,0x01,"(**)");
        FS_READY();// ��PC�����Ϳ���ָ��
    }

    ShowString(0x13,0x01,"Ready!");


#else
    uint16_t  CO_RZERO;
    int16_t CO_cha;

    RTCTimeTransfer(&systmtime);
    ShowString(0x01,0x05,Num2StrI(systmtime.tm_year,"%04d"));// 0x0E������ĸ�λ
    ShowString(0x06,0x05,Num2StrI(systmtime.tm_mon,"%02d"));
    ShowString(0x09,0x05,Num2StrI(systmtime.tm_mday,"%02d"));

    ShowString(0x0C,0x05,Num2StrI(systmtime.tm_hour,"%02d"));
    ShowString(0x0F,0x05,Num2StrI(systmtime.tm_min,"%02d"));
    ShowString(0x12,0x05,Num2StrI(systmtime.tm_sec,"%02d"));


    SELET_CO();//ģ�⿪�ش�COģ���ź�

    ADC(&CO_RZERO);

//  printf("CO zero real test value:%d\r\n",CO_RZERO);// ������

    CO_cha=CO_RZERO-CO_ZERO;
//		printf("Z1:%d\r\n",CO_cha);//�������ֵ�����ڳ��ڹ۲�
    if(F_Zero==1)
    {
        printf("Z1:%d\r\n",CO_cha);//�������ֵ�����ڳ��ڹ۲�
    }
    if(CO_cha>=0)
    {
        ShowString(0x06,0x01,Num2StrI(CO_cha,"%06d"));
    }
    else
    {
        ShowString(0x06,0x01,"-");
        ShowString(0x07,0x01,Num2StrI(abs(CO_cha),"%05d"));
    }

    if(F_PC==1)
    {
        ShowString(0x1A,0x01,"(**)");
//        FS_READY();// ��PC�����Ϳ���ָ��
        //  FS_ZERO(&CO_cha);
    }

    ShowString(0x13,0x01,"Ready!");
#endif
}
/**
  * @brief  framedisplayDate(void)
	* @datail �������ý���
	* @param  none
	* @return none
  */
void framedisplayDate(void)
{
    LcmClear(0x00);// ����
    ShowString(0x00,0x01,"press UP or DN and RT or LT tO");
    ShowString(0x00,0x03,"set Date(Y/M/D)");
    ShowString(0x04,0x06,"Date:      /   /  ");

    /* ��ʾ��ǰʱ�� */
    RTCTimeTransfer(&systmtime);
    // ����2���ַ�
    ShowString(0x0B,0x06,Num2StrI(systmtime.tm_year,"%04d"));// 0x0E������ĸ�λ
    ShowString(0x11,0x06,Num2StrI(systmtime.tm_mon,"%02d"));
    ShowString(0x15,0x06,Num2StrI(systmtime.tm_mday,"%02d"));

    LcmWriteCommandWith2Par(0x0E,0x06,0x21); // д����
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ

    cursorx=0x0E;
    cursory=0x06;
}
/**
  * @brief  framedisplaytime(void)
	* @datail ʱ�����ý���
	* @param  none
	* @return none
  */
void framedisplaytime(void)
{
    LcmClear(0x00);// ����
    ShowString(0x00,0x01,"press UP or DN and RT or LT tO");
    ShowString(0x00,0x03,"set Time(HR/MIN/SEC)");
    ShowString(0x04,0x06,"Time:    :   :");

    /* ��ʾ��ǰʱ�� */
    RTCTimeTransfer(&systmtime);
    ShowString(0x0B,0x06,Num2StrI(systmtime.tm_hour,"%02d"));
    ShowString(0x0F,0x06,Num2StrI(systmtime.tm_min,"%02d"));
    ShowString(0x13,0x06,Num2StrI(systmtime.tm_sec,"%02d"));

    LcmWriteCommandWith2Par(0x0C,0x06,0x21); // д����
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ

    cursorx=0x0C;
    cursory=0x06;
}
/**
  * @brief  framedisplayZERO(void)
	* @datail ���ZERO���ý���
	* @param  none
	* @return none
  */
void framedisplayZERO(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"ZERO:");
    ShowString(0x07,0x03,Num2StrI(CO_ZERO,"%06d"));
    ShowString(0x0c,0x04,"+-RUN");
    LcmWriteCommandWith2Par(0x0C,0x03,0x21); // д����
}
/**
  * @brief  framedisplayCO2(void)
	* @datail CO2��ֵ���ý���
	* @param  none
	* @return none
  */
void framedisplayCO2(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"CO2:    %");
    ShowString(0x07,0x03,Num2StrI(CO2_PB,"%03d"));
    ShowString(0x0c,0x04,"+-RUN");
    LcmWriteCommandWith2Par(0x09,0x03,0x21); // д����
}
/**
  * @brief  framedisplayX_B(void)
	* @datail X-B���ý���
	* @param  none
	* @return none
  */
void framedisplayX_B(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"X_B:");
    ShowString(0x0c,0x04,"+-RUN");
    ShowString(0x07,0x03,Num2StrI(R_XB,"%06d"));
    LcmWriteCommandWith2Par(0x0C,0x03,0x21); // д����
}
/**
  * @brief  framedisplayGasDefault(void)
	* @param  none
	* @return none
  */
void framedisplayGasDefault(void)
{
    LcmClear(0x00);//����
		ShowString(0x02,0x03,"Sample type:");
		ShowString(0x0c,0x04,"+-RUN");
		if(F_GasType==1)
		{
		  ShowString(0x0E,0x03,"Auto Gas");
		}
		else
		{
		  ShowString(0x0E,0x03,"Blow    ");
		}
			
}

/**
  * @brief  framedisplayGasFactor(void)
	* @param  none
	* @return none
  */
void framedisplayGasFactor(void)
{
    LcmClear(0x00);//����
		ShowString(0x02,0x03,"Gas factor:");
		ShowString(0x0c,0x04,"+-RUN");
		ShowString(0xE,0x03,Num2StrI(gasTypeFactor,"%03d"));			
}
/**
  * @brief  framedisplayCO2Coeff(void)
	* @datail CO2ϵ����ʾ����
	* @param  none
	* @return none
  */
void framedisplayCO2Coeff(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"CO2_Coeff:");
    ShowString(0x0E,0x03,Num2StrI(CO2_Coeff,"%06d"));
    LcmWriteCommandWith2Par(0x0E,0x03,0x21); // д����
}
/**
  * @brief  framedisplayATMCoeff(void)
	* @datail ��ѹ����ϵ����ʾ
	* @param  none
	* @return none
  */
void framedisplayATMCoeff(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"ATM_Coeff:");
    ShowString(0x0E,0x03,Num2StrI(ATM_Coeff,"%06d"));
    LcmWriteCommandWith2Par(0x0E,0x03,0x21); // д����
}
/**
  * @brief  framedisplayVersion(void)
	* @datail �汾����ʾ����
	* @param  none
	* @return none
  */
void framedisplayVersion(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"Version: V1.4.4");//20190215
}

/**
  * @brief  framedisplayH0(void)
	* @datail ���òο����θ߶�
	* @param  none
	* @return none
  */
void framedisplayH0(void)
{
    LcmClear(0x00);
    ShowString(0x02,0x03,"H0:     m");
    ShowString(0x05,0x03,Num2StrI(H0,"%05d"));
    LcmWriteCommandWith2Par(0x09,0x03,0x21); // д����
}

/**
  * @brief  framedisplayT0(void)
	* @datail ���òο��¶�
	* @param  none
	* @return none
  */
void framedisplayT0(void)
{
    LcmClear(0x00);
    ShowString(0x02,0x03,"T0:      ");
    ShowString(0x05,0x03,Num2StrF(T0,"%2.1f"));
    LcmWriteCommandWith2Par(0x08,0x03,0x21); // д����
}

/**
  * @brief  framedisplayRubberRing(void)
	* @datail ��Ȧʹ�ô�����ʾ
	* @param  none
	* @return none
  */
void framedisplayRubberRing(void)
{
    LcmClear(0x00);// ����
    ee_ReadBytes((uint8_t *)&Ring_Num,10,2);
    ShowString(0x02,0x03,"Rubber Ring:");
    ShowString(0x0F,0x03,Num2StrI(Ring_Num,"%04d"));
    LcmWriteCommandWith2Par(0x0F,0x03,0x21); // д����
}
/**
  * @brief  framedisplayZEROCO2(void)
	* @datail CO2���̨������ʾ
	* @param  none
	* @return none
  */
void framedisplayZEROCO2(void)
{
    LcmClear(0x00);// ����
    ShowString(0x01,0x03,"CO2_ZERO:");
    ShowString(0x0B,0x03,Num2StrI(CO2_ZERO,"%06d"));
    LcmWriteCommandWith2Par(0x10,0x03,0x21); // д����
}
/**
  * @brief  framedisplayRun(void)
	* @datail HB���������ʾ
	* @param  none
	* @return none
  */
void framedisplayRun(void)
{
    LcmClear(0x00);// ����
    ShowString(0x01,0x02,"Press Up or DN to input HB:");
    ShowString(0x01,0x06,"Press RUN key to start");
    ShowString(0x01,0x07,"testing");
    ShowString(0x02,0x04,"HB:        (g/L)");
    ShowString(0x05,0x04,Num2StrI(HB,"%03d"));
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
    LcmWriteCommandWith2Par(0x07,0x04,0x21); // д����
}
/**
  * @brief  Dis_Sample_Test(void)
	* @datail ����������ʾ
	* @param  none
	* @return none
  */
void Dis_Sample_Test(void)
{
    LcmClear(0x00);// ����
    ShowString(0x01,0x01,"Sample_Testing:");
}
/**
  * @brief  framedisplayQC(void)
	* @datail �ʿز����������
	* @param  none
	* @return none
  */
void framedisplayQC(void)
{
    LcmClear(0x00);// ����
//	ShowString(0x01,0x01,"Sample_Testing:");
//	ShowString(0x01,0x06,"press RUN key to start");
    ShowString(0x00,0x00,"Press UP or DN to input QC_GAS");
    ShowString(0x00,0x01,"Difference");
    ShowString(0x00,0x05,"Press RUN to cmpl QC or Press LT to stop QC and Return to Ready!");


//	ShowString(0x00,0x07, "Ready!");
    ShowString(0x02,0x03,"QC_CO Gas:");
    ShowString(0x0C,0x03,Num2StrI(QC_CONC,"%06d"));
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
    LcmWriteCommandWith2Par(0x11,0x03,0x21); // д����

}
/**
  * @brief  framedisplayQCCO2(void)
	* @datail �ʿ�CO2�������
	* @param  none
	* @return none
  */
void framedisplayQCCO2(void)
{
    LcmClear(0x00);// ����
//	ShowString(0x01,0x01,"Sample_Testing:");
//	ShowString(0x01,0x06,"press RUN key to start");
    ShowString(0x00,0x00,"Press UP or DN to input QC_GAS Difference");
    ShowString(0x00,0x01,"Difference");
    ShowString(0x00,0x05,"Press RUN to cmpl QC or Press LT to stop QC and Return to Ready!");

//	ShowString(0x00,0x07, "Ready!");
    ShowString(0x02,0x03,"QC_CO2 Gas:");
    ShowString(0x0D,0x03,Num2StrI(QC_CO2_CONC,"%06d"));
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
    LcmWriteCommandWith2Par(0x12,0x03,0x21); // д����
}
/**
  * @brief  framdisplayCurve(void)
	* @datail �궨COŨ���������
	* @param  none
	* @return none
  */
void framdisplayCurve(void)
{
    // �������ñ�׼��CO��Ũ��
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"CO_ppm:");
    ShowString(0x0c,0x04,"+-RUN");
    ShowString(0x0A,0x03,Num2StrI(QC_CONC,"%06d"));
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
    LcmWriteCommandWith2Par(0x0F,0x03,0x21); // д����
}
/**
  * @brief  framdisplayZeroFault(void)
	* @datail ZERO_Fault��ʾ����
	* @param  none
	* @return none
  */
void framdisplayZeroFault(void)
{
    LcmClear(0x00);// ����
    ShowString(0x0A,0x03,"ZERO_Fault!");
    ShowString(0x00,0x06,"Remove all bags,return to ");
    ShowString(0x00,0x07,"Ready!");
}

/**
  * @brief  framedisplayHumidity(void)
	* @datail CO2��ֵ���ý���
	* @param  none
	* @return none
  */
void framedisplayHumidity(void)
{
    LcmClear(0x00);// ����
    ShowString(0x02,0x03,"Humidity threshold:   %");
    ShowString(0x16,0x03,Num2StrI(HumidityThreshold,"%02d"));
    ShowString(0x0c,0x04,"+-RUN");
    LcmWriteCommandWith2Par(0x09,0x03,0x16); // д����
}
/**
  *@brief  framedisplayAirTight()
  *@datail �����Լ������
  *@param  void
  *@retval void
  */
void framedisplayAirTight(void)
{
    LcmClear(0x00);// ����
    ShowString(0x03,0x03,"AIR TIGHT TEST");
}

/**
  *@brief framedisplaySex(uint8_t m)
  *@datail �����Ա����
  *@param m:�Ա�
  *@retval void
  */
void framedisplaySex(uint8_t m)
{
    LcmClear(0x00);// ����
    ShowString(0x01,0x02,"Press Up to select gender:");
    ShowString(0x01,0x04,"Gender:       ");
    if(m==0)
    {
        ShowString(0x09,0x04,"Female");
    }
    else
    {
        ShowString(0x09,0x04," Male ");
    }
    ShowString(0x14,0x06,"+-RUN");
    LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
    LcmWriteCommandWith2Par(0x0E,0x04,0x21); // д����
}