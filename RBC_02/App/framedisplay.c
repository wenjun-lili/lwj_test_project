#include "framedisplay.h"
#include "bsp.h"
#include "DWIN.h"
#include "globalval.h"
#include "BME280.h"

extern uint8_t RetFrame3Flag;

uint16_t Warming_up_Cnt;
extern struct rtc_time systmtime;
extern uint16_t DGUS_Layout_Flag;

/**
  *@breif  预热流程
  *@param  none
  *@retval none
  */
void framedisplay2(void)
{
    uint8_t temp[2];
    temp[0] = 0x04;
    temp[1] = 0xB0;
    Wr_82_Scr(0x0001, temp, 2);	//预热时间赋初值1200s
    if(F_PC == 1)
    {
        FS_INT_ON();// 初始化进行中
    }
    //Warming_up_Cnt = 10; //1200s倒计时
	Warming_up_Cnt = 1200;// 2019/12/16  lwj   把预热模式下的10s改成1200秒  
    EV_Open(6);
    EV_Open(2);
    Delay10ms(1);
    GPIO_SetBits(GPIOE, GPIO_Pin_15); //气泵启动
    Delay100ms(1);
    TIM_Cmd(TIM7, ENABLE);

WARM:
    if(Warming_up_Cnt < 1170) // 倒计时
    {
        EV_Open(1); //先开CO气室电磁阀
        Delay10ms(1);
        EV_Close(2);//再关CO2电磁阀
        Delay100ms(1);
    }
    temp[0] = 0x00;
    temp[1] = 0x02;
    Wr_80_Scr(0x03, temp, 2); //从页面1切换到页面2
    while(1)
    {
        if(Warming_up_Cnt == 0)
        {
            TIM_Cmd(TIM7, DISABLE);
            break;
        }
        if(Warming_up_Cnt == 1170) // 倒计时
        {
            EV_Open(1); //先开CO气室电磁阀
            Delay100ms(1);
            EV_Close(2);//再关CO2电磁阀
            Delay100ms(1);
        }
        if(BtnPressedFlag == 0xBB) // 程序若切换到密码输入界面
        {
            BtnPressedFlag = 0;
            framedisplay29();//进入密码输入界面
            if(F_tiaoji == 0)
            {
                goto WARM;
            }
            else
            {
                TIM_Cmd(TIM7, DISABLE);
            }
            break;
        }
    }
    close_paiqi(); //关闭CO排气
    EV_Close(2); //关闭CO2气室
    if(F_PC == 1)
    {
        Send_Gas_Com(); //联机状态下，发送气袋的状态命令
        FS_INT_OK();// 初始化OK
        FS_READY();// 给PC机发送空闲指令
    }

}
/**
  *@breif 工程师模式输入密码界面
  *@param  none
  *@retval none
  */
void framedisplay29(void)
{
    uint8_t temp[2] = {0x00, 0x1D};
    Wr_80_Scr(0x03, temp, 2); //从02预热界面跳到80工程师密码输入界面
    while(1)
    {
        if(BtnPressedFlag == 0xAA) //取消键
        {
            BtnPressedFlag = 0;
            F_tiaoji = 0;
            break;
        }
        if(BtnPressedFlag == 0xBB) //确认键
        {
            BtnPressedFlag = 0;
            temp[0] = 0x00;
            temp[1] = 0x21;
            Rd_83_Scr(0x0021, 1);
            Delay1ms(5);
            if((Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x21))
            {
                Key_Input = Com3RBuf[7] * 256 + Com3RBuf[8];
                if(Key_Input == Password)
                {
                    F_tiaoji = 1;
                    break;
                }
                else
                {
                    temp[0] = 0x00;
                    temp[1] = 0x3D;
                    Wr_80_Scr(0x03, temp, 2); //跳到密码错误提示界面
                    while(1)
                    {
                        //按确认键返回输入密码界面
                        if(BtnPressedFlag == 0XBB)
                        {
                            BtnPressedFlag = 0;
                            break;
                        }
                    }
                    temp[0] = 0x00;
                    temp[1] = 0x1D;
                    Wr_80_Scr(0x03, temp, 2);
                }
            }
        }
    }
}

/**
  *@breif  用户待机界面
	*@param  none
	*@retval none
	*/
void framedisplay3(void)
{
    uint8_t temp[2] = {0x00, 0x03};
    Wr_80_Scr(0x03, temp, 2);
}
/**
  *@breif  工程师待机界面
	*@param  none
	*@retval none
	*/
void framedisplay4(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x04;
    Wr_80_Scr(0x03, temp, 2);

}
/**
  *@breif  零点电平显示程序
	*@detail 零点每一秒获取一次
	*@param  none
	*@retval none
*/
void framedisplay_zero(void)
{
    uint16_t  CO_RZERO;
    int16_t CO_cha;
    uint8_t temp[2];

    ADC(&CO_RZERO);//测量CO值

    CO_cha = CO_RZERO - CO_ZERO; //计算co零点值

    if(F_Zero == 1)
    {
        printf("z=%d\r\n", CO_cha);
    }

    temp[0] = CO_cha >> 8; //高位先进
    temp[1] = CO_cha;
    Wr_82_Scr(0x0002, temp, 2);

}

/**
  *@breif  参数配置总界面
	*@param  none
	*@retval none
	*/
void framedisplay5(void)
{
    uint8_t temp1[2];
    uint8_t i = 0;
    temp1[0] = 0x00;
    temp1[1] = 0x05;
    Wr_80_Scr(0x03, temp1, 2);
    while(1)
    {
        switch(BtnPressedFlag)
        {
        case 0x66: //日期时间密封圈配置
            BtnPressedFlag = 0;
            framedisplay8();
            Wr_80_Scr(0x03, temp1, 2);
            break;
        case 0x99://参数设置
            BtnPressedFlag = 0;
            Set_Para();
            Wr_80_Scr(0x03, temp1, 2);
            break;
        case 0xAB://版本号查看
            BtnPressedFlag = 0;
            framedisplay44();
            Wr_80_Scr(0x03, temp1, 2);
            break;
        case 0xAA://返回待机界面
            BtnPressedFlag = 0;
            i = 1;
            break;
        default:
            break;
        }
        if(i == 1)
        {
            RetFrame3Flag = 1;
            break;
        }
    }

}

/**
  *@breif  用户模式下时间和密封圈配置界面
	*@detail 由于时间可单独在DGUS屏上进行设置，无需涉及到STM32，所以不进行配置
	*@param  none
	*@retval none
	*/
void framedisplay8(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x08;
    Wr_80_Scr(0x03, temp, 2);
    while(1)
    {
        if(BtnPressedFlag == 0xAA)
        {
            BtnPressedFlag = 0;
            if(F_tiaoji == 1)
            {
                break;
            }
            else
            {
                RetFrame3Flag = 1;
                break;
            }
        }
        if(BtnPressedFlag == 0x44)
        {
            BtnPressedFlag = 0;
            temp[0] = 0;
            temp[1] = 0;
            Wr_82_Scr(0x0009, temp, 2);
            Ring_Num = 0;
            ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180228-lcy
        }
    }
}
void framedisplay9(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x09;
    Wr_80_Scr(0x03, temp, 2);

}
/**
  *@breif  标准测量流程界面11程序
  *@param  none
  *@retval none
  */

void framedisplay11(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x0B;
    Wr_80_Scr(0x03, temp, 2);
    while(1)
    {
        if((ReceiveFlag6 == 1)&&(PC_ctr_Flag == 0 ))
        {
            if((RBC_pc_ComR[0] == 0x20) && (RBC_pc_ComR[1] == 0x00) && (RBC_pc_ComR[2] == 0x00) && (RBC_pc_ComR[3] == 0x00) && (RBC_pc_ComR[4] == 0x00) && (RBC_pc_ComR[5] == 0x20))
            {
                RBCtoPC_Cmd_00();
                Delay1ms(5);
                PC_ctr_Flag = 1; //PC控制测量赋值1
            }
            ReceiveFlag6 = 0;
        }
        if((BtnPressedFlag == 0xBB) || (PC_ctr_Flag == 1)) //按下测量键或者pc控制测量标志为1
        {
            BtnPressedFlag = 0;
            framedisplay12();
        }
        if(BtnPressedFlag == 0xAA) //返回待机界面
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            PC_ctr_Flag = 0;
            temp[0] = 0;
            temp[1] = 0;
            Wr_82_Scr(0x000A, temp, 2); //置0
            Wr_82_Scr(0x000E, temp, 2);
            Wr_82_Scr(0x0060, temp, 2);
            break;
        }
    }

}
/**
  *@breif  界面12程序
	*@param  none
	*@retval none
  */
void framedisplay12(void)
{
    uint8_t temp[2];
    uint16_t runTimeOut = 0; //超时
    uint8_t  sendNum = 0; //串口重新发送次数

    while(!is_gasbag_exit(0));
    if(F_PC == 1)
    {
        FS_GasYes(0); //联机状态下，发送气袋的状态命令
        Delay10ms(1);
        FS_GasYes(4);//发送全部插到位
        Delay10ms(1);
    }
    while(F_PC)
    {
        runTimeOut++;
        if(ReceiveFlag6 == 1)
        {
            ReceiveFlag6 = 0;
            PC_Val_Analy();
        }
        if(F_Sex == 1) ///收到上位机软件发送的性别值
        {
            F_Sex = 0;
            Delay10ms(1);
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
        Sex = Gender;
    }

    if(PC_ctr_Flag == 1)
    {
        temp[0] = HB >> 8;
        temp[1] = HB;
        Wr_82_Scr(0x000E, temp, 2); //将HB值写入触摸屏中

        Delay1ms(1);
        temp[0] = 0;
        temp[1] = Gender;
        Wr_82_Scr(0x0060, temp, 2); //将性别值写入触摸屏中
    }
    else
    {
        /*获取血红蛋白质值*/
        Rd_83_Scr(0x000E, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0E) && (Com3RBuf[6] == 0x01))
        {
            HB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }

        /*获取性别*/
        Rd_83_Scr(0x0060, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x60) && (Com3RBuf[6] == 0x01))
        {
            Sex = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }
    }
    if(F_DC == 1)
    {
        printf("输入的性别=%d\r\n", Sex);
        printf("输入的血红蛋白浓度值=%d\r\n", HB);
    }
    temp[0] = 0x00;
    temp[1] = 0x0C;
    Wr_80_Scr(0x03, temp, 2); //正式进入12标准测量界面
    RUN_Program();//开始测量

}
/**
  *@breif  精确测量流程首页
	*@param  none
	*@retval none
*/
void framedisplay20(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x14;
    Wr_80_Scr(0x03, temp, 2);
    while(1)
    {
        if((ReceiveFlag6 == 1)&&(PC_ctr_Flag == 0 ))
        {
            if((RBC_pc_ComR[0] == 0x20) && (RBC_pc_ComR[1] == 0x00) && (RBC_pc_ComR[2] == 0x00) && (RBC_pc_ComR[3] == 0x00) && (RBC_pc_ComR[4] == 0x00) && (RBC_pc_ComR[5] == 0x20))
            {
                RBCtoPC_Cmd_00();
                Delay10ms(1);
                PC_ctr_Flag = 1; //PC控制测量赋值1
            }
            ReceiveFlag6 = 0;
        }
        if((BtnPressedFlag == 0xBB) || (PC_ctr_Flag == 1)) //按下测量键或者pc控制测量标志为1
        {
            BtnPressedFlag = 0;
            framedisplay21();
        }
        if(BtnPressedFlag == 0xAA) //返回待机界面
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            temp[0] = 0;
            temp[1] = 0;
            Wr_82_Scr(0x000A, temp, 2);
            Wr_82_Scr(0x000E, temp, 2);
            Wr_82_Scr(0x0060, temp, 2);
            break;
        }
    }

}
/**
  *@breif  精确测量模式第2页
	*@param  none
	*@retval none
*/
void framedisplay21(void)
{
    uint8_t temp[2];
    uint16_t runTimeOut = 0; //超时
    uint8_t  sendNum = 0; //串口重新发送次数

    while(!is_gasbag_exit(0));
    if(F_PC == 1)
    {
        FS_GasYes(0); //联机状态下，发送气袋的状态命令
        Delay1ms(5);
        FS_GasYes(4);//发送全部插到位，有待于与上位机确认
        Delay1ms(5);
    }

    while(F_PC)
    {
        runTimeOut++;
        if(ReceiveFlag6 == 1)
        {
            ReceiveFlag6 = 0;
            PC_Val_Analy();
        }
        if(F_Sex == 1) ///收到上位机软件发送的性别值
        {
            F_Sex = 0;
            Delay10ms(1);
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

    if(PC_ctr_Flag == 1)
    {
        temp[0] = HB >> 8;
        temp[1] = HB;
        Wr_82_Scr(0x000E, temp, 2); //将HB值写入触摸屏中

        Delay1ms(1);
        temp[0] = 0;
        temp[1] = Gender;
        Wr_82_Scr(0x0060, temp, 2); //将性别值写入触摸屏中
    }
    else
    {

        /*获取血红蛋白质值*/
        Rd_83_Scr(0x000E, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0E) && (Com3RBuf[6] == 0x01))
        {
            HB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }

        /*获取性别*/
        Rd_83_Scr(0x0060, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x60) && (Com3RBuf[6] == 0x01))
        {
            Sex = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }
    }

    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
    temp[0] = Ring_Num >> 8;
    temp[1] = Ring_Num;
    Wr_82_Scr(0x0009, temp, 2);

    if(F_DC == 1)
    {
        printf("输入的性别=%d\r\n", Sex);
        printf("输入的血红蛋白浓度值=%d\r\n", HB);
    }

    temp[0] = 0x00;
    temp[1] = 0x15;
    Wr_80_Scr(0x03, temp, 2); //正式进入21标准测量界面01
    Adj_RUN_Program();//开始测量
}
/**
  *@breif  界面30程序-CO2标定首界面
	*@param  none
	*@retval none
*/
void framedisplay30(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};

    temp[0] = 0x00;
    temp[1] = 0x1E;
    Wr_80_Scr(0x03, temp, 2); //进入界面30

    while(1)
    {
        if(BtnPressedFlag == 0xBB)
        {
            BtnPressedFlag = 0;
            CO2_CON();//开始进入CO2标定
        }
        if(BtnPressedFlag == 0xAA)
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            Wr_82_Scr(0x0012, temp1, 2); //CO2标定标气浓度归零
            break;
        }
    }
}
/**
  *@breif  CO标定34界面程序
	*@param  none
	*@retval none
*/
void framedisplay34(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};
    temp[0] = 0x00;
    temp[1] = 0x22;
    Wr_80_Scr(0x03, temp, 2); //跳到34CO标定00界面
    while(1)
    {
        if(BtnPressedFlag == 0xBB) //按下开始，则进入
        {
            BtnPressedFlag = 0;
            Curve_PROM();
        }
        if(BtnPressedFlag == 0xAA) //按下返回，则进入待机界面
        {
            BtnPressedFlag = 0;
            Wr_82_Scr(0x0013, temp1, 2);
            Wr_82_Scr(0x0012, temp1, 2);
            RetFrame3Flag = 1;
            break;
        }
    }

}
void framedisplay37(void)
{
    uint8_t temp[2];

    temp[0] = 0x00;
    temp[1] = 0x25;
    Wr_80_Scr(0x03, temp, 2); //跳到37质控01界面

    while(1)
    {

        if((ReceiveFlag6 == 1)&&(PC_ctr_Flag == 0 ))
        {
            if((RBC_pc_ComR[0] == 0x20) && (RBC_pc_ComR[1] == 0x01) && (RBC_pc_ComR[2] == 0x00) && (RBC_pc_ComR[3] == 0x00) && (RBC_pc_ComR[4] == 0x00) && (RBC_pc_ComR[5] == 0x21))
            {
                RBCtoPC_Cmd_00();
                Delay10ms(1);
                PC_ctr_Flag = 1; //PC控制测量赋值1
            }
            ReceiveFlag6 = 0;
        }
        if((BtnPressedFlag == 0xBB) || (PC_ctr_Flag == 1)) //按下测量键或者pc控制测量标志为1
        {
            BtnPressedFlag = 0;
            QC_PROM();
        }
        if(BtnPressedFlag == 0xAA) //返回待机界面
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            PC_ctr_Flag = 0;
            temp[0] = 0;
            temp[1] = 0;
            Wr_82_Scr(0x0013, temp, 2); //置0
            Wr_82_Scr(0x0012, temp, 2);
            Wr_82_Scr(0x000A, temp, 2);
            Wr_82_Scr(0x0032, temp, 2);
            Wr_82_Scr(0x0080, temp, 2);
            break;
        }
    }
}

//版本号查看界面
void framedisplay44(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x2C;
    Wr_80_Scr(0x03, temp, 2);
    while(1)
    {
        if(BtnPressedFlag == 0xAA)
        {
            BtnPressedFlag = 0;
            break;
        }

    }
}
/**
  *@breif  气密性检验程序
	*@param  none
	*@retval none
*/
void framedisplay45(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x2D;
    Wr_80_Scr(0x03, temp, 2);
    EValve_All_Close();//关闭所有电磁阀
    while(1)
    {
        if(BtnPressedFlag == 0xAA) //返回待机界面
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            break;
        }
        else if(BtnPressedFlag == 0xBB) //开启电磁阀
        {
            BtnPressedFlag = 0;
            EV_Open(1);
            EV_Open(2);
            EV_Open(3);
            EV_Open(4);
            EV_Open(5);
            temp[0] = 0x00;
            temp[1] = 0x2E;
            Wr_80_Scr(0x03, temp, 2); //进入46气密性检验01界面
            while(1)
            {
                if(BtnPressedFlag == 0xAF) //关闭电磁阀
                {
                    BtnPressedFlag = 0;
                    temp[0] = 0x00;
                    temp[1] = 0x2D;
                    Wr_80_Scr(0x03, temp, 2); //进入45气密性检验00界面
                    EValve_All_Close();//关闭所有电磁阀
                    break;
                }
                else if(BtnPressedFlag == 0xAA)
                {
                    EValve_All_Close();//关闭所有电磁阀
                    break;//返回待机界面，此处不清零，跳出循环后，在外面循环进行清零跳出气密性检验界面
                }
            }
            //开启电磁阀
        }
    }

}
/**
  *@breif  界面47-配气界面
	*@param  none
	*@retval none
*/
void framedisplay47(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};
    temp[0] = 0x00;
    temp[1] = 0x2F;
    Wr_80_Scr(0x03, temp, 2);
    while(1)
    {
        if(BtnPressedFlag == 0xAA)
        {
            BtnPressedFlag = 0;
            Wr_82_Scr(0x001E, temp1, 2); //界面数值清零
            Wr_82_Scr(0x001F, temp1, 2);
            RetFrame3Flag = 1;
            break;
        }
        if(BtnPressedFlag == 0xBB)
        {
            BtnPressedFlag = 0;
            PeiGas_PROM();//配气
        }
    }

}

/**
  *@breif  自检流程
  *@detail 主要检验密封圈、CO2传感器、CO电平和三合一传感器、干燥剂失效性校验
  *@param  none
  *@retval none
  */
void framedisplay80(void)
{
    uint8_t temp[2];

    temp[0] = 0x00;
    temp[1] = 0x50;
    Wr_80_Scr(0x03, temp, 2); //进入设备自检界面

    CO2_CO_SensorZERO();// CO与CO2 ZERO 故障判断

    Rubber_Ring_Fault();//橡胶圈使用次数预警-20180228

    Bme280Fault();//对传感器故障进行诊断
}

/**
  *@breif  待机界面选择
	*@param  none
	*@retval none
	*/
void framedisplayStandby(void)
{
    if(F_tiaoji == 0)
    {
        framedisplay3();//03用户待机界面
    }
    else
    {
        framedisplay4();//04工程师待机界面
    }
}
/**
  *@breif  设置界面选择
	*@param  none
	*@retval none
	*/

void framedisplaySet(void)
{
    if(F_tiaoji == 1)
    {
        framedisplay5();//进入工程师设置模式
    }
    else
    {
        framedisplay8();//进入用户设置模式
    }
}





