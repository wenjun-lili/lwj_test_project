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
  *@breif  Ԥ������
  *@param  none
  *@retval none
  */
void framedisplay2(void)
{
    uint8_t temp[2];
    temp[0] = 0x04;
    temp[1] = 0xB0;
    Wr_82_Scr(0x0001, temp, 2);	//Ԥ��ʱ�丳��ֵ1200s
    if(F_PC == 1)
    {
        FS_INT_ON();// ��ʼ��������
    }
    //Warming_up_Cnt = 10; //1200s����ʱ
	Warming_up_Cnt = 1200;// 2019/12/16  lwj   ��Ԥ��ģʽ�µ�10s�ĳ�1200��  
    EV_Open(6);
    EV_Open(2);
    Delay10ms(1);
    GPIO_SetBits(GPIOE, GPIO_Pin_15); //��������
    Delay100ms(1);
    TIM_Cmd(TIM7, ENABLE);

WARM:
    if(Warming_up_Cnt < 1170) // ����ʱ
    {
        EV_Open(1); //�ȿ�CO���ҵ�ŷ�
        Delay10ms(1);
        EV_Close(2);//�ٹ�CO2��ŷ�
        Delay100ms(1);
    }
    temp[0] = 0x00;
    temp[1] = 0x02;
    Wr_80_Scr(0x03, temp, 2); //��ҳ��1�л���ҳ��2
    while(1)
    {
        if(Warming_up_Cnt == 0)
        {
            TIM_Cmd(TIM7, DISABLE);
            break;
        }
        if(Warming_up_Cnt == 1170) // ����ʱ
        {
            EV_Open(1); //�ȿ�CO���ҵ�ŷ�
            Delay100ms(1);
            EV_Close(2);//�ٹ�CO2��ŷ�
            Delay100ms(1);
        }
        if(BtnPressedFlag == 0xBB) // �������л��������������
        {
            BtnPressedFlag = 0;
            framedisplay29();//���������������
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
    close_paiqi(); //�ر�CO����
    EV_Close(2); //�ر�CO2����
    if(F_PC == 1)
    {
        Send_Gas_Com(); //����״̬�£�����������״̬����
        FS_INT_OK();// ��ʼ��OK
        FS_READY();// ��PC�����Ϳ���ָ��
    }

}
/**
  *@breif ����ʦģʽ�����������
  *@param  none
  *@retval none
  */
void framedisplay29(void)
{
    uint8_t temp[2] = {0x00, 0x1D};
    Wr_80_Scr(0x03, temp, 2); //��02Ԥ�Ƚ�������80����ʦ�����������
    while(1)
    {
        if(BtnPressedFlag == 0xAA) //ȡ����
        {
            BtnPressedFlag = 0;
            F_tiaoji = 0;
            break;
        }
        if(BtnPressedFlag == 0xBB) //ȷ�ϼ�
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
                    Wr_80_Scr(0x03, temp, 2); //�������������ʾ����
                    while(1)
                    {
                        //��ȷ�ϼ����������������
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
  *@breif  �û���������
	*@param  none
	*@retval none
	*/
void framedisplay3(void)
{
    uint8_t temp[2] = {0x00, 0x03};
    Wr_80_Scr(0x03, temp, 2);
}
/**
  *@breif  ����ʦ��������
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
  *@breif  ����ƽ��ʾ����
	*@detail ���ÿһ���ȡһ��
	*@param  none
	*@retval none
*/
void framedisplay_zero(void)
{
    uint16_t  CO_RZERO;
    int16_t CO_cha;
    uint8_t temp[2];

    ADC(&CO_RZERO);//����COֵ

    CO_cha = CO_RZERO - CO_ZERO; //����co���ֵ

    if(F_Zero == 1)
    {
        printf("z=%d\r\n", CO_cha);
    }

    temp[0] = CO_cha >> 8; //��λ�Ƚ�
    temp[1] = CO_cha;
    Wr_82_Scr(0x0002, temp, 2);

}

/**
  *@breif  ���������ܽ���
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
        case 0x66: //����ʱ���ܷ�Ȧ����
            BtnPressedFlag = 0;
            framedisplay8();
            Wr_80_Scr(0x03, temp1, 2);
            break;
        case 0x99://��������
            BtnPressedFlag = 0;
            Set_Para();
            Wr_80_Scr(0x03, temp1, 2);
            break;
        case 0xAB://�汾�Ų鿴
            BtnPressedFlag = 0;
            framedisplay44();
            Wr_80_Scr(0x03, temp1, 2);
            break;
        case 0xAA://���ش�������
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
  *@breif  �û�ģʽ��ʱ����ܷ�Ȧ���ý���
	*@detail ����ʱ��ɵ�����DGUS���Ͻ������ã������漰��STM32�����Բ���������
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
  *@breif  ��׼�������̽���11����
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
                PC_ctr_Flag = 1; //PC���Ʋ�����ֵ1
            }
            ReceiveFlag6 = 0;
        }
        if((BtnPressedFlag == 0xBB) || (PC_ctr_Flag == 1)) //���²���������pc���Ʋ�����־Ϊ1
        {
            BtnPressedFlag = 0;
            framedisplay12();
        }
        if(BtnPressedFlag == 0xAA) //���ش�������
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            PC_ctr_Flag = 0;
            temp[0] = 0;
            temp[1] = 0;
            Wr_82_Scr(0x000A, temp, 2); //��0
            Wr_82_Scr(0x000E, temp, 2);
            Wr_82_Scr(0x0060, temp, 2);
            break;
        }
    }

}
/**
  *@breif  ����12����
	*@param  none
	*@retval none
  */
void framedisplay12(void)
{
    uint8_t temp[2];
    uint16_t runTimeOut = 0; //��ʱ
    uint8_t  sendNum = 0; //�������·��ʹ���

    while(!is_gasbag_exit(0));
    if(F_PC == 1)
    {
        FS_GasYes(0); //����״̬�£�����������״̬����
        Delay10ms(1);
        FS_GasYes(4);//����ȫ���嵽λ
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
        if(F_Sex == 1) ///�յ���λ��������͵��Ա�ֵ
        {
            F_Sex = 0;
            Delay10ms(1);
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
                PC_ctr_Flag = 0; //��λ��������λ����־����
                break;
            }
        }
        Sex = Gender;
    }

    if(PC_ctr_Flag == 1)
    {
        temp[0] = HB >> 8;
        temp[1] = HB;
        Wr_82_Scr(0x000E, temp, 2); //��HBֵд�봥������

        Delay1ms(1);
        temp[0] = 0;
        temp[1] = Gender;
        Wr_82_Scr(0x0060, temp, 2); //���Ա�ֵд�봥������
    }
    else
    {
        /*��ȡѪ�쵰����ֵ*/
        Rd_83_Scr(0x000E, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0E) && (Com3RBuf[6] == 0x01))
        {
            HB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }

        /*��ȡ�Ա�*/
        Rd_83_Scr(0x0060, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x60) && (Com3RBuf[6] == 0x01))
        {
            Sex = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }
    }
    if(F_DC == 1)
    {
        printf("������Ա�=%d\r\n", Sex);
        printf("�����Ѫ�쵰��Ũ��ֵ=%d\r\n", HB);
    }
    temp[0] = 0x00;
    temp[1] = 0x0C;
    Wr_80_Scr(0x03, temp, 2); //��ʽ����12��׼��������
    RUN_Program();//��ʼ����

}
/**
  *@breif  ��ȷ����������ҳ
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
                PC_ctr_Flag = 1; //PC���Ʋ�����ֵ1
            }
            ReceiveFlag6 = 0;
        }
        if((BtnPressedFlag == 0xBB) || (PC_ctr_Flag == 1)) //���²���������pc���Ʋ�����־Ϊ1
        {
            BtnPressedFlag = 0;
            framedisplay21();
        }
        if(BtnPressedFlag == 0xAA) //���ش�������
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
  *@breif  ��ȷ����ģʽ��2ҳ
	*@param  none
	*@retval none
*/
void framedisplay21(void)
{
    uint8_t temp[2];
    uint16_t runTimeOut = 0; //��ʱ
    uint8_t  sendNum = 0; //�������·��ʹ���

    while(!is_gasbag_exit(0));
    if(F_PC == 1)
    {
        FS_GasYes(0); //����״̬�£�����������״̬����
        Delay1ms(5);
        FS_GasYes(4);//����ȫ���嵽λ���д�������λ��ȷ��
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
        if(F_Sex == 1) ///�յ���λ��������͵��Ա�ֵ
        {
            F_Sex = 0;
            Delay10ms(1);
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
                PC_ctr_Flag = 0; //��λ��������λ����־����
                break;
            }
        }
    }

    if(PC_ctr_Flag == 1)
    {
        temp[0] = HB >> 8;
        temp[1] = HB;
        Wr_82_Scr(0x000E, temp, 2); //��HBֵд�봥������

        Delay1ms(1);
        temp[0] = 0;
        temp[1] = Gender;
        Wr_82_Scr(0x0060, temp, 2); //���Ա�ֵд�봥������
    }
    else
    {

        /*��ȡѪ�쵰����ֵ*/
        Rd_83_Scr(0x000E, 1);
        Delay1ms(1);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x0E) && (Com3RBuf[6] == 0x01))
        {
            HB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        }

        /*��ȡ�Ա�*/
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
        printf("������Ա�=%d\r\n", Sex);
        printf("�����Ѫ�쵰��Ũ��ֵ=%d\r\n", HB);
    }

    temp[0] = 0x00;
    temp[1] = 0x15;
    Wr_80_Scr(0x03, temp, 2); //��ʽ����21��׼��������01
    Adj_RUN_Program();//��ʼ����
}
/**
  *@breif  ����30����-CO2�궨�׽���
	*@param  none
	*@retval none
*/
void framedisplay30(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};

    temp[0] = 0x00;
    temp[1] = 0x1E;
    Wr_80_Scr(0x03, temp, 2); //�������30

    while(1)
    {
        if(BtnPressedFlag == 0xBB)
        {
            BtnPressedFlag = 0;
            CO2_CON();//��ʼ����CO2�궨
        }
        if(BtnPressedFlag == 0xAA)
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            Wr_82_Scr(0x0012, temp1, 2); //CO2�궨����Ũ�ȹ���
            break;
        }
    }
}
/**
  *@breif  CO�궨34�������
	*@param  none
	*@retval none
*/
void framedisplay34(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};
    temp[0] = 0x00;
    temp[1] = 0x22;
    Wr_80_Scr(0x03, temp, 2); //����34CO�궨00����
    while(1)
    {
        if(BtnPressedFlag == 0xBB) //���¿�ʼ�������
        {
            BtnPressedFlag = 0;
            Curve_PROM();
        }
        if(BtnPressedFlag == 0xAA) //���·��أ�������������
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
    Wr_80_Scr(0x03, temp, 2); //����37�ʿ�01����

    while(1)
    {

        if((ReceiveFlag6 == 1)&&(PC_ctr_Flag == 0 ))
        {
            if((RBC_pc_ComR[0] == 0x20) && (RBC_pc_ComR[1] == 0x01) && (RBC_pc_ComR[2] == 0x00) && (RBC_pc_ComR[3] == 0x00) && (RBC_pc_ComR[4] == 0x00) && (RBC_pc_ComR[5] == 0x21))
            {
                RBCtoPC_Cmd_00();
                Delay10ms(1);
                PC_ctr_Flag = 1; //PC���Ʋ�����ֵ1
            }
            ReceiveFlag6 = 0;
        }
        if((BtnPressedFlag == 0xBB) || (PC_ctr_Flag == 1)) //���²���������pc���Ʋ�����־Ϊ1
        {
            BtnPressedFlag = 0;
            QC_PROM();
        }
        if(BtnPressedFlag == 0xAA) //���ش�������
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            PC_ctr_Flag = 0;
            temp[0] = 0;
            temp[1] = 0;
            Wr_82_Scr(0x0013, temp, 2); //��0
            Wr_82_Scr(0x0012, temp, 2);
            Wr_82_Scr(0x000A, temp, 2);
            Wr_82_Scr(0x0032, temp, 2);
            Wr_82_Scr(0x0080, temp, 2);
            break;
        }
    }
}

//�汾�Ų鿴����
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
  *@breif  �����Լ������
	*@param  none
	*@retval none
*/
void framedisplay45(void)
{
    uint8_t temp[2];
    temp[0] = 0x00;
    temp[1] = 0x2D;
    Wr_80_Scr(0x03, temp, 2);
    EValve_All_Close();//�ر����е�ŷ�
    while(1)
    {
        if(BtnPressedFlag == 0xAA) //���ش�������
        {
            BtnPressedFlag = 0;
            RetFrame3Flag = 1;
            break;
        }
        else if(BtnPressedFlag == 0xBB) //������ŷ�
        {
            BtnPressedFlag = 0;
            EV_Open(1);
            EV_Open(2);
            EV_Open(3);
            EV_Open(4);
            EV_Open(5);
            temp[0] = 0x00;
            temp[1] = 0x2E;
            Wr_80_Scr(0x03, temp, 2); //����46�����Լ���01����
            while(1)
            {
                if(BtnPressedFlag == 0xAF) //�رյ�ŷ�
                {
                    BtnPressedFlag = 0;
                    temp[0] = 0x00;
                    temp[1] = 0x2D;
                    Wr_80_Scr(0x03, temp, 2); //����45�����Լ���00����
                    EValve_All_Close();//�ر����е�ŷ�
                    break;
                }
                else if(BtnPressedFlag == 0xAA)
                {
                    EValve_All_Close();//�ر����е�ŷ�
                    break;//���ش������棬�˴������㣬����ѭ����������ѭ�������������������Լ������
                }
            }
            //������ŷ�
        }
    }

}
/**
  *@breif  ����47-��������
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
            Wr_82_Scr(0x001E, temp1, 2); //������ֵ����
            Wr_82_Scr(0x001F, temp1, 2);
            RetFrame3Flag = 1;
            break;
        }
        if(BtnPressedFlag == 0xBB)
        {
            BtnPressedFlag = 0;
            PeiGas_PROM();//����
        }
    }

}

/**
  *@breif  �Լ�����
  *@detail ��Ҫ�����ܷ�Ȧ��CO2��������CO��ƽ������һ�������������ʧЧ��У��
  *@param  none
  *@retval none
  */
void framedisplay80(void)
{
    uint8_t temp[2];

    temp[0] = 0x00;
    temp[1] = 0x50;
    Wr_80_Scr(0x03, temp, 2); //�����豸�Լ����

    CO2_CO_SensorZERO();// CO��CO2 ZERO �����ж�

    Rubber_Ring_Fault();//��Ȧʹ�ô���Ԥ��-20180228

    Bme280Fault();//�Դ��������Ͻ������
}

/**
  *@breif  ��������ѡ��
	*@param  none
	*@retval none
	*/
void framedisplayStandby(void)
{
    if(F_tiaoji == 0)
    {
        framedisplay3();//03�û���������
    }
    else
    {
        framedisplay4();//04����ʦ��������
    }
}
/**
  *@breif  ���ý���ѡ��
	*@param  none
	*@retval none
	*/

void framedisplaySet(void)
{
    if(F_tiaoji == 1)
    {
        framedisplay5();//���빤��ʦ����ģʽ
    }
    else
    {
        framedisplay8();//�����û�����ģʽ
    }
}





