#include "framedisplay.h"
#include "bsp.h"


uint16_t Warming_up_Cnt;
extern struct rtc_time systmtime;

/**
  * @brief  framediaplay0(void)
	* @datail Onilining显示
	* @param  none
	* @return none
  */
void framediaplay0(void)
{
    uint8_t i;
    LcmClear(0x00);// 清屏
    ShowString( 0x01,0x01, "Onlining");
    for(i=0; i<16; i++)
    {
        ShowString( 0x0A+i,1, ".");
        Delay1ms(200); // 200ms
        if(i==8)
        {
            if(ReceiveFlag1==0)
            {
                RBCtoPC_Cmd_AA();//发送0xAA联络信号
            }
        }
    }
}
/**
  * @brief  framedisplay1(void)
	* @datail 预热界面显示
	* @param  none
	* @return none
  */
void framedisplay1(void)
{
    LcmClear(0x00);// 清屏
    ShowString( 1,1, "Warming up.......");
    ShowString(0x17,0x07,"(    )");
    if(F_PC==1)
    {
        FS_INT_ON();// 初始化进行中
    }

    Warming_up_Cnt=1200;
    ShowString(0x17,0x07,"(    )");
    ShowString(0x18,0x07, Num2StrI(Warming_up_Cnt, "%4d"));
    TIM_Cmd(TIM7, ENABLE);

    Open_CO2_Paiqi(); // 排气   打开E2，E7，气泵工作

    while(1)
    {
        if(Warming_up_Cnt==0)
        {
            TIM_Cmd(TIM7, DISABLE);
            break;
        }
        if(Warming_up_Cnt==1170)// 倒计时
        {
            EV_Open(1); //先开CO气室电磁阀
            Delay100ms(1);
            EV_Close(2);//再关CO2电磁阀
            Delay100ms(1);
        }
        if(F_tiaoji==1) // 程序若切换到调机程序
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

    close_paiqi(); //关闭CO排气
    EV_Close(2); //在调机程序下确保关闭CO2气室
    Delay100ms(1);

    if(F_PC==1)
    {
        Send_Gas_Com(); //联机状态下，发送气袋的状态命令
        FS_INT_OK();// 初始化OK
    }

}
/**
  * @brief  framedisplay_ok(void)
	* @datail OK显示
	* @param  none
	* @return none
  */
void framedisplay_ok(void)
{
    ShowString(0x17,0x07,"( ok )");
}
/**
  * @brief  framedisplay3(void)
	* @datail 待机界面显示
	* @param  none
	* @return none
  */
void framedisplay3(void)
{

    LcmClear(0x00);// 清屏
    

    if(F_tiaoji==1)
    {
        //ShowString(0x15,0x07,"ST QU RUN");		lwj
			
			ShowString(0x0,0x04,"RUN/SET: to input parameter");
			ShowString(0x0,0x05,"INPUT  : to measure");
			ShowString(0x0,0x06,"UP:to CO2_Calibt      DN:to QC");
			ShowString(0x0,0x07,"LT:to Conf_Gas RT:to CO_Calibt");
			ShowString(0x11,0x01,"Engineer Mode");
    }
    else
    {
			ShowString(0x00,0x05,"Press RUN/SET to set param");
			ShowString(0x00,0x06,"Press DN key to input QC");
			ShowString(0x00,0x07,"Press INPUT key to HB");
			ShowString(0x12,0x01,"User Mode");
    }
    ShowString(0x01,0x01,"ZERO:");
    ShowString(0x0a,0x02,"    -  -     :  :  ");



}
/**
  * @brief  framedisplay3_sub(void)
	* @datail 待机界面动态数据显示
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
    ShowString(0x01,0x05,Num2StrI(systmtime.tm_year,"%04d"));// 0x0E处是年的个位
    ShowString(0x06,0x05,Num2StrI(systmtime.tm_mon,"%02d"));
    ShowString(0x09,0x05,Num2StrI(systmtime.tm_mday,"%02d"));

    ShowString(0x0C,0x05,Num2StrI(systmtime.tm_hour,"%02d"));
    ShowString(0x0F,0x05,Num2StrI(systmtime.tm_min,"%02d"));
    ShowString(0x12,0x05,Num2StrI(systmtime.tm_sec,"%02d"));


    SELET_CO();//模拟开关打开CO模拟信号

    if(k==0) // 第一次采样，给CO_RZERO赋值
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

//  printf("CO zero real test value:%d\r\n",CO_RZERO);// 调试用

    CO_cha=abs(CO_RZERO-CO_ZERO);
    ShowString(0x06,0x01,Num2StrI(CO_cha,"%06d"));

    if(F_PC==1)
    {
        ShowString(0x1A,0x01,"(**)");
        FS_READY();// 给PC机发送空闲指令
    }

    ShowString(0x13,0x01,"Ready!");


#else
    uint16_t  CO_RZERO;
    int16_t CO_cha;

    RTCTimeTransfer(&systmtime);
    ShowString(0x0a,0x02,Num2StrI(systmtime.tm_year,"%04d"));// 0x0E处是年的个位
    ShowString(0x0f,0x02,Num2StrI(systmtime.tm_mon,"%02d"));
    ShowString(0x12,0x02,Num2StrI(systmtime.tm_mday,"%02d"));

    ShowString(0x15,0x02,Num2StrI(systmtime.tm_hour,"%02d"));
    ShowString(0x18,0x02,Num2StrI(systmtime.tm_min,"%02d"));
    ShowString(0x1b,0x02,Num2StrI(systmtime.tm_sec,"%02d"));


    SELET_CO();//模拟开关打开CO模拟信号

    ADC(&CO_RZERO);

//  printf("CO zero real test value:%d\r\n",CO_RZERO);// 调试用

    CO_cha=CO_RZERO-CO_ZERO;
//		printf("Z1:%d\r\n",CO_cha);//发送零点值，利于长期观察
    if(F_Zero==1)
    {
        printf("Z1:%d\r\n",CO_cha);//发送零点值，利于长期观察
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
//        FS_READY();// 给PC机发送空闲指令
        //  FS_ZERO(&CO_cha);
    }

    
#endif
}
/**
  * @brief  framedisplayDate(void)
	* @datail 日期设置界面
	* @param  none
	* @return none
  */
void framedisplayDate(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN and RT or LT to");
    ShowString(0x00,0x02,"set Date(Y/M/D)");
    ShowString(0x04,0x05,"Date:      /   /  ");
	
	ShowString(0x05,0x07,"Press RUN/SET to continue");			//lwj

    /* 显示当前时间 */
    RTCTimeTransfer(&systmtime);
    // 空了2个字符
    ShowString(0x0B,0x05,Num2StrI(systmtime.tm_year,"%04d"));// 0x0E处是年的个位
    ShowString(0x11,0x05,Num2StrI(systmtime.tm_mon,"%02d"));
    ShowString(0x15,0x05,Num2StrI(systmtime.tm_mday,"%02d"));

    LcmWriteCommandWith2Par(0x0E,0x05,0x21); // 写入光标
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示

    cursorx=0x0E;
    cursory=0x05;
}
/**
  * @brief  framedisplaytime(void)
	* @datail 时间设置界面
	* @param  none
	* @return none
  */
void framedisplaytime(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN and RT or LT to");
    ShowString(0x00,0x02,"set Time(HR/MIN/SEC)");
    ShowString(0x04,0x05,"Time:    :   :");
	
	ShowString(0x05,0x07,"Press RUN/SET to continue");			//lwj

    /* 显示当前时间 */
    RTCTimeTransfer(&systmtime);
    ShowString(0x0B,0x05,Num2StrI(systmtime.tm_hour,"%02d"));
    ShowString(0x0F,0x05,Num2StrI(systmtime.tm_min,"%02d"));
    ShowString(0x13,0x05,Num2StrI(systmtime.tm_sec,"%02d"));

    LcmWriteCommandWith2Par(0x0C,0x05,0x21); // 写入光标
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示

    cursorx=0x0C;
    cursory=0x05;
}
/**
  * @brief  framedisplayZERO(void)
	* @datail 零点ZERO设置界面
	* @param  none
	* @return none
  */
void framedisplayZERO(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN to set ZERO:");//lwj
    ShowString(0x02,0x04,"ZERO:");
    ShowString(0x07,0x04,Num2StrI(CO_ZERO,"%06d"));
    //ShowString(0x0c,0x04,"+-RUN");
    LcmWriteCommandWith2Par(0x0C,0x04,0x21); // 写入光标
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
}
/**
  * @brief  framedisplayCO2(void)
	* @datail CO2阈值设置界面
	* @param  none
	* @return none
  */
void framedisplayCO2(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x02,0x03,"CO2:    %");
    ShowString(0x07,0x03,Num2StrI(CO2_PB,"%03d"));
    ShowString(0x0c,0x04,"+-RUN");
    LcmWriteCommandWith2Par(0x09,0x03,0x21); // 写入光标
}
/**
  * @brief  framedisplayX_B(void)
	* @datail X-B设置界面
	* @param  none
	* @return none
  */
void framedisplayX_B(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN to set X_B:");//lwj
    ShowString(0x02,0x04,"X_B:");
    //ShowString(0x0c,0x04,"+-RUN");
    ShowString(0x07,0x04,Num2StrI(R_XB,"%06d"));
    LcmWriteCommandWith2Par(0x0C,0x04,0x21); // 写入光标
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
        LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
}
/**
  * @brief  framedisplayGasDefault(void)
	* @param  none
	* @return none
  */
void framedisplayGasDefault(void)
{
    LcmClear(0x00);//清屏
    
    ShowString(0x00,0x01,"Press UP or DN to set Sample  type:");//lwj
	ShowString(0x02,0x04,"Sample type:");
	ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
	
    LcmWriteCommandWith2Par(0x11,0x04,0x21); // 写入光标
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示

	
	//ShowString(0x0c,0x04,"+-RUN");
	if(F_GasType==1)
	{
	  ShowString(0x0E,0x04,"Auto Gas");
	}
	else
	{
	  ShowString(0x0E,0x04,"Blow    ");
	}
		
}
/*
函数功能： 测量模式中的Sample type界面
*/
void framedisplayGas_celiang_Default(void)
{
	LcmClear(0x00);//清屏
	
	ShowString(0x00,0x01,"Press UP or DN to set Sample  type:");//lwj
	ShowString(0x02,0x04,"Sample type:");
	ShowString(0x00,0x06,"Then press RUN/SET to start   testing.");	 //lwj
						  
	LcmWriteCommandWith2Par(0x0f,0x04,0x21); // 写入光标
	LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
	//ShowString(0x0c,0x04,"+-RUN");
	if(F_GasType==1)
	{
	  ShowString(0x0E,0x04,"Auto Gas");
	}
	else
	{
	  ShowString(0x0E,0x04,"Blow	");
	}
		
}

/**
  * @brief  framedisplayGasFactor(void)
	* @param  none
	* @return none
  */
void framedisplayGasFactor(void)
{
    LcmClear(0x00);//清屏
	ShowString(0x00,0x01,"Press UP or DN to set Gas     factor:");//lwj

	ShowString(0x02,0x04,"Gas factor:");
	//ShowString(0x0c,0x04,"+-RUN");
	ShowString(0xE,0x04,Num2StrI(gasTypeFactor,"%03d"));		
	LcmWriteCommandWith2Par(0x10,0x04,0x21); // 写入光标
	ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj	
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示

		
}
/**
  * @brief  framedisplayCO2Coeff(void)
	* @datail CO2系数显示界面
	* @param  none
	* @return none
  */
void framedisplayCO2Coeff(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x02,0x04,"CO2_Coeff:");
    ShowString(0x0E,0x04,Num2StrI(CO2_Coeff,"%06d"));
    LcmWriteCommandWith2Par(0x12,0x04,0x21); // 写入光标
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
        LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
}
/**
  * @brief  framedisplayATMCoeff(void)
	* @datail 气压修正系数显示
	* @param  none
	* @return none
  */
void framedisplayATMCoeff(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x02,0x03,"ATM_Coeff:");
    ShowString(0x0E,0x03,Num2StrI(ATM_Coeff,"%06d"));
    LcmWriteCommandWith2Par(0x0E,0x03,0x21); // 写入光标
}
/**
  * @brief  framedisplayVersion(void)
	* @datail 版本号显示界面
	* @param  none
	* @return none
  */
void framedisplayVersion(void)
{
    LcmClear(0x00);// 清屏

    ShowString(0x02,0x04,"Version: V1.4.6");//20190215
	//20191217  V1.4.5	友好界面修改初版本
	//20191219  V1.4.6	根据V1.4.5版本界面出现的一些小问题进行修改
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
}

/**
  * @brief  framedisplayH0(void)
	* @datail 设置参考海拔高度
	* @param  none
	* @return none
  */
void framedisplayH0(void)
{
    LcmClear(0x00);
    ShowString(0x02,0x03,"H0:     m");
    ShowString(0x05,0x03,Num2StrI(H0,"%05d"));
    LcmWriteCommandWith2Par(0x09,0x03,0x21); // 写入光标
}

/**
  * @brief  framedisplayT0(void)
	* @datail 设置参考温度
	* @param  none
	* @return none
  */
void framedisplayT0(void)
{
    LcmClear(0x00);
	ShowString(0x00,0x01,"Press UP or DN to set T0:");//lwj
    ShowString(0x02,0x04,"T0:      ");
    ShowString(0x05,0x04,Num2StrF(T0,"%2.1f"));
    LcmWriteCommandWith2Par(0x08,0x04,0x21); // 写入光标
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
}

/**
  * @brief  framedisplayRubberRing(void)
	* @datail 橡胶圈使用次数显示
	* @param  none
	* @return none
  */
void framedisplayRubberRing(void)
{
	LcmClear(0x00);// 清屏
	ShowString(0x00,0x01,"If Rubber Ring renewed,press  RT to set zero,otherwise press  RUN/SET to continue.");//lwj
    ee_ReadBytes((uint8_t *)&Ring_Num,10,2);
    ShowString(0x05,0x06,"Rubber Ring:");
    ShowString(0x11,0x06,Num2StrI(Ring_Num,"%04d"));
	
    LcmWriteCommandWith2Par(0x14,0x06,0x21); // 写入光标
        LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
}
/**
  * @brief  framedisplayZEROCO2(void)
	* @datail CO2零点台阶数显示
	* @param  none
	* @return none
  */
void framedisplayZEROCO2(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN to set CO2_ZERO:");//lwj
    ShowString(0x01,0x04,"CO2_ZERO:");
    ShowString(0x0B,0x04,Num2StrI(CO2_ZERO,"%06d"));
    LcmWriteCommandWith2Par(0x10,0x04,0x21); // 写入光标
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示

}
/**
  * @brief  framedisplayRrror(void)
	* @datail 误操作界面
	* @param  none
	* @return none
  */
void framedisplayRrror(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x01,0x02,"The bag is not plugged in.   Please insert the bag. It will return automatically after 10 seconds!");
   

}

/**
  * @brief  framedisplayRun(void)
	* @datail HB输入界面显示
	* @param  none
	* @return none
  */
void framedisplayRun(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x01,0x01,"Press Up or DN to input HB:");
    ShowString(0x01,0x06,"Press RUN/SET to select");
    ShowString(0x01,0x07,"Gender");
    ShowString(0x02,0x04,"HB:        (g/L)");
    ShowString(0x05,0x04,Num2StrI(HB,"%03d"));
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
    LcmWriteCommandWith2Par(0x07,0x04,0x21); // 写入光标
}
/**
  * @brief  Dis_Sample_Test(void)
	* @datail 样本测量显示
	* @param  none
	* @return none
  */
void Dis_Sample_Test(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x01,0x01,"Sample_Testing:");
}
/**
  * @brief  framedisplayQC(void)
	* @datail 质控参数输入界面
	* @param  none
	* @return none
  */
void framedisplayQC(void)
{
    LcmClear(0x00);// 清屏
//	ShowString(0x01,0x01,"Sample_Testing:");
//	ShowString(0x01,0x06,"press RUN key to start");
    ShowString(0x00,0x00,"Press UP or DN to input QC_GAS");
    ShowString(0x00,0x01,"Difference");
    ShowString(0x00,0x05,"Press RUN/SET to cmpl QC or press LT to stop QC and Return to Ready!");


//	ShowString(0x00,0x07, "Ready!");
    ShowString(0x02,0x03,"QC_CO Gas:");
    ShowString(0x0C,0x03,Num2StrI(QC_CONC,"%06d"));
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
    LcmWriteCommandWith2Par(0x11,0x03,0x21); // 写入光标

}
/**
  * @brief  framedisplayQCCO2(void)
	* @datail 质控CO2输入界面
	* @param  none
	* @return none
  */
void framedisplayQCCO2(void)
{
    LcmClear(0x00);// 清屏
//	ShowString(0x01,0x01,"Sample_Testing:");
//	ShowString(0x01,0x06,"press RUN key to start");
    ShowString(0x00,0x00,"Press UP or DN to input QC_GAS Difference");
    ShowString(0x00,0x01,"Difference");
    ShowString(0x00,0x05,"Press RUN to cmpl QC or Press LT to stop QC and Return to Ready!");

//	ShowString(0x00,0x07, "Ready!");
    ShowString(0x02,0x03,"QC_CO2 Gas:");
    ShowString(0x0D,0x03,Num2StrI(QC_CO2_CONC,"%06d"));
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
    LcmWriteCommandWith2Par(0x12,0x03,0x21); // 写入光标
}
/**
  * @brief  framdisplayCurve(void)
	* @datail 标定CO浓度输入界面
	* @param  none
	* @return none
  */
void framdisplayCurve(void)
{
    // 做曲线用标准气CO的浓度
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN to set CO_ppm:");//lwj
    ShowString(0x02,0x03,"CO_ppm:");
   // ShowString(0x0c,0x04,"+-RUN");
	ShowString(0x05,0x07,"Press RUN/SET to continue");	 //lwj

   ShowString(0x0A,0x03,Num2StrI(QC_CONC,"%06d"));
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
    LcmWriteCommandWith2Par(0x0F,0x03,0x21); // 写入光标
}
/**
  * @brief  framdisplayZeroFault(void)
	* @datail ZERO_Fault显示界面
	* @param  none
	* @return none
  */
void framdisplayZeroFault(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x0A,0x03,"ZERO_Fault!");
    ShowString(0x00,0x06,"Remove all bags,return to ");
    ShowString(0x00,0x07,"Ready!");
}

/**
  * @brief  framedisplayHumidity(void)
	* @datail CO2阈值设置界面
	* @param  none
	* @return none
  */
void framedisplayHumidity(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x00,0x01,"Press UP or DN to set Humidity threshold:");//lwj
    ShowString(0x02,0x04,"Humidity threshold:   %");
    ShowString(0x16,0x04,Num2StrI(HumidityThreshold,"%02d"));
    //ShowString(0x0c,0x04,"+-RUN");
    LcmWriteCommandWith2Par(0x16,0x04,0x21); // 写入光标
    ShowString(0x05,0x07,"Press RUN/SET to continue");   //lwj
    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示

}
/**
  *@brief  framedisplayAirTight()
  *@datail 气密性检验界面
  *@param  void
  *@retval void
  */
void framedisplayAirTight(void)
{
    LcmClear(0x00);// 清屏
    ShowString(0x03,0x03,"AIR TIGHT TEST");
}

/**
  *@brief framedisplaySex(uint8_t m)
  *@datail 调整性别界面
  *@param m:性别
  *@retval void
  */
void framedisplaySex(uint8_t m)
{
    LcmClear(0x00);// 清屏
    ShowString(0x01,0x01,"Press Up to select Gender:");
    ShowString(0x01,0x04,"Gender:       ");
    if(m==0)
    {
        ShowString(0x09,0x04,"Female");
    }
    else
    {
        ShowString(0x09,0x04," Male ");
    }
//    ShowString(0x14,0x06,"+-RUN");
    ShowString(0x01,0x06,"Press RUN/SET to select");
    ShowString(0x01,0x07,"Sample type");

    LcmWriteCommand(0x9F);// 显示光标，启动光标闪烁，光标显示
    LcmWriteCommandWith2Par(0x0E,0x04,0x21); // 写入光标
}
