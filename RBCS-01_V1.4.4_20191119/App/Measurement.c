/**
  * @file  Measurement.c
	* @brief �������ʿء��궨�����ļ�
	*/
#include "Measurement.h"
#include "bsp.h"
#include "BME280.h"


extern struct rtc_time systmtime;
extern uint8_t RetFrame3Flag;

/**
  * @brief  Chu_Zhi(void)
	* @datail ������ʼ��
	* @param  none
	* @return none
	*/
void Chu_Zhi(void)
{
    systmtime.tm_year = 2008;
    systmtime.tm_mon = 5;
    systmtime.tm_mday = 12;
    systmtime.tm_hour = 14;
    systmtime.tm_min = 28;
    systmtime.tm_sec = 23;
    RTC_Configuration();
    Time_Adjust(&systmtime); // ������ʱ�����ʼֵ����

    CO_ZERO = 2645; //CO������ֵ2645

    R_XB = 400; // ����ϵ�� ������10000

    QC_CONC = 400; // �ʿ�����COŨ�Ȳ� 4ppm*100

    Curve_CONC = 400; // �����߱���COŨ�Ȳ� ppm*100

    CO2_PB = 2; // CO2����ֵ2%

    CO2_ZERO = 500;

    CO2_Coeff = 100; // 1%��CO2̨����300

    CO2_B = 400; //CO2��׼����Ũ�Ȳ�

    Ring_Num = 0; //��Ȧʹ�ô�������

    T0_Var = 280;

    H0 = 65;
    F_GasType = 0;
    gasTypeFactor = 135;

    HumidityThreshold = 40;
    ee_WriteBytes((uint8_t *)&CO_ZERO, 0, 2);
    ee_WriteBytes((uint8_t *)&CO2_PB, 2, 2);
    ee_WriteBytes((uint8_t *)&R_XB, 4, 2);
    ee_WriteBytes((uint8_t *)&CO2_Coeff, 6, 2);
    ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
    ee_WriteBytes((uint8_t *)&F_GasType, 14, 2);
    ee_WriteBytes((uint8_t *)&T0_Var, 16, 2);
    ee_WriteBytes((uint8_t *)&gasTypeFactor, 20, 2);
    ee_WriteBytes((uint8_t *)&HumidityThreshold, 18, 2);
}
/**
  * @brief  OnlinePC(void)
	* @datail ����
	* @param  none
	* @return none
	*/
void OnlinePC (void)
{
    uint16_t numm = 0;

    framediaplay0();// ��ʾ������ʾOnling������������			ͨ�����ڷ���0xAA   �ȴ����Իظ�

    while(!ReceiveFlag)
    {
        numm++;
        Delay10ms(1);
        if(numm > 150) //1.5sδ���յ��ظ���������
        {
            break;
        }
    }
    if(ReceiveFlag == 1) // ����������������
    {
        ReceiveFlag = 0;
        if(ReceiveFlag1 == 0)
        {
            switch(RBC_pc_ComR[0])
            {
            case 0x99:
                RBC_pc_ComR[0] = 0;
                F_DC = 1;
                USART_SendData(USART1, (uint8_t)0xAD);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
                break;
            case 0xBB:
                RBC_pc_ComR[0] = 0; // ��0
                F_PC = 1;
                GPIO_SetBits( GPIOE,  GPIO_Pin_6);//ONLINE ָʾ����
                break;
            case 0xEE:
                RBC_pc_ComR[0] = 0;
                F_Zero = 1;
                break;
            default:
                F_PC = 0;
                GPIO_ResetBits( GPIOE,  GPIO_Pin_6);//ONLINE ָʾ����
                break;
            }
        }
    }
    if(F_PC == 1)
    {
        ShowString(0x1A, 0x01, "(**)");
    }
    else
    {
        ShowString(0x1A, 0x01, "    ");
    }
    ReceiveFlag1 = 0;

}

void date_add(void)
{   // �տ�ʼ�������ĸ�λ��

    if((cursorx == 0x0E) && (cursory == 0x05)) // ��
    {
        systmtime.tm_year += 1;
        ShowString(cursorx - 3, cursory, Num2StrI(systmtime.tm_year, "%04d"));
    }
    else if((cursorx == 0x12) && (cursory == 0x05)) // ��
    {
        if(systmtime.tm_mon != 12)
        {
            systmtime.tm_mon += 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_mon, "%02d"));
    }
    else if((cursorx == 0x16) && (cursory == 0x05)) //��
    {
        if(systmtime.tm_mon == 2)
        {
            if(systmtime.tm_mday < 29)
            {
                systmtime.tm_mday += 1;
            }
        }
        else if((systmtime.tm_mon == 4) | (systmtime.tm_mon == 6) | (systmtime.tm_mon == 9) | (systmtime.tm_mon == 11))
        {
            if(systmtime.tm_mday < 30)
            {
                systmtime.tm_mday += 1;
            }
        }
        else
        {
            if(systmtime.tm_mday < 31)
            {
                systmtime.tm_mday += 1;
            }
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_mday, "%02d"));
    }

}

void date_dec(void)
{
    if((cursorx == 0x0E) && (cursory == 0x05)) // ��
    {
        if(systmtime.tm_year != 0)
        {
            systmtime.tm_year -= 1;
        }
        ShowString(cursorx - 3, cursory, Num2StrI(systmtime.tm_year, "%04d"));
    }
    else if((cursorx == 0x12) && (cursory == 0x05)) // ��
    {
        if(systmtime.tm_mon != 1)
        {
            systmtime.tm_mon -= 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_mon, "%02d"));

    }
    else if((cursorx == 0x16) && (cursory == 0x05)) //��
    {
        if(systmtime.tm_mday > 1)
        {
            systmtime.tm_mday -= 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_mday, "%02d"));
    }
}

void time_add(void)
{
    if((cursorx == 0x0C) && (cursory == 0x05)) // hours
    {
        if(systmtime.tm_hour != 23)
        {
            systmtime.tm_hour += 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_hour, "%02d"));
    }
    else if((cursorx == 0x10) && (cursory == 0x05)) // min
    {
        if(systmtime.tm_min != 59)
        {
            systmtime.tm_min += 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_min, "%02d"));

    }

    else if((cursorx == 0x14) && (cursory == 0x05)) // sec
    {
        if(systmtime.tm_sec != 59)
        {
            systmtime.tm_sec += 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_sec, "%02d"));
    }

}

void time_dec(void)
{
    if((cursorx == 0x0C) && (cursory == 0x05)) // hours
    {
        if(systmtime.tm_hour != 0)
        {
            systmtime.tm_hour -= 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_hour, "%02d"));
    }
    else if((cursorx == 0x10) && (cursory == 0x05)) // min
    {
        if(systmtime.tm_min != 0)
        {
            systmtime.tm_min -= 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_min, "%02d"));

    }

    else if((cursorx == 0x14) && (cursory == 0x05)) // sec
    {
        if(systmtime.tm_sec != 0)
        {
            systmtime.tm_sec -= 1;
        }
        ShowString(cursorx - 1, cursory, Num2StrI(systmtime.tm_sec, "%02d"));
    }
}
/**
  * @brief   Set_Para(void)
  * @datail  ���ò���
  * @param   none
  * @return  none
  */
void Set_Para(void)
{
    //1. ��ʾ����
    framedisplayDate();

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1: // L
            BtnPressedFlag = -1;
            if(cursorx != 0x0E)
            {
                cursorx -= 4;
            }
            LcmWriteCommandWith2Par(cursorx, cursory, 0x21); // д����
            break;

        case 6: // -
            BtnPressedFlag = -1;
            date_dec();
            break;
        case 7: // +
            BtnPressedFlag = -1;
            date_add();
            break;
        case 5: // R
            BtnPressedFlag = -1;
            if(cursorx != 0x16)
            {
                cursorx += 4;
            }
            LcmWriteCommandWith2Par(cursorx, cursory, 0x21); // д����
            break;
        default:
            break;
        }
        if(BtnPressedFlag == 4) // SET
        {
            BtnPressedFlag = -1;
            RTC_Configuration();
            Time_Adjust(&systmtime);
            break;
        }
    }

    // 2.��ʾʱ��
    framedisplaytime();

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            if(cursorx != 0x0C)
            {
                cursorx -= 4;
            }
            LcmWriteCommandWith2Par(cursorx, cursory, 0x21); // д����
            break;
        case 6://-
            BtnPressedFlag = -1;
            time_dec();
            break;
        case 7://+
            BtnPressedFlag = -1;
            time_add();
            break;
        case 5://R
            BtnPressedFlag = -1;
            if(cursorx != 0x14)
            {
                cursorx += 4;
            }
            LcmWriteCommandWith2Par(cursorx, cursory, 0x21); // д����
            break;
        default:
            break;
        }
        if(BtnPressedFlag == 4) // SET
        {
            BtnPressedFlag = -1;
            RTC_Configuration();
            Time_Adjust(&systmtime);
            break;

        }
    }
   // 3.����Ĭ����������
    framedisplayGasDefault();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 6://-
            BtnPressedFlag = -1;
            F_GasType = !F_GasType;
            break;
        case 7://+
            BtnPressedFlag = -1;
            F_GasType = !F_GasType;
            break;
        default:
            break;
        }
        if(F_GasType == 1)
        {
            ShowString(0x0E, 0x04, "Auto Gas");
        }
        else
        {
            ShowString(0x0E, 0x04, "Blow    ");
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            break;
        }


    }
    // �����û����򣬺���Ĳ��������ã�����ֱ������SET
    if(F_tiaoji == 0)
    {
        RetFrame3Flag = 1;
        LcmWriteCommand(0x9C);	// ��ʾ�������ã��ı�����ͼ�ο��������˸��
        return ;
    }

    //4. �������ZERO

    framedisplayZERO();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(CO_ZERO != 0)
            {
                CO_ZERO -= 1;
            }
            ShowString(0x07, 0x04, Num2StrI(CO_ZERO, "%06d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(CO_ZERO != 4095)
                CO_ZERO += 1;
            ShowString(0x07, 0x04, Num2StrI(CO_ZERO, "%06d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break ;
        default:
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
						LcmWriteCommand(0x9C);	// ��ʾ�������ã��ı�����ͼ�ο��������˸��
            break;
        }
    }

//    // 4.CO2Ũ���ж�ֵ����(0%~5%)
//    framedisplayCO2();
//    while(1)
//    {
//        switch(BtnPressedFlag)
//        {
//        case 1://L
//            BtnPressedFlag = -1;
//            break;
//        case 6://-
//            BtnPressedFlag = -1;
//            if(CO2_PB != 0)
//                CO2_PB -= 1;
//            ShowString(0x07, 0x03, Num2StrI(CO2_PB, "%03d"));
//            break;
//        case 7://+
//            BtnPressedFlag = -1;
//            if(CO2_PB != 5)
//                CO2_PB += 1;
//            ShowString(0x07, 0x03, Num2StrI(CO2_PB, "%03d"));
//            break;
//        case 5://R
//            BtnPressedFlag = -1;
//            break ;
//        default:
//            break;
//        }
//        if(BtnPressedFlag == 4)
//        {
//            BtnPressedFlag = -1;
//            LcmWriteCommand(0x9C);	// ��ʾ�������ã��ı�����ͼ�ο��������˸��
//            break;
//        }
//    }
 
    // 5.�޸���������ϵ��
    framedisplayGasFactor();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(gasTypeFactor != 0)
                gasTypeFactor -= 1;
            ShowString(0xE, 0x04, Num2StrI(gasTypeFactor, "%03d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(gasTypeFactor != 250)
                gasTypeFactor += 1;
            ShowString(0xE, 0x04, Num2StrI(gasTypeFactor, "%03d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break ;
        default:
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            break;
        }
    }

    // 6.�޸�CO����ϵ��XB��ֵ
    framedisplayX_B();

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(R_XB != 0)
                R_XB -= 1;
            ShowString(0x07, 0x04, Num2StrI(R_XB, "%06d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(R_XB != 1000)
                R_XB += 1;
            ShowString(0x07, 0x04, Num2StrI(R_XB, "%06d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break ;
        default:
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            break;
        }
    }

    // 7. CO2Ũ������ϵ��20180622
#if 1
    framedisplayCO2Coeff();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
//						 CO2_Taijie-=1;
//            ShowString(0x0B,0x03,Num2StrI(CO2_Coeff,"%04d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
//					  CO2_Taijie +=1;
//            ShowString(0x0E,0x03,Num2StrI(CO2_Coeff,"%04d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break ;
        default:
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;

            break;
        }
    }
#endif
    //8. ��ʾCO2���
    //�ڴ���ģʽ�£�����ʹ�� 20180620
#if 1
    framedisplayZEROCO2();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
						 CO2_ZERO-=1;
            ShowString(0x0B, 0x04, Num2StrI(CO2_ZERO, "%06d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
					   CO2_ZERO+=1;
            ShowString(0x0B, 0x04, Num2StrI(CO2_ZERO, "%06d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break ;
        default:
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            RetFrame3Flag = 1; // ��������ʾ��־

            break;
        }
    }
#endif

    /**
     9. ʪ����ֵ��������
      */
    framedisplayHumidity();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(HumidityThreshold != 0)
                HumidityThreshold -= 1;
            ShowString(0x16, 0x04, Num2StrI(HumidityThreshold, "%02d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(HumidityThreshold != 90)
                HumidityThreshold += 1;
            ShowString(0x16, 0x04, Num2StrI(HumidityThreshold, "%02d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break ;
        default:
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            LcmWriteCommand(0x9C);	// ��ʾ�������ã��ı�����ͼ�ο��������˸��
            break;
        }
    }
#if 0
    //10����ʾ��ѹ����ϵ��
    framedisplayATMCoeff();
    while(1)
    {
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;

            break;
        }
    }
#endif
    //11������汾��
    framedisplayVersion();
    while(1)
    {
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            break;
        }
    }
#if 0
    //12�����θ߶�
    framedisplayH0();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            if(H0 > 99)
                H0 -= 100;
            ShowString(0x05, 0x03, Num2StrI(H0, "%05d"));
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(H0 != 0)
                H0 -= 1;
            ShowString(0x05, 0x03, Num2StrI(H0, "%05d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(H0 != 5000)
                H0 += 1;
            ShowString(0x05, 0x03, Num2StrI(H0, "%05d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            if(H0 < 4900)
                H0 += 100;
            ShowString(0x05, 0x03, Num2StrI(H0, "%05d"));
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;

            break;
        }
    }
#endif
    //13���¶�
    framedisplayT0();
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(T0 != 0.0)
                T0 -= 0.1;
            ShowString(0x05, 0x04, Num2StrF(T0, "%2.1f"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(T0 != 40.0)
                T0 += 0.1;
            ShowString(0x05, 0x04, Num2StrF(T0, "%2.1f"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            break;
        }
    }
    //14����Ȧʹ�ô���  20180621
    framedisplayRubberRing();//��Ȧ������ʾ-lcy-20180621
    while(1)
    {
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            RetFrame3Flag = 1; // ��������ʾ��־

            break;
        }
        if(BtnPressedFlag == 5) //�����Ҽ�����
        {
            BtnPressedFlag = -1;
            Ring_Num = 0;
            ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
            break;
        }
    }
    LcmWriteCommand(0x9C);	// ��ʾ�������ã��ı�����ͼ�ο��������˸��
    T0_Var = (uint16_t)(T0 * 10);
    ee_WriteBytes((uint8_t *)&CO_ZERO, 0, 2);
    ee_WriteBytes((uint8_t *)&CO2_PB, 2, 2);
    ee_WriteBytes((uint8_t *)&R_XB, 4, 2);
    ee_WriteBytes((uint8_t *)&T0_Var, 16, 2);
    ee_WriteBytes((uint8_t *)&HumidityThreshold, 18, 2);
    ee_WriteBytes((uint8_t *)&F_GasType, 14, 2);
    ee_WriteBytes((uint8_t *)&gasTypeFactor, 20, 2);


}
/**
  * @brief  RUN_Program(void)
  * @datail ��������
  *
  * @param  none
  * @return none
  */
void RUN_Program(void)
{
    uint8_t i;
    uint8_t m;
    uint16_t CO2_AD, CO2_ZTest; // ʵ��CO2̨���� �Լ�����CO2���
    float CO2_CONC; // CO2Ũ��
    float CO2_CONC_CAL;//������Ļ��ʾ��V/VŨ��
    uint16_t CO_ZERO1, CO_ZERO2;
    uint16_t CO_AD1, CO_AD2, CO_Re1, CO_Re2;
    float CO_Cha;
    float CO_AD2_Z, CO_Re2_Z, CO_AD1_Z; //������CO��ƽ��������ƽ�Ĳ�ֵ����ʱ����
    float PcoR, Pco; // COʵ��ֵ
    float RBC_Temp;//��ϸ�������м����
    uint16_t RBC;
    uint8_t F_Zero_Error = 0; //����ں���Χ�ڣ���Ҫ�ڽ����ʶ�ı�־��1��������
    uint8_t Zero_Num = 0; //���������
    uint8_t F_CO2_Low = 0; //co2���ͱ�־��1����CO2Ũ����[2%-4.5%]
    uint16_t Diff_Zero;//����ֵ
    uint8_t sex = 0; //�Ա�0��Ů��1Ϊ��
    uint32_t runTimeOut = 0; //��ʱ
    uint8_t  sendNum = 0; //�������·��ʹ���
    uint16_t nnnn = 0;
		uint8_t  sampleTypeTmp=F_GasType;//���������ݴ�
		
#if 1
  //  while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)) || (!is_gasbag_exit(3)));
	while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)) || (!is_gasbag_exit(3)))
	{    
		BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
    	RetFrame3Flag = 1; // ��������ʾ��־
		framedisplayRrror();
		Delay1ms(3000);
		return ;
	}

    if(F_PC == 1)
    {
        for(m = 0; m < 4; m++) //����������״ָ̬��
        {
            FS_GasYes(m);
        }
        FS_GasYes(4);//����ȫ���嵽λ
        Delay1ms(5);
        if(F_PC == 0)
        {
            USART1_Timeout_UserCallback();
            F_PC_Ctrl = 0; //��λ��������λ����־����
        }
        while(F_PC)
        {
            runTimeOut++;
            if(ReceiveFlag6 == 1)
            {
                ReceiveFlag6 = 0;
                PC_Val_Analy();
            }
            if(F_Sex == 1) ///�յ���λ��������͵��Ա�ֵ
            {
                F_Sex = 0;
                Delay10ms(5);
                break;
            }
            Delay10ms(1);
            if(runTimeOut == 1000) //��ʱ�����ﵽ
            {
                runTimeOut = 0; //��ʼ��
                sendNum++;
                if(sendNum < 3) //�������
                {
                    RBCtoPC_Cmd_FF();
                    continue;
                }
                else
                {
                    USART1_Timeout_UserCallback();
                    F_PC_Ctrl = 0; //��λ��������λ����־����
                    break;
                }
            }
        }
        sex = Gender;
    }
    // ������������
    if(F_PC_Ctrl == 0)
    {
        // 1. �޸�Ѫ�쵰��HB,ͨ��+��LR�޸�
        framedisplayRun(); //HB�������
        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 6://-
                BtnPressedFlag = -1;
                if(HB != 0)
                    HB -= 1;
                ShowString(0x05, 0x04, Num2StrI(HB, "%03d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(HB != 999)
                    HB += 1;
                ShowString(0x05, 0x04, Num2StrI(HB, "%03d"));
                break;
            default:
                break;

            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                break;
            }
        }
        //�����Ա�
        framedisplaySex(sex);
        while(1)
        {
            if((BtnPressedFlag == 6) || (BtnPressedFlag == 7))
            {
                BtnPressedFlag = -1;
                sex = !sex;
                if(sex == 0)
                {
                    ShowString(0x09, 0x04, "Female");
                }
                else
                {
                    ShowString(0x09, 0x04, " Male ");
                }
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                break;
            }
        }
        /*�������͵���*/
        framedisplayGas_celiang_Default();
        while(1)
        {
            if((BtnPressedFlag == 6) || (BtnPressedFlag == 7))
            {
                BtnPressedFlag = -1;
                F_GasType = !F_GasType;
                if(F_GasType == 1)
                {
                    ShowString(0x0E, 0x03, "Auto Gas");
                }
                else
                {
                    ShowString(0x0E, 0x03, "Blow    ");
                }
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                RetFrame3Flag = 1; //��������ʾ��־
                break;
            }
        }

    }

#if 0
    //�����¶� 20180716
    LcmClear(0x00);// ����
    ShowString(0x01, 0x03, "Temperature:");
    ShowString(0x10, 0x05, "+-RUN");
    ShowString(0x0E, 0x03, Num2StrF(T_Var, "%02.1f"));
    LcmWriteCommandWith2Par(0x11, 0x03, 0x21); // д����
    LcmWriteCommand(0x9F);

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(T_Var != 0.0)
                T_Var -= 0.1;
            ShowString(0x0E, 0x03, Num2StrF(T_Var, "%02.1f"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(T_Var != 40.0)
                T_Var += 0.1;
            ShowString(0x0E, 0x03, Num2StrF(T_Var, "%02.1f"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            LcmWriteCommand(0x9C);
            RetFrame3Flag = 1; //��������ʾ��־
            break;
        }
    }
#endif
    if(F_DC == 1)
    {
        printf("HB=%d\r\n", HB);
        printf("�Ա�%d\r\n", Gender);
				printf("�������ͣ�%d\r\n",F_GasType);
    }
    // . ��Ļ��ʾSample����Testing
    Dis_Sample_Test();
    if(F_PC == 1)
    {
        FS_RUN_ON();
    }
    //�����¶Ⱥ���ѹ
    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);

    //  ����������CO2,��һ����CO2Ũ���Լ�����
    ShowString(0x01, 0x03, "STEP(1).............. ");
    ShowString(0x14, 0x03, "(RUNNING)");

    if(F_PC == 1)
    {
        Delay10ms(2);
        FS_RUN_step1_ON();
    }
    Ring_Num++;//��Ȧʹ�ô�������1  20180621
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //ʹ�ô���д��eeprom 20180621
    Purge_Cylinder();	//���ÿ���

    Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);
    // ����׼��:����CO2��̨����
    CO2_CONC = (float)(CO2_AD - CO2_ZTest) / CO2_Coeff; //adcģʽ
#if 0
    CO2_CONC_CAL = ATM_Coeff * 0.0001 * CO2_CONC * (T_Var + 273) / (T0 + 273); //������ѹ���¶�У������ʽ��V/V
#else
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    if(F_DC == 1)
    {
        printf("��ѹУ��ϵ��=%d\r\n", ATM_Coeff);
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
    }
    CO2_CONC_CAL = ATM_Coeff * 0.0001 * CO2_CONC * (BME_Temperature + 273) / (T0 + 273);
#endif
    if(F_DC == 1)
    {
        printf("CO2����ƽ=%d,CO2������ƽ=%d\r\n", CO2_ZTest, CO2_AD);
        printf("M/VŨ��CO2=%f\r\n", CO2_CONC);
        printf("V/VŨ��CO2=%f\r\n", CO2_CONC_CAL);
    }
    if(CO2_CONC_CAL < 0.50) /// ���CO2����Ũ��С��0.5%
    {
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "Sample Fault or Wrong Order");
        ShowString(0x00, 0x06, "Remove all bags,return to ");
        ShowString(0x00, 0x07, "Ready!");
        if(F_PC == 1)
        {
            FS_CO2_ERRO();
        }
        F_CO2_Num = 0;
        goto LAGE1;

    }
    ShowString(0x01, 0x07, "CO2:     %");
    ShowString(0x10, 0x07, "( ok )");
    ShowString(0x06, 0x07, Num2StrF(CO2_CONC_CAL, "%1.2f"));

    if((CO2_CONC_CAL < 2.0000) || (CO2_CONC_CAL >= 7.0000))
    {
        F_CO2_Num++;
        if(F_DC == 1)
        {
            printf("F_CO2_Num=%d\r\n", F_CO2_Num);
        }
    }
    else if(CO2_CONC_CAL < 3.0000)
    {
        F_CO2_Low = 1; //������̼���ͱ�־
        F_CO2_Num = 0;
    }
    else
    {
        F_CO2_Num = 0;
    }

    if(F_RUN2 == 0) // һ������µ���3��
    {
        for(i = 0; i < 3; i++)
        {
            DaoQi_Y(); //E3->E5
        }

    }
    Purge_Cylinder();


LAGE_TEST:

    if(F_RUN2 == 0)
    {
        Paiqi_RR_PROM();// ��ϴCO2����10 S
        for(i = 0; i < 3; i++)
        {
            DaoQi_B(); //E4->E6
        }
    }
    else     // ��ȷ�����µ���5��
    {
        for(i = 0; i < 5; i++)
        {
            DaoQi_Y();
        }
        Paiqi_RR_PROM();// ��ϴCO2����10 S
        for(i = 0; i < 5; i++)
        {
            DaoQi_B();
        }
    }

    Daiji(200);//����200S ,��CO2���������


    Paiqi_R_PROM();//��ϴCO����100S

    ShowString(0x14, 0x03, "(END)    ");

    if(F_PC == 1)
    {
        FS_RUN_step1_OK();
    }


    // �ڶ��� ��ȡ����������Ļ�����
    ShowString(0x01, 0x04, "STEP(2).............. ");
    ShowString(0x14, 0x04, "(RUNNING)");

    if(F_PC == 1)
    {
        Delay10ms(10);
        FS_RUN_step2_ON();
    }

    ADC(&CO_ZERO1); //��B��CO���*********************ZERO1
    if(F_DC == 1)
    {
        printf(" �������ֵ=%d\r\n", CO_ZERO1);
    }
    // ��ȡ����200ml(����B��)
    for(i = 0; i < 2; i++)
    {
        Chou_BGas();// �ӻ����������黷����2��E6->E1
    }

    ShowString(0x00, 0x06, "                      "); // ���CO2��ʾ��ʾ
    ShowString(0x00, 0x07, "                      ");
    ShowString(0x14, 0x04, "(END)     ");
    if(F_PC == 1)
    {
        FS_RUN_step2_OK();
        Delay10ms(10);
    }
    // ������������������CǪ��
    ShowString(0x01, 0x05, "STEP(3).............. ");
    ShowString(0x14, 0x05, "(RUNNING)");
    if(F_PC == 1)
    {
        FS_RUN_step3_ON();
    }


    Daiji(30);// ����ʱ30S
    ADC(&CO_AD1); //  ����������B����̨��************
    if(F_DC == 1)
    {
        printf(" ������ƽֵ=%d\r\n", CO_AD1);
    }
    if(F_RUN2 == 1) //��ȷ������
    {
        for(i = 0; i < 2; i++)
        {
            Chou_BGas();// �ӻ����������黷����2��
        }
        Daiji(30);// ����ʱ30S
        ADC(&CO_Re1);
        CO_AD1 = (CO_AD1 + CO_Re1) / 2;
    }

    ShowString(0x14, 0x05, "(END)    ");

    if(F_PC == 1)
    {
        FS_RUN_step3_OK();
        Delay10ms(10);
    }
    // ���Ĳ���������Ʒ����CǪ��
    ShowString(0x01, 0x06, "STEP(4).............. ");
    ShowString(0x14, 0x06, "(RUNNING)");
    if(F_PC == 1)
    {
        FS_RUN_step4_ON();
    }

    Paiqi_R_PROM( );//��ϴCO����100S

    ADC(&CO_ZERO2); //   Y��CO���
    if(F_DC == 1)
    {
        printf(" �������ֵ=%d\r\n", CO_ZERO2);
    }
    Diff_Zero = abs(CO_ZERO2 - CO_ZERO1);
    //�������ж�
    if(Diff_Zero > 10)
    {
        if(Diff_Zero > 20) //����20
        {
            if((Zero_Num == 1) || (F_RUN2 == 1)) //��ȷ����ֱ�ӱ������������β������������⣬����������
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
                Daiji(180);
                goto LAGE_TEST;//����������������
            }
        }
        else
        {
            F_Zero_Error = 1; //��ʾ��־��1
            F_Zero_Num = 0; //��Ϊ�쳣�����������
        }
    }
    else
    {
        F_Zero_Num = 0; //��Ϊ�쳣�����������
    }

    // ��ȡ����
    for(i = 0; i < 2; i++)
    {
        Chou_YGas();// �ӷ������������黷����2��  E5->E1
    }

    Daiji(30);// ����ʱ30S
    ADC(&CO_AD2); // ����Y����̨��
    if(F_DC == 1)
    {
        printf(" ������ƽֵ=%d\r\n", CO_AD2);
    }
    CO_AD2_Z = CO_AD2 - CO_ZERO2;
    if(CO_AD2_Z <= 0.00001) //�����ж���������㻹�͵����
    {
        CO_AD2_Z = 0.0;
    }

    if(F_RUN2 == 1)
    {
        for(i = 0; i < 2; i++)
        {
            Chou_YGas();// �ӻ����������黷����2��
        }
        Daiji(30);// ����ʱ30S

        ADC(&CO_Re2);

        CO_Re2_Z = CO_Re2 - CO_ZERO2; // ��������CO�� ƽ��

        CO_Re2_Z = CO_Re2_Z * (1 - CO2_CONC * 0.01);
        if(CO_Re2_Z <= 0.00001)
        {
            CO_Re2_Z = 0.0;
        }

        CO_AD2_Z = (CO_AD2_Z + CO_Re2_Z) / 2;

    }

    ShowString(0x14, 0x06, "(END)    ");
    if(F_PC == 1)
    {
        FS_RUN_step4_OK();
    }

    //����CO��̨�ײ�
    CO_AD1_Z = CO_AD1 - CO_ZERO1;

    CO_Cha = CO_AD2 - CO_AD1; //V1.3.1.3�޸�20181102

    //�����������Ũ�Ȳ�
    PcoR = CO_Cha * (float)R_XB / 10000; // ppm  ����ֵ��M/Vģʽ
#if 0
    Pco = PcoR * ATM_Coeff * 0.0001 * (273 + T_Var) / (273 + T0); //������ѹ���¶�У��
#else
    Pco = PcoR * ATM_Coeff * 0.0001 * (273 + BME_Temperature) / (273 + T0); //������ѹ���¶�У��
#endif
    if(F_DC == 1)
    {
        printf("CO_Cha=%f\r\n", CO_Cha);
        printf("��ѹ���¶�У����Pco=%f\r\n", Pco);
    }
    Pco = Pco * (1 - CO2_CONC_CAL * 0.01) - CO2_CONC_CAL * 0.01 * R_XB / 10000 * CO_AD1_Z; //����CO2����Ũ��У��
    if(F_DC == 1)
    {
        printf("CO2Ũ��У����Pco=%f\r\n", Pco);
    }
    //ϡ��У�� ����Ů����
    if(sex == 1)
    {
        if(CO2_CONC_CAL < 5.3)
        {
            Pco = Pco * 5.3 / CO2_CONC_CAL;
        }
        else if(CO2_CONC_CAL > 6)
        {
            Pco = Pco * 6.0 / CO2_CONC_CAL;
        }
        else
        {
            Pco = Pco;
        }
    }
    else
    {
        if(CO2_CONC_CAL < 5.1)
        {
            Pco = Pco * 5.1 / CO2_CONC_CAL;
        }
        else if(CO2_CONC_CAL > 5.8)
        {
            Pco = Pco * 5.8 / CO2_CONC_CAL;
        }
        else
        {
            Pco = Pco;
        }
    }
    if(F_DC == 1)
    {
        printf("�Ա�ϡ��У��PCO=%f\r\n", Pco);
    }
		if(F_GasType==1)
		{
		    Pco=Pco*gasTypeFactor*0.01;
		}
		if(F_DC==1)
		{
		    printf("��������У��PCO=%f\r\n", Pco);
		}
    LcmClear(0x00);// ����
    if(Pco <= 0.0001)
    {
        ShowString(0x06, 0x04, "Test_Fault!");
        ShowString(0x00, 0x06, "Remove all bags,return to ");
        ShowString(0x00, 0x07, "Ready!");

        ShowString(0x10, 0x02, "BG:");
        ShowString(0x14, 0x02, Num2StrI(CO_AD1, "%04d"));
        ShowString(0x19, 0x02, Num2StrI(CO_ZERO1, "%04d"));
        ShowString(0x10, 0x01, "CO:");
        ShowString(0x14, 0x01, Num2StrI(CO_AD2, "%04d"));
        ShowString(0x19, 0x01, Num2StrI(CO_ZERO2, "%04d"));

        if(F_PC == 1)
        {
            FS_TEST_ERRO();
        }
        goto LAGE1;

    }
    if(F_tiaoji == 1)
    {
        LcmClear(0x00);// ����
        ShowString(0x01, 0x02, "BG:");
        ShowString(0x0F, 0x02, Num2StrI(CO_AD1, "%06d"));
        ShowString(0x17, 0x02, Num2StrI(CO_ZERO1, "%06d"));

        ShowString(0x01, 0x01, "CO:");
        ShowString(0x0F, 0x01, Num2StrI(CO_AD2, "%06d"));
        ShowString(0x17, 0x01, Num2StrI(CO_ZERO2, "%06d"));
    }
#endif
    //��PCO����λ������������ 20181102 lcy

    if((((uint32_t)(Pco * 100000)) % 10) < 5)
    {
        Pco = (uint32_t)(Pco * 10000) / 10000.0;
    }
    else
    {
        Pco = ((uint32_t)(Pco * 10000) + 1) / 10000.0;
    }
    // ���ݹ�ʽRBC=(HB*1.38)/PCO ����õ��������ˣ�RBC=120�죩
    // ��PCO=1.265ppm,����110g/L:

    RBC_Temp = 1.38 * HB / Pco;
    if((((uint32_t)(RBC_Temp * 10)) % 10) < 5)
    {
        RBC = (uint16_t)RBC_Temp;
    }
    else
        RBC = (uint16_t)RBC_Temp + 1;
    if(F_DC == 1)
    {
        printf("RBC=%d\r\n", RBC);
    }
    ShowString(0x01, 0x03, "PCO=       (ppm)");
    ShowString(0x05, 0x03, Num2StrF(Pco, "%02.4f"));
    if(RBC < 251)
    {
        ShowString(0x01, 0x04, "RBC_SPAN=   (day)");
        ShowString(0x0A, 0x04, Num2StrI(RBC, "%03d"));

    }
    else
    {
        ShowString(0x01, 0x04, "RBC_SPAN>250(day)");
    }
    ShowString(0x01, 0x05, "CO2=     %(ok)");
    ShowString(0x05, 0x05, Num2StrF(CO2_CONC_CAL, "%02.2f"));

    ShowString(0x00, 0x06, "Remove all bags,return to");
    ShowString(0x00, 0x07, "Ready!");
    if(F_Zero_Error == 1)
    {
        ShowString(0x16, 0x03, "*(    )"); //��ʾƫ��ϴ�����
        ShowString(0x18, 0x03, Num2StrI(Diff_Zero, "%04d")); //��ʾ����ֵ
    }
    if(F_Zero_Num != 0)
    {
        ShowString(0x15, 0x03, "**(    )"); //��ʾƫ��ϴ�����
        ShowString(0x18, 0x03, Num2StrI(Diff_Zero, "%04d")); //��ʾ����ֵ
    }
    if(F_CO2_Low == 1)
    {
        ShowString(0x1C, 0x05, "*"); //��ʾCO2Ũ�ȹ�����ʾ
    }
    if(F_CO2_Num != 0)
    {
        ShowString(0x1b, 0x05, "**"); //��ʾCO2Ũ���쳣��ʾ
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

        if((F_CO2_Num != 0) && (CO2_CONC_CAL < 7.0000))
        {
            FS_CO2_Fault();
        }
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }
				FS_SampleType(F_GasType);
        if(F_Timeout == 1)
        {
            F_Timeout = 0;
            break;
        }

        FS_SEX(&sex);
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

        FS_CO2(&CO2_CONC_CAL);
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
            SD_SAVE();   ///SD����������
        }
    }
    BEEP_DOING();// ��������

    while(is_gasbag_exit(0) || (is_gasbag_exit(1) || is_gasbag_exit(2) || is_gasbag_exit(3)));

    if(F_PC == 1)
    {
        for(m = 0; m < 4; m++)
        {
            FS_GasNo(m);
            Delay10ms(1);
        }
    }
    if((F_CO2_Num == 3) || (F_Zero_Num == 2)) ///���CO2��������3���쳣����zero�쳣2����ֱ�Ӿܾ���һ������
    {
        if(F_CO2_Num == 3)
        {
            if(F_Zero_Num == 2)
            {
                LcmClear(0x00);// ����
                ShowString(0x01, 0x03, "CO2_Fault and Zero_Fault");
                ShowString(0x00, 0x06, "Please contact the manufacturer");
            }
            else
            {
                LcmClear(0x00);// ����
                ShowString(0x05, 0x03, "CO2_Fault!");
                ShowString(0x00, 0x06, "Please contact the manufacturer");
            }

        }
        else
        {
            LcmClear(0x00);// ����
            ShowString(0x05, 0x03, "Zero_Fault!");
            ShowString(0x00, 0x06, "Please contact the manufacturer");
        }
        while(1);
    }
    RetFrame3Flag = 1; // ��������ʾ��־
		F_GasType=sampleTypeTmp;
    F_PC_Ctrl = 0; //PC���Ʊ�־����
    ReceiveFlag6 = 0; //��ֹ��λ����������������󰴲�����
    HB = 150;
    BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
}

//��������

/**
  * @breif  QC_PROM(void)
  * @datail �ʿ�����
  *
  * @param  none
  * @return none
  * @date 2019-01-02
  */
void QC_PROM(void)
{
    uint8_t i;
    uint8_t m, n;
    uint16_t CO_ZERO1, CO_ZERO2, CO_AD1, CO_AD2; //CO_ZERO_V ��̬���
    float PcoR[3], Pco[3], CO_Cha;
    uint16_t R_CO_Precision, R_CO_Accuracy; //���ս��ֵ
    float CO2_Precision, CO2_Accuracy; //CO2�����ľ��ܶȺ�׼ȷ��
    float CO_AD1_Z, CO_AD2_Z;
    uint16_t A_CO2_CONC = 500; //����co2Ũ�����ֳ���100��V/Vֵ
    float A_CO2_MV[3], A_CO2_VV[3], SUM_ACO2_VV; //CO2Ũ�ȣ���Ҫ��M/V��V/V����
    uint8_t F_Zero_Error = 0; //����ں���Χ�ڣ���Ҫ�ڽ����ʶ�ı�־��1��������
    uint8_t Zero_Num = 0; //���������
    uint16_t Diff_Zero = 0; //����ֵ
    uint8_t mum = 0;
    uint8_t sendNum = 0;
    uint8_t F_CO2FAULT = 0; //CO2δ����־
    uint16_t runTimeOut = 0;
    uint16_t QC_ZTest, QCCO2_AD;
    float Pco_Temp[3];
  //  while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)) || (!is_gasbag_exit(3)));
	while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)) || (!is_gasbag_exit(3)))
	{    
		
    	RetFrame3Flag = 1; // ��������ʾ��־
		framedisplayRrror();
		Delay1ms(3000);
		BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
		return ;
	}

    if(F_PC == 1)
    {
        Delay100ms(1);
        for(n = 0; n < 4; n++)
        {
            FS_GasYes(n);
        }
        FS_GasYes(5);//����ȫ���嵽λ
        Delay10ms(1);
        if(F_PC == 0)
        {
            USART1_Timeout_UserCallback();
            F_PC_Ctrl = 0; //��λ��������λ����־����
        }
        if(F_PC_Ctrl == 1)
        {
            while(F_PC)
            {
                runTimeOut++;
                if(ReceiveFlag6 == 1)
                {
                    ReceiveFlag6 = 0;
                    PC_Val_Analy();
                }
                if(F_CO2 == 1) ///�յ���λ��������� 20190318
                {
                    F_CO2 = 0;
                    Delay10ms(5);
                    break;
                }
                Delay10ms(1);
                if(runTimeOut == 1000) //��ʱ�����ﵽ
                {
                    runTimeOut = 0; //��ʼ��
                    sendNum++;
                    if(sendNum < 3) //�������
                    {
                        RBCtoPC_Cmd_FF();
                        continue;
                    }
                    else
                    {
                        USART1_Timeout_UserCallback();
                        F_PC_Ctrl = 0; //��λ��������λ����־����
                        break;
                    }
                }
            }
            A_CO2_CONC = QC_CO2_CONC;
        }
    }
    Ring_Num++;//20180621
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180621
#if 1
    if(F_PC_Ctrl == 0)
    {
        framedisplayQC(); //У׼����CO�궨��ֵ����

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(QC_CONC != 0)
                    QC_CONC -= 1;
                ShowString(0x0C, 0x03, Num2StrI(QC_CONC, "%06d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(QC_CONC != 1000)
                    QC_CONC += 1;
                ShowString(0x0C, 0x03, Num2StrI(QC_CONC, "%06d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4) // ����SET����
            {
                BtnPressedFlag = -1;

                LcmWriteCommand(0x9C);// ���ر�
                break;
            }
            else if(BtnPressedFlag == 1) //����LT
            {
                BtnPressedFlag = -1;

                LcmWriteCommand(0x9C);// ���ر�
                RetFrame3Flag = 1; // ��������ʾ��־
                return;
            }
        }

        /*
         		��������CO2Ũ��
         		20180625
         */
        LcmClear(0x00);
		ShowString(0x00,0x01,"press UP or DN to set A_CO2_CONC:");//lwj
        ShowString(0x02, 0x03, "A_CO2_CONC:");
       // ShowString(0x0c, 0x04, "+-RUN");
       ShowString(0x05,0x07,"press RUN/SET to continue");   //lwj
        ShowString(0x0E, 0x03, Num2StrI(A_CO2_CONC, "%04d"));
        LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
        LcmWriteCommandWith2Par(0x11, 0x03, 0x21); // д����

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(A_CO2_CONC != 0)
                    A_CO2_CONC -= 1;
                ShowString(0x0E, 0x03, Num2StrI(A_CO2_CONC, "%04d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(A_CO2_CONC != 1000)
                    A_CO2_CONC += 1;
                ShowString(0x0E, 0x03, Num2StrI(A_CO2_CONC, "%04d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                break;
            }
        }
#if 0
        /*
        	 �¶�����
        	 20180713
        	 */
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "Temperature:");
        ShowString(0x0C, 0x04, "+-RUN");
        ShowString(0x0E, 0x03, Num2StrF(T_Var, "%02.1f"));
        LcmWriteCommandWith2Par(0x10, 0x03, 0x21); // д����
        LcmWriteCommand(0x9F);

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(T_Var != 0.0)
                    T_Var -= 0.1;
                ShowString(0x0E, 0x03, Num2StrF(T_Var, "%02.1f"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(T_Var != 40.0)
                    T_Var += 0.1;
                ShowString(0x0E, 0x03, Num2StrF(T_Var, "%02.1f"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                RetFrame3Flag = 1; //��������ʾ��־
                break;
            }
        }
        //���뺣�θ߶�
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "Altitude:");
        ShowString(0x0C, 0x05, "+-RUN");
        ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
        ShowString(0x11, 0x03, "m");
        LcmWriteCommandWith2Par(0x0F, 0x03, 0x21); // д����
        LcmWriteCommand(0x9F);

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                if(H_Var > 99)
                    H_Var -= 100;
                ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(H_Var != 0)
                    H_Var -= 1;
                ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(H_Var != 5000) //����ܳ���5000��
                    H_Var += 1;
                ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                if(H_Var < 4900) //����ܳ���5000��
                    H_Var += 100;
                ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                break;
            }
        }

        if(F_tiaoji == 1) //�ڹ���ʦģʽ�£���Ҫ��������ѹУ��ϵ��
        {
            //����ѹ��������
            P0_Temp = 1 - H0 / 44330.0;
            P1_Temp = 1 - H_Var / 44330.0;
            ATM_Coeff = (uint16_t)(10000 * pow(P0_Temp, 5.255) / pow(P1_Temp, 5.255));
        }
#endif
    }

    if(F_DC == 1)
    {
        printf("��׼COŨ�Ȳ%d\r\n", QC_CONC);
        printf("ACO2Ũ�ȣ�%d\r\n", A_CO2_CONC);
    }
    LcmClear(0x00);// ����
    if(F_PC == 1)
    {
        Delay10ms(5);
        FS_QC_ON();
    }
    //�ڵ���״̬��
    ShowString(0x01, 0x01, "Quality_Testing:");
    ShowString(0x01, 0x03, "Gas Collection.......");
    ShowString(0x14, 0x03, "(RUNNING)");
    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    if(F_DC == 1)
    {
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
        printf("��ѹУ��ϵ��=%d\r\n", ATM_Coeff);
    }
    DaoQi_Z(200);//�ռ�����

    for(m = 0; m < 3; m++)
    {
        LcmClear(0x00);// ����
        ShowString(0x01, 0x01, "Quality_Testing:  NO.");
        ShowString(0x16, 0x01, Num2StrI(m + 1, "%01d"));
        ShowString(0x01, 0x03, "STEP(1).............. ");
        ShowString(0x14, 0x03, "(RUNNING)");
        if(F_PC == 1)
        {
            switch(m)
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
        Purge_Cylinder();
        Celiang_ZhunBeiNew(&QC_ZTest, &QCCO2_AD); //��������ϴ��ƽ

        A_CO2_MV[m] = (float)(QCCO2_AD - QC_ZTest) / CO2_Coeff;
        //��CO2��V/VŨ��
#if 0
        A_CO2_VV[m] = ATM_Coeff * 0.0001 * A_CO2_MV[m] * (T_Var + 273) / (T0 + 273);
//        printf("��%d��A_CO2_VV=%f\r\n",m,A_CO2_VV[m]);
#else
        A_CO2_VV[m] = ATM_Coeff * 0.0001 * A_CO2_MV[m] * (BME_Temperature + 273) / (T0 + 273);
#endif
        if(F_DC == 1)
        {
            printf("CO2���ֵ=%d\r\n,CO2��ƽֵ=%d\r\n", QC_ZTest, QCCO2_AD);
            printf("��%d��A_CO2_MV=%f\r\n", m, A_CO2_MV[m]);
            printf("��%d��A_CO2_VV=%f\r\n", m, A_CO2_VV[m]);
        }
        SUM_ACO2_VV += A_CO2_VV[m];
        ShowString(0x01, 0x07, "CO2:      %");
        ShowString(0x06, 0x07, Num2StrF(A_CO2_VV[m], "%1.2f"));

        //����3�Σ�������200s
        for(i = 0; i < 3; i++)
        {
            DaoQi_Y(); //E3->E5
        }
        Purge_Cylinder();
        Paiqi_RR_PROM();// ��ϴCO2����10S
        for(i = 0; i < 3; i++)
        {
            DaoQi_B();//E4->E6

        }

        Daiji(200);//����200S

        ShowString(0x14, 0x03, "(END)    ");
        // �ڶ��� ��ȡ����������Ļ�����
        ShowString(0x01, 0x04, "STEP(2).............. ");
        ShowString(0x14, 0x04, "(RUNNING)");
        Paiqi_R_PROM( );//��ϴCO����100S
        ADC(&CO_ZERO1); //��B��CO���*********************ZERO1
        if(F_DC == 1)
        {
            printf(" ��%d�ε������=%d\r\n", m + 1, CO_ZERO1);
        }
        for(i = 0; i < 2; i++)
        {
            Chou_BGas();
        }
        Daiji(30);
        ADC(&CO_AD1);//����������B����̨��
        if(F_DC == 1)
        {
            printf("��%d�ε�����ƽֵ=%d\r\n", m + 1, CO_AD1);
        }
        ShowString(0x00, 0x06, "                      "); // ���CO2��ʾ��ʾ
        ShowString(0x00, 0x07, "                      ");
        ShowString(0x14, 0x04, "(END)     ");

        // ������������������CǪ��
        ShowString(0x01, 0x05, "STEP(3).............. ");
        ShowString(0x14, 0x05, "(RUNNING)");

//			TIM6Delay_f(10000);//  ��ʱ10S
        Paiqi_R_PROM();//��ϴ����100s
        ADC(&CO_ZERO2); //   Y��CO���
        if(F_DC == 1)
        {
            printf(" ��%d���������2=%d\r\n", m + 1, CO_ZERO2);
        }
        ShowString(0x14, 0x05, "(END)    ");

        // ���Ĳ���������Ʒ����CǪ��
        ShowString(0x01, 0x06, "STEP(4).............. ");
        ShowString(0x14, 0x06, "(RUNNING)");

        Diff_Zero = abs(CO_ZERO2 - CO_ZERO1);
        //�������ж�
        if(Diff_Zero > 10)
        {
            if(Diff_Zero > 20) //����20
            {
                if(Zero_Num == 2) //���β������������⣬ֱ�ӱ���
                {
                    framdisplayZeroFault();//������ʾ
                    if(F_PC == 1)
                    {
                        FS_QC_RUN_ERR2();
                    }
                    goto QC_LAGE;
                }
                else
                {
                    Zero_Num++;
                    Pco[m] = 0.0; //�������ֵ0
                    Daiji(180);//��ֹ3min��������ȶ�����
                    goto QC_LAGE2;
                }
            }
            else
            {
                F_Zero_Error = 1; //��ʾ��־��1
            }
        }

        // ��ȡ����
        for(i = 0; i < 2; i++)
        {
            Chou_YGas();// ����������������2��
        }

        Daiji(30);// ����ʱ30S
        ADC(&CO_AD2); // ����Y����̨��
        if(F_DC == 1)
        {
            printf(" ��%d��������ƽ=%d\r\n", m + 1, CO_AD2);
        }
        CO_AD2_Z = CO_AD2 - CO_ZERO2;
        if(CO_AD2_Z <= 0.00001)
        {
            CO_AD2_Z = 0.0;
        }
        ShowString(0x14, 0x06, "(END)    ");
        CO_AD1_Z = CO_AD1 - CO_ZERO1;

        //����CO��̨�ײCO����ƽ��ֵ
        CO_Cha = CO_AD2 - CO_AD1;

//        printf("��%d��δУ��CO_Cha=%f\r\n",m+1,CO_Cha);

        PcoR[m] = (float)R_XB / 10000 * CO_Cha; // ppm
        if(F_DC == 1)
        {
            printf("У��ǰ pcoR[%d]=%f\r\n", m + 1, PcoR[m]);
        }
        //��ѹ���¶Ⱥ�����У��
#if 0
        Pco[m] = PcoR[m] * ATM_Coeff * 0.0001 * (T_Var + 273) / (T0 + 273) * (1 - A_CO2_VV[m] / 100); // ϡ��У��Pco
#else
        Pco[m] = PcoR[m] * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / (T0 + 273) * (1 - A_CO2_VV[m] / 100); // ϡ��У��Pco
#endif
//        printf("У���� Pco[%d]=%f\r\n",m+1,Pco[m]);
        if(Pco[m] <= 0.0001)
        {
            LcmClear(0x00);// ����
            ShowString(0x0A, 0x03, "Test_Fault!");
            ShowString(0x00, 0x06, "Remove all bags,return to ");
            ShowString(0x00, 0x07, "Ready!");
            goto QC_LAGE;
        }
QC_LAGE2:
        if(m < 2)
        {
            Paiqi_RRR_PROM();
            BEEP_DOING( );// ��������
            LcmClear(0x00);// ����
            ShowString(0x00, 0x06, "Remove third & fourth,next");
            while(is_gasbag_exit(2) || is_gasbag_exit(3));
            LcmClear(0x00);// ����
            ShowString(0x00, 0x06, "Plug in third & fourth,next");
            while((!is_gasbag_exit(2)) || (!is_gasbag_exit(3)));
        }
        LcmClear(0x00);// ����
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
    } //ѭ������
    CO2_Accuracy = 100 * StandardDeviation(A_CO2_VV, 3, 0, A_CO2_CONC); //׼ȷ�ȼ���
    CO2_Precision = 100 * StandardDeviation(A_CO2_VV, 3, 1, A_CO2_CONC); //���ܶȼ���
    if(F_DC == 1)
    {
        printf("CO2׼ȷ��Ϊ��%f\r\n", CO2_Accuracy);
    }
    if((CO2_Accuracy > 4.0) || (CO2_Precision > 4.0))
    {
        LcmClear(0x00);// ����
        ShowString(0x0A, 0x01, "CO2 FAULT");
        ShowString(0x01, 0x02, "R_CO2_Accuracy=   %~   %");
        ShowString(0x01, 0x03, "R_CO2_Precision=   %~   %");
        CO2_Accuracy += 100;
        ShowString(0x16, 0x02, Num2StrI(CO2_Accuracy, "%03d"));
        CO2_Accuracy = 200 - CO2_Accuracy;
        ShowString(0x11, 0x02, Num2StrI(CO2_Accuracy, "%02d"));
        CO2_Precision += 100;
        ShowString(0x16, 0x03, Num2StrI(CO2_Precision, "%03d"));
        CO2_Precision = 200 - CO2_Precision;
        ShowString(0x11, 0x03, Num2StrI(CO2_Precision, "%02d"));
        if(F_PC == 1)
        {
            FS_QC_RUN_ERR1();
        }
        F_CO2FAULT = 1;
    }
    // ���� Rֵ
//			R=(Pco/(QC_CONC*0.01))*100;  //��%��ʾ
#endif
//��׼�������֤20180720
    for(i = 0; i < 3; i++)
    {
        if(Pco[i] > 0.1)
        {
            Pco_Temp[mum] = Pco[i];
            mum++;
        }
    }
    R_CO_Precision = (uint16_t)(100 * StandardDeviation(Pco_Temp, mum, 1, QC_CONC)); //���ܶȼ���,��ҪQC_CONC������������ֵ��λ��%
    R_CO_Accuracy = (uint16_t)(100 * StandardDeviation(Pco_Temp, mum, 0, QC_CONC)); //׼ȷ�ȼ���
    if(F_DC == 1)
    {
        printf("���ܶ�*100=%d\r\n", R_CO_Precision);
        printf("׼ȷ��*100=%d\r\n", R_CO_Accuracy);
    }
    if(F_CO2FAULT == 1)
    {
        ShowString(0x01, 0x04, "R_CO_Precision=   %~   %");
        ShowString(0x01, 0x05, "R_CO_Accuracy =   %~   %");
        R_CO_Precision += 100;
        ShowString(0x15, 0x04, Num2StrI(R_CO_Precision, "%03d"));
        R_CO_Precision = 100 - (R_CO_Precision - 100);
        ShowString(0x11, 0x04, Num2StrI(R_CO_Precision, "%02d"));
        R_CO_Accuracy += 100;
        ShowString(0x15, 0x05, Num2StrI(R_CO_Accuracy, "%03d"));
        R_CO_Accuracy = 100 - (R_CO_Accuracy - 100);
        ShowString(0x11, 0x05, Num2StrI(R_CO_Accuracy, "%02d"));
    }
    else if((R_CO_Precision <= 10) && (R_CO_Accuracy <= 10))
    {
        LcmClear(0x00);// ����
        if(F_PC == 1)
        {
            FS_RA(&R_CO_Precision);
            Delay10ms(2);
            FS_RP(&R_CO_Accuracy);
            Delay10ms(2);
        }
        ShowString(0x0A, 0x02, "QC PASS");
        ShowString(0x01, 0x04, "R_CO_Precision=   %~   %");
        ShowString(0x01, 0x05, "R_CO_Accuracy =   %~   %");
        R_CO_Precision += 100;
        ShowString(0x15, 0x04, Num2StrI(R_CO_Precision, "%03d"));
        R_CO_Precision = 100 - (R_CO_Precision - 100);
        ShowString(0x11, 0x04, Num2StrI(R_CO_Precision, "%02d"));
        R_CO_Accuracy += 100;
        ShowString(0x15, 0x05, Num2StrI(R_CO_Accuracy, "%03d"));
        R_CO_Accuracy = 100 - (R_CO_Accuracy - 100);
        ShowString(0x11, 0x05, Num2StrI(R_CO_Accuracy, "%02d"));
        if(F_Zero_Error == 1)
        {
            ShowString(0x14, 0x02, "*");
            if(F_PC == 1)
            {
                FS_QC_RS_CRI_OK();//��ʾ�ʿ�*
            }
        }
        else
        {
            if(F_PC == 1)
            {
                FS_QC_RS_OK();//��ʾ�ʿ�ͨ��
            }
        }
    }
    else
    {
        LcmClear(0x00);// ����
        if(F_PC == 1)
        {
            FS_RA(&R_CO_Precision);
            Delay10ms(2);
            FS_RP(&R_CO_Accuracy);
            Delay10ms(2);
        }
        ShowString(0x0A, 0x02, "QC Failed");
        ShowString(0x01, 0x04, "R_CO_Precision=   %~   %");
        ShowString(0x01, 0x05, "R_CO_Accuracy =   %~   %");
        R_CO_Precision += 100;
        ShowString(0x15, 0x04, Num2StrI(R_CO_Precision, "%03d"));
        R_CO_Precision = 100 - (R_CO_Precision - 100);
        ShowString(0x11, 0x04, Num2StrI(R_CO_Precision, "%02d"));
        R_CO_Accuracy += 100;
        ShowString(0x15, 0x05, Num2StrI(R_CO_Accuracy, "%03d"));
        R_CO_Accuracy = 100 - (R_CO_Accuracy - 100);
        ShowString(0x11, 0x05, Num2StrI(R_CO_Accuracy, "%02d"));
        if(F_PC == 1)
        {
            FS_QC_RS_ERR();//��ʾ�ʿ�δͨ��
        }
    }
    if(F_PC == 1)
    {
        Delay10ms(10);
        FS_QC_OK();
    }
    Paiqi_RRR_PROM();
QC_LAGE:
    BEEP_DOING( );// ��������

    while(is_gasbag_exit(0) || (is_gasbag_exit(1) || is_gasbag_exit(2) || is_gasbag_exit(3)));
    if(F_PC == 1)
    {
//        Send_Gas_Com(); //����״̬�£�����������״̬��?
        for(n = 0; n < 4; n++)
        {
            FS_GasNo(n);
            Delay10ms(6);
        }
        Delay10ms(5);
        FS_QC_BACK();
        Delay10ms(5);
    }
    F_PC_Ctrl = 0;
    RetFrame3Flag = 1; // ��������ʾ��־
    ReceiveFlag6 = 0; //��ֹ��λ������������������ʿؼ�
    BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
}
//�ʿؽ���

/**
  * @breif  Curve_PROM(void)
  * @datail �궨����
  *
  * @param  none
  * @return none
  * @date   2019-01-02
  */
void Curve_PROM(void)
{
    uint16_t CO_ZERO1, CO_ZERO2, CO_AD2, CO_AD1;
    float CO_AD1_Z, CO_AD2_Z;
    float CO_cha[3];
    float Sum_CO_cha = 0;
    uint8_t i;
    uint16_t A_CO2_CONC = 500; //CO2Ũ�����ֳ���100��V/Vֵ
//    uint16_t H_Var=0;
//    float T_Var=25.0;
    float A_CO2_MV[3], A_CO2_VV[3]; // CO2Ũ�ȣ���Ҫ��M/V,V/V����
    float SUM_ACO2_VV = 0.0;
    uint16_t Curve_CONC_MV;//�궨CO��M/VŨ��
    uint8_t n = 0;
    float CO2_Accuracy, DCO_Precision;
    uint16_t CO2_CZTest, CO2_CAD;
//	  double P0_Temp,P1_Temp;
    if(F_tiaoji == 1)
    {
				while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)) || (!is_gasbag_exit(3)))
				{    

						RetFrame3Flag = 1; // ��������ʾ��־
						framedisplayRrror();
						Delay1ms(3000);
						BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
						return ;
				}

				Ring_Num++;//20180625
        ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180625

        framdisplayCurve();

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(Curve_CONC != 0)
                    Curve_CONC -= 1;
                ShowString(0x0A, 0x03, Num2StrI(Curve_CONC, "%06d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(Curve_CONC != 1000)
                    Curve_CONC += 1;
                ShowString(0x0A, 0x03, Num2StrI(Curve_CONC, "%06d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;

                LcmWriteCommand(0x9C);
                break;
            }
        }
        /*
         		��������CO2Ũ��
         		20180625
         */
        LcmClear(0x00);
		ShowString(0x00,0x01,"press UP or DN to set A_CO2_CONC:");//lwj
        ShowString(0x02, 0x03, "A_CO2_CONC:");
        //ShowString(0x0c, 0x04, "+-RUN");
        ShowString(0x05,0x07,"press RUN/SET to continue");   //lwj
        ShowString(0x0E, 0x03, Num2StrI(A_CO2_CONC, "%04d"));
        LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
        LcmWriteCommandWith2Par(0x11, 0x03, 0x21); // д����

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(A_CO2_CONC != 0)
                    A_CO2_CONC -= 1;
                ShowString(0x0E, 0x03, Num2StrI(A_CO2_CONC, "%04d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(A_CO2_CONC != 1000)
                    A_CO2_CONC += 1;
                ShowString(0x0E, 0x03, Num2StrI(A_CO2_CONC, "%04d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                RetFrame3Flag = 1; //��������ʾ��־
                break;
            }
        }
#if 0
        /*
        		��������
        		20180711
        */
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "Altitude:");
        ShowString(0x0C, 0x04, "+-RUN");
        ShowString(0x0D, 0x03, Num2StrI(H_Var, "%04d"));
        ShowString(0x11, 0x03, "m");
        LcmWriteCommandWith2Par(0x10, 0x03, 0x21); // д����
        LcmWriteCommand(0x9F);
        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                if(H_Var > 99)
                    H_Var -= 100;
                ShowString(0x0D, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(H_Var != 0)
                    H_Var -= 1;
                ShowString(0x0D, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(H_Var != 5000) //����ܳ���5000��
                    H_Var += 1;
                ShowString(0x0D, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                if(H_Var < 4900) //����ܳ���5000��
                    H_Var += 100;
                ShowString(0x0D, 0x03, Num2StrI(H_Var, "%04d"));
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                break;
            }
        }
        /*
        	 �¶�����
        	 20180710
        	 */
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "Temperature:");
        ShowString(0x0C, 0x05, "+-RUN");
        ShowString(0x0F, 0x03, Num2StrF(T_Var, "%02.1f"));
        LcmWriteCommandWith2Par(0x12, 0x03, 0x21); // д����
        LcmWriteCommand(0x9F);

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(T_Var != 0.0)
                    T_Var -= 0.1;
                ShowString(0x0F, 0x03, Num2StrF(T_Var, "%02.1f"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(T_Var != 40.0)
                    T_Var += 0.1;
                ShowString(0x0F, 0x03, Num2StrF(T_Var, "%02.1f"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                LcmWriteCommand(0x9C);
                RetFrame3Flag = 1; //��������ʾ��־
                break;
            }
        }
#endif
        if(F_DC == 1)
        {
            printf("ACO2Ũ��=%d\r\n", A_CO2_CONC);
            printf("��׼coŨ�Ȳ�=%d\r\n", Curve_CONC);
//        printf("�¶�����ֵ=%.1f\r\n",T_Var);
//        printf("��������ֵ=%d\r\n",H_Var);
        }
        LcmClear(0x00);// ����
        ShowString(0x01, 0x01, "Curve_Testing:");
        ShowString(0x01, 0x03, "Gas Collection..... ");
        ShowString(0x14, 0x03, "(RUNNING)");
//        ATM_Coeff=10000*(101325-11.67*H0)/(101325-11.67*H_Var);
#if 0
        P0_Temp = 1 - H0 / 44330.0;
        P1_Temp = 1 - H_Var / 44330.0;
        ATM_Coeff = (uint16_t)(10000 * pow(P0_Temp, 5.255) / pow(P1_Temp, 5.255));
#else
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
        if(F_DC == 1)
        {
            printf("��ѹ=%f����\r\n", BME_Pressure);
            printf("�¶�=%f���϶�\r\n", BME_Temperature);
            printf("��ѹ����ϵ��=%d\r\n", ATM_Coeff);
        }
#endif
        DaoQi_Z(200);//ͨ����Ĥ�ò�����

        //��ʼ
        for(n = 0; n < 3; n++)
        {
            LcmClear(0x00);// ����
            ShowString(0x12, 0x01, "NO.");
            ShowString(0x01, 0x01, "Curve_Testing:");
            ShowString(0x01, 0x03, "STEP(1).............. ");
            ShowString(0x14, 0x03, "(RUNNING)");
            ShowString(0x15, 0x01, Num2StrI(n + 1, "%01d"));
            Purge_Cylinder();
            Celiang_ZhunBeiNew(&CO2_CZTest, &CO2_CAD);
            if(F_DC == 1)
            {
                printf("CO2����ƽ=%d,CO2������ƽ=%d\r\n", CO2_CZTest, CO2_CAD);
            }
            // ����׼��:����CO2��̨����
            A_CO2_MV[n] = (float)(CO2_CAD - CO2_CZTest) / CO2_Coeff; //adcģʽ
            //CO2ʵ��Ũ�ȼ���
            if(F_DC == 1)
            {
                printf("��%d�β���A_CO2_MV=%f\r\n", n, A_CO2_MV[n]);
            }

            //��CO2��V/VŨ��
#if 0
            A_CO2_VV[n] = ATM_Coeff * 0.0001 * A_CO2_MV[n] * (T_Var + 273) / (T0 + 273);
#else
            A_CO2_VV[n] = ATM_Coeff * 0.0001 * A_CO2_MV[n] * (BME_Temperature + 273) / (T0 + 273);
#endif
            if(F_DC == 1)
            {
                printf("��%d��A_CO2_VV=%f\r\n", n, A_CO2_VV[n]);
            }
            SUM_ACO2_VV += A_CO2_VV[n];

            //����3�Σ�������200s
            for(i = 0; i < 3; i++)
            {
                DaoQi_Y(); //E3->E5
            }
            Purge_Cylinder();
            Paiqi_RR_PROM();// ��ϴCO2����10S
            for(i = 0; i < 3; i++)
            {
                DaoQi_B();//E4->E6

            }
            Daiji(200);//����200S ,��CO2���������
            Paiqi_R_PROM();//  100S ������ϴ����
            ShowString(0x14, 0x03, "(  END  )");
            //�ڶ��� ��ȡ����������������

            ShowString(0x01, 0x04, "STEP(2).............. ");
            ShowString(0x14, 0x04, "(RUNNING)");

            ADC(&CO_ZERO1); // ����B�������
            if(F_DC == 1)
            {
                printf("���������ֵ=%d\r\n", CO_ZERO1);
            }
            // ��ȡ����200ml(����B��)
            for(i = 0; i < 2; i++)
            {
                Chou_BGas();// �ӻ����������黷����2��E6->E1
            }
            ShowString(0x14, 0x04, "(  END  )");

            ShowString(0x01, 0x05, "STEP(3).............. ");
            ShowString(0x14, 0x05, "(RUNNING)");

            Daiji(30);// ����ʱ30S
            ADC(&CO_AD1); //  ����������B����̨��************
            if(F_DC == 1)
            {
                printf("������ƽֵ=%d\r\n", CO_AD1);
            }
            ShowString(0x14, 0x05, "(  END  )");

            ShowString(0x01, 0x06, "STEP(4).............. ");
            ShowString(0x14, 0x06, "(RUNNING)");

            Paiqi_R_PROM();//��ϴCO����100S
            ADC(&CO_ZERO2); // ����Y����̨��
            if(F_DC == 1)
            {
                printf("���������ֵ=%d\r\n", CO_ZERO2);
            }

            //�������ж�
            if(abs(CO_ZERO2 - CO_ZERO1) > 10)
            {
                LcmClear(0x00);// ����
                ShowString(0x0A, 0x03, "ZERO_Fault!");
                ShowString(0x00, 0x06, "Remove all bags,return to ");
                ShowString(0x00, 0x07, "Ready!");

                goto LAGE_CURVE;
            }
            // ��ȡ����
            for(i = 0; i < 2; i++)
            {
                Chou_YGas();// �ӷ��ݵ������黷����2��
            }
            Daiji(30);//����30S

            ADC(&CO_AD2);
            if(F_DC == 1)
            {
                printf("�����ĵ�ƽֵ=%d\r\n", CO_AD2);
            }
            ShowString(0x14, 0x06, "(  END  )");
//            if(CO_AD1<CO_ZERO1)
//            {
//                CO_AD1=0.0;
//            }
//            else
//            {
////                CO_AD1=CO_AD1-CO_ZERO1;//20171031�޸�
////            }
//            printf("��%d����������ƽ=%d\r\n",m+1,CO_AD1);
            CO_AD2_Z = CO_AD2 - CO_ZERO2; //20171031�޸�
            if(CO_AD2_Z < 0.00001)
            {
                CO_AD2_Z = 0.0;
            }

            CO_AD1_Z = CO_AD1 - CO_ZERO1;

            CO_cha[n] = CO_AD2 - CO_AD1;
            Sum_CO_cha += CO_cha[n];
            if(F_DC == 1)
            {
                printf("��%d�ε�ƽ��=%f\r\n", n + 1, CO_cha[n]);
            }
            if(n < 2)
            {
                Paiqi_RRR_PROM();
                BEEP_DOING();// ��������
                LcmClear(0x00);// ����
                ShowString(0x00, 0x06, "Remove third & fourth,next");
                while(is_gasbag_exit(2) || is_gasbag_exit(3));
                LcmClear(0x00);// ����
                ShowString(0x00, 0x06, "Plug in third & fourth,next");
                while((!is_gasbag_exit(2)) || (!is_gasbag_exit(3)));
            }

        }
        CO2_Accuracy = 100 * StandardDeviation(A_CO2_VV, 3, 0, A_CO2_CONC); //׼ȷ�ȼ���
        if(F_DC == 1)
        {
            printf("CO2׼ȷ��Ϊ��%f\r\n", CO2_Accuracy);
        }
        if(CO2_Accuracy > 4.0)
        {
            LcmClear(0x00);// ����
            ShowString(0x0A, 0x03, "CO2 FAULT");
            ShowString(0x02, 0x05, "CO2_Accuracy=");
            ShowString(0x11, 0x05, Num2StrI((uint16_t)CO2_Accuracy, "%04d"));
            goto LAGE_CURVE;
        }

        DCO_Precision = (uint16_t)(100 * StandardDeviation(CO_cha, 3, 1, 1)); //���ܶȼ���
        if(F_DC == 1)
        {
            printf("CO��ƽ���ܶȣ�%f\r\n", DCO_Precision);
        }
        if(DCO_Precision > 10.0)
        {
            LcmClear(0x00);// ����
            ShowString(0x0A, 0x03, "Dco Fault");
            ShowString(0x02, 0x05, "DCO_Precision=");
            ShowString(0x11, 0x05, Num2StrI((uint16_t)DCO_Precision, "%04d"));
            goto LAGE_CURVE;
        }
        //��MV��ʽ��COŨ��
#if 0
        Curve_CONC_MV = Curve_CONC * 10000.0 / ATM_Coeff * (273 + T0) / (273 + T_Var);
#else
        Curve_CONC_MV = Curve_CONC * 10000.0 / ATM_Coeff * (273 + T0) / (273 + BME_Temperature);
#endif
        if(F_DC == 1)
        {
            printf("M/V��ʽCO��׼��Ũ�Ȳ�=%d\r\n", Curve_CONC_MV);
        }
        //����У��
        Curve_CONC_MV = Curve_CONC_MV / (1 - SUM_ACO2_VV / 300);
        if(F_DC == 1)
        {
            printf("����У��M/V��ʽCO��׼��Ũ�Ȳ�=%d\r\n", Curve_CONC_MV);
        }
        R_XB = 10000 * Curve_CONC_MV * 0.03 / Sum_CO_cha;
        ee_WriteBytes((uint8_t *)&R_XB, 4, 2);
//        ee_WriteBytes((uint8_t *)&ATM_Coeff,14,2);
        if(F_DC == 1)
        {
            printf("R_XB=%d\r\n", R_XB);
        }
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "X_B:");
        ShowString(0x07, 0x03, Num2StrI(R_XB, "%04d"));

LAGE_CURVE:
        Paiqi_RRR_PROM();
        BEEP_DOING();// ��������
        while(is_gasbag_exit(0) || is_gasbag_exit(1) || is_gasbag_exit(2) || is_gasbag_exit(3));
    }
    else
    {
        F_RUN2 = 1; // ��ȷ����
        RUN_Program();
        F_RUN2 = 0;
    }

    RetFrame3Flag = 1; // ��������ʾ��־
    BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�

}
//�궨����

/**
  * @brief  PeiGas_PROM(void)
  * @datail ��������
  *
  * @param  none
  * @return none
  */
void PeiGas_PROM(void)
{
    uint8_t i;
    uint8_t BGas_Num = 1;
    uint8_t ZGas_Num = 1;

    if(F_tiaoji == 1) // ��������������Ȼ�˳��ص�������
    {
        //while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)));
        while((!is_gasbag_exit(0)) || (!is_gasbag_exit(1)) || (!is_gasbag_exit(2)))
		{    
			BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
	    	RetFrame3Flag = 1; // ��������ʾ��־
			framedisplayRrror();
			Delay1ms(3000);
			return ;
		}
        Ring_Num++;//20180621
        ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180621
        if(F_PC == 1)
        {
            Send_Gas_Com(); //����״̬�£�����������״̬����
        }
//	 QiGuang_Reset();//2017.6.21 ȡ��
        // �޸������ı�׼������

        LcmClear(0x00);// ����
        ShowString(0x00,0x01,"press UP or DN to set INPUT_B_GAS:");//lwj
        ShowString(0x02, 0x04, "INPUT_B_GAS:");
        ShowString(0x0E, 0x04, Num2StrI(BGas_Num, "%03d"));
        ShowString(0x05,0x07,"press RUN/SET to continue"); 
        LcmWriteCommandWith2Par(0x10, 0x04, 0x21);
        LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ
        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(BGas_Num != 0)
                    BGas_Num -= 1;
                ShowString(0x0E, 0x04, Num2StrI(BGas_Num, "%03d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(BGas_Num != 255)
                    BGas_Num += 1;
                ShowString(0x0E, 0x04, Num2StrI(BGas_Num, "%03d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;

                LcmWriteCommand(0x9C);
                break;
            }
        }

        LcmClear(0x00);// ����
        ShowString(0x00,0x01,"press UP or DN to set INPUT_Z_GAS:");//lwj
        ShowString(0x02, 0x04, "INPUT_Z_GAS:");
        ShowString(0x0E, 0x04, Num2StrI(ZGas_Num, "%03d"));
   //     ShowString(0x0C, 0x04, "+-RUN");
   		ShowString(0x05,0x07,"press RUN/SET to continue");   //lwj
        LcmWriteCommandWith2Par(0x10, 0x04, 0x21);
        LcmWriteCommand(0x9F);// ��ʾ��꣬���������˸�������ʾ

        while(1)
        {
            switch(BtnPressedFlag)
            {
            case 1://L
                BtnPressedFlag = -1;
                break;
            case 6://-
                BtnPressedFlag = -1;
                if(ZGas_Num != 0)
                    ZGas_Num -= 1;
                ShowString(0x0E, 0x04, Num2StrI(ZGas_Num, "%03d"));
                break;
            case 7://+
                BtnPressedFlag = -1;
                if(ZGas_Num != 255)
                    ZGas_Num += 1;
                ShowString(0x0E, 0x04, Num2StrI(ZGas_Num, "%03d"));
                break;
            case 5://R
                BtnPressedFlag = -1;
                break;
            }
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;

                LcmWriteCommand(0x9C);
                break;
            }
        }

        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "GAS***(    :    )");
        ShowString(0x0A, 0x03, Num2StrI(BGas_Num, "%03d"));
        ShowString(0x0E, 0x03, Num2StrI(ZGas_Num, "%03d"));

        for(i = 0; i < BGas_Num; i++)
        {
            Get_BGas();
        }
        for(i = 0; i < ZGas_Num; i++)
        {
            Get_ZGas();
        }
        LcmClear(0x00);// ����
        ShowString(0x02, 0x03, "GAS_OK");
        BEEP_DOING( );// ��������
        while((is_gasbag_exit(0)) || (is_gasbag_exit(1)) || (is_gasbag_exit(2)));

        if(F_PC == 1)
        {
            Send_Gas_Com(); //����״̬�£�����������״̬����
        }
    }
    RetFrame3Flag = 1; // ��������ʾ��־
    BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
}
//��������

/**
  * @brief  CO2_Zero(void)
  * @datail ��ȡCO2���
  *
  * @param  none
  * @return none
  * @note   ����ֵ��ȫ�ֱ�������
  */
// ����CO2������ƽ
void CO2_Zero(void)
{
    // CO2��������
    QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();

    Get_Zerro_Gas();// �ӻ���������������CO2����

    Daiji(40);
    CO2_Test();
//	 ADC(&CO2_ZERO);
    ADC2_ValConvt(&CO2_ZERO);
    ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);

    LcmClear(0x00);// ����
    ShowString(0x01, 0x03, "CO2_ZERO:");
    ShowString(0x0A, 0x03, Num2StrI(CO2_ZERO, "%06d"));

    while(1)
    {
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            break;
        }
    }
    // SET������֮�����CO2�ı궨

    CO2_percent1();


}
/**
  * @brief  CO2_percentl(void)
  * @datail ��ȡCO2(1%)̨��
  *
  * @param  none
  * @return none
  * @note  ����ֵ��ȫ�ֱ�������
  */
void CO2_percent1(void)
{
    uint16_t CO2_ADVal;

    LcmClear(0x00);// ����
    ShowString(0x02, 0x03, "CO2:");
    ShowString(0x0C, 0x04, "+-RUN");
    ShowString(0x09, 0x03, Num2StrI(CO2_B, "%06d"));
    LcmWriteCommandWith2Par(0x0E, 0x03, 0x21); // д����
    LcmWriteCommand(0x9F);

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(CO2_B != 0)
                CO2_B -= 1;
            ShowString(0x09, 0x03, Num2StrI(CO2_B, "%06d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(CO2_B != 1000)
                CO2_B += 1;
            ShowString(0x09, 0x03, Num2StrI(CO2_B, "%06d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;

            LcmWriteCommand(0x9C);
            break;
        }
    }
    LcmClear(0x00);// ����
    ShowString(0x01, 0x06, "Sample_Identify.....");

    QiGuang_Reset();
    Open_CO2_Paiqi();

    Daiji(60);// 60S��ϴCO2����

    Close_CO2_Paiqi();

    Get_CO2_GAS(3);// ��E3���CO2����

    Daiji(40);
    CO2_Test();
//	  ADC(&CO2_ADVal);
    ADC2_ValConvt(&CO2_ADVal);

    CO2_Taijie = (uint16_t)(CO2_ADVal - CO2_ZERO) / (CO2_B * 0.01);
    ee_WriteBytes((uint8_t *)&CO2_Taijie, 6, 2);

    LcmClear(0x00);// ����
    ShowString(0x02, 0x03, "CO2(1%):");
    ShowString(0x0B, 0x03, Num2StrI(CO2_Taijie, "%06d"));

    Open_CO2_Paiqi();
    Daiji(60); // 60S��ϴCO2����
    Close_CO2_Paiqi();
    BEEP_DOING( );// ��������


}
/**
  * @brief  CO2_Cal_Pro(void)
  * @datail CO2Ũ����������
  *
  * @param  none
  * @return none
  * @note ����ֵ��ȫ�ֱ�������
  */

void CO2_Cal_Pro(void)
{
    uint16_t CO2_Act_Val;
    uint16_t CO2_Act_Sum = 0;
    uint16_t CO2_Zero_Sum = 0;
//    uint16_t H_Var=0;//�����������20180717
//    float T_Var=25.0;//�¶��������20180908
    uint8_t i;
    uint16_t CO2_Zero_Val;
//	  double P0_Temp,P1_Temp;

    LcmClear(0x00);// ����
    ShowString(0x00,0x01,"press UP or DN to set CO2:");//lwj
    ShowString(0x02, 0x04, "CO2:");
//    ShowString(0x0C, 0x04, "+-RUN");
	ShowString(0x05,0x07,"press RUN/SET to continue");	 //lwj

    ShowString(0x09, 0x04, Num2StrI(CO2_B, "%06d"));
    LcmWriteCommandWith2Par(0x0E, 0x04, 0x21); // д����
    LcmWriteCommand(0x9F);

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(CO2_B != 0)
                CO2_B -= 1;
            ShowString(0x09, 0x04, Num2StrI(CO2_B, "%06d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(CO2_B != 1000)
                CO2_B += 1;
            ShowString(0x09, 0x04, Num2StrI(CO2_B, "%06d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;

            LcmWriteCommand(0x9C);
            break;
        }
    }
#if 0
    //���뺣�θ߶�
    LcmClear(0x00);// ����
    ShowString(0x02, 0x03, "Altitude:");
    ShowString(0x0C, 0x05, "+-RUN");
    ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
    ShowString(0x11, 0x03, "m");
    LcmWriteCommandWith2Par(0x0F, 0x03, 0x21); // д����
    LcmWriteCommand(0x9F);

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            if(H_Var > 99)
                H_Var -= 100;
            ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(H_Var != 0)
                H_Var -= 1;
            ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(H_Var != 5000) //����ܳ���5000��
                H_Var += 1;
            ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            if(H_Var < 4900) //����ܳ���5000��
                H_Var += 100;
            ShowString(0x0C, 0x03, Num2StrI(H_Var, "%04d"));
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            LcmWriteCommand(0x9C);
            break;
        }
    }
    /*
    	 �¶�����
    	 20180710
    	 */
    LcmClear(0x00);// ����
    ShowString(0x01, 0x03, "Temperature:");
    ShowString(0x0C, 0x05, "+-RUN");
    ShowString(0x0D, 0x03, Num2StrF(T_Var, "%02.1f"));
    LcmWriteCommandWith2Par(0x10, 0x03, 0x21); // д����
    LcmWriteCommand(0x9F);

    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 1://L
            BtnPressedFlag = -1;
            break;
        case 6://-
            BtnPressedFlag = -1;
            if(T_Var != 0.0)
                T_Var -= 0.1;
            ShowString(0x0D, 0x03, Num2StrF(T_Var, "%02.1f"));
            break;
        case 7://+
            BtnPressedFlag = -1;
            if(T_Var != 50.0)
                T_Var += 0.1;
            ShowString(0x0D, 0x03, Num2StrF(T_Var, "%02.1f"));
            break;
        case 5://R
            BtnPressedFlag = -1;
            break;
        }
        if(BtnPressedFlag == 4)
        {
            BtnPressedFlag = -1;
            LcmWriteCommand(0x9C);
            break;
        }
    }
#endif
//    printf("CO2����ֵ(*100)��%d\r\n",CO2_B);
//    printf("��������ֵ��%d\r\n",H_Var);
//    printf("�¶�����ֵ��%.1f\r\n",T_Var);
    LcmClear(0x00);// ����
    ShowString(0x01, 0x06, "Sample_Identify.....");
    //��ȡ�¶Ⱥ���ѹ 20181222
    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
    printf("��ѹ=%f����\r\n", BME_Pressure);
    printf("�¶�=%f���϶�\r\n", BME_Temperature);
    QiGuang_Reset();
    for(i = 0; i < 3; i++)
    {
        Purge_Cylinder();
        ShowString(0x16, 0x06, "NO.");
        ShowString(0x19, 0x06, Num2StrI(i + 1, "%01d"));
        Celiang_ZhunBeiNew(&CO2_Zero_Val, &CO2_Act_Val);
        if(F_DC == 1)
        {
            printf("��%d��CO2����ƽ=%d,CO2������ƽ=%d\r\n", i + 1, CO2_Zero_Val, CO2_Act_Val);
        }
        Paiqi_RR_PROM();// ��ϴCO2����10 S
        CO2_Zero_Sum += CO2_Zero_Val;
        CO2_Act_Sum += CO2_Act_Val;
    }
#if 0   //20181218����BME280������
    P0_Temp = 1 - H0 / 44330.0;
    P1_Temp = 1 - H_Var / 44330.0;
    CO2_B *= pow(P1_Temp, 5.255) / pow(P0_Temp, 5.255) * (T0 + 273) / (T_Var + 273); //�Ա�׼����V/Vת��Ϊm/v
//    CO2_B*=(101325-11.67*H_Var)/(101325-11.67*H0)*(T0+273)/(T_Var+273);//�Ա�׼����V/Vת��Ϊm/v
#else
    CO2_B *= BME_Pressure / 1013.25 * (T0 + 273) / (BME_Temperature + 273);
#endif


    CO2_Coeff = (uint16_t)(CO2_Act_Sum - CO2_Zero_Sum) / (CO2_B * 0.03); //CO2_B����λ������������100����CO2_Act_Sum����λ��������100��������������ҳ���100������3�������β���ƽ��ֵ,mVת��ģʽ
    if(F_DC == 1)
    {
        printf("����У����궨������Ũ��(*100):%d\r\n", CO2_B);
        printf("CO2_Coeff=%d\r\n", CO2_Coeff);
    }

    CO2_ZERO = CO2_Zero_Sum / 3;

    ee_WriteBytes((uint8_t *)&CO2_Coeff, 6, 2);
    ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);

    LcmClear(0x00);// ����
    ShowString(0x02, 0x03, "CO2_Coeff:");
    ShowString(0x0E, 0x03, Num2StrI(CO2_Coeff, "%04d"));
    ShowString(0x02, 0x04, "CO2_ZERO:");
    ShowString(0x0E, 0x04, Num2StrI(CO2_ZERO, "%04d"));

    Open_CO2_Paiqi();
    Daiji(60); // 60S��ϴCO2����
    Close_CO2_Paiqi();
    BEEP_DOING( );// ��������
}
/**
  * @brief  CO2_CON(void)
  * @datail CO2�궨����
  * @param  none
  * @return none
  */
void CO2_CON(void)
{
    if(F_tiaoji == 1) // ��������CO2У������, �����ǵ����˻�������
    {
        while(!is_gasbag_exit(0))
		{    
			
	    	RetFrame3Flag = 1; // ��������ʾ��־
			framedisplayRrror();
			Delay1ms(3000);
			BtnPressedFlag = -1; //������ʼ����������й����д����Ұ��Ŀ����ԡ�
			return ;
		}
        Ring_Num++;//20180625
        ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180625
        LcmClear(0x00);// ����
        ShowString(0x01, 0x02, "CO2_CAL_TEST?");

        while(1)
        {
#if 0
            if(BtnPressedFlag == 0)
            {
                BtnPressedFlag = -1;
                CO2_Zero();
                break;
            }
            else if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                CO2_percent1();
                break;
            }
#endif
            if(BtnPressedFlag == 4)
            {
                BtnPressedFlag = -1;
                CO2_Cal_Pro();
                break;
            }
        }

        while(is_gasbag_exit(0));
        RetFrame3Flag = 1; // ��������ʾ��־
    }
}
/**
  * @brief  StandardDeviation(float *P,uint8_t num,uint8_t select,uint16_t Stand)
  * @datail ���ܶȺ�׼ȷ��
  *
  * @param  p �������ݵ�ַ
  * @param  num �������ݵĸ���
  * @param  select ���ܶȺ�׼ȷ�ȵ�ѡ��Ϊ1ʱ���о��ܶȼ��㣬Ϊ0ʱ����׼ȷ�ȼ���
  * @param  Stand ׼ȷ�Լ������õı�׼ֵ
  * @return R_SD ׼ȷ�Ȼ��ܶ�
  */
float StandardDeviation(float *P, uint8_t num, uint8_t select, uint16_t Stand)
{
    float stdd;//��׼ƫ��ֵ
    float R_SD;
    float sum = 0; //���
    float avg;//ƽ��ֵ
    float Sum_Dev = 0;
    uint8_t m;
    for(m = 0; m < num; m++) //���
    {
        sum += P[m];
    }
    avg = sum / num; //���ֵ
//    printf("��ֵ=%f\r\n",avg);
    if(select == 1) //���ܶȼ���
    {
        for(m = 0; m < num; m++)
        {
            Sum_Dev += (P[m] - avg) * (P[m] - avg);
        }
    }
    else  //׼ȷ�ȼ���
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
//        printf("���ܶ�R_SD=%f\r\n",R_SD);
    }
    else
    {
        R_SD = stdd / (Stand * 0.01);
//        printf("׼ȷ��R_SD=%f\r\n",R_SD);
    }
    return R_SD;

}

