#include "Measurement.h"
#include "bsp.h"
#include "DWIN.h"
#include "globalval.h"
#include "BME280.h"


extern struct rtc_time systmtime;
extern uint8_t RetFrame3Flag;

/**
  *@brief ����λ����������
  *@param  none
  *@retval none
  */
void OnlinePC (void)
{
    uint8_t temp[2] = {0x00, 0x01}; //ҳ��1
    uint16_t numm = 0;

    Delay100ms(20);//2s

    if(ReceiveFlag1 == 0) //����λ��δ�������������źţ�����0xAA�����ź�
    {
        RBCtoPC_Cmd_AA();//����0xAA�����ź�
        while(!ReceiveFlag)
        {
            numm++;
            Delay10ms(1);
            if(numm > 150) //1.5sδ���յ��ظ���������
            {
                break;
            }
        }
    }
    if(ReceiveFlag == 1) // ����������������
    {
        ReceiveFlag = 0;

        if(ReceiveFlag1 == 0)
        {
            switch(RBC_pc_ComR[0])
            {
            case 0xBB://����Ӧ��
                RBC_pc_ComR[0] = 0; // ��0
                F_PC = 1; //������־
                temp[0] = 0x00;
                temp[1] = 0x01;
                Wr_82_Scr(0x0070, temp, 2);
                break;
            case 0x99://ά��ģʽӦ�𣬿��Բ鿴��������ֵ
                RBC_pc_ComR[0] = 0;
                F_DC = 1;
                USART_SendData(USART1, (uint8_t)0xAD);
                while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
                break;
            case 0xEE://��ƽ��ӡģʽӦ��
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
    TIM6Delay_f(100);// ��ʱ0.1S
}
/**
  *@breif  ��������
	*@detail ��Ϊ��������ʹ�õ��ǰ��������Զ��ϴ����ܣ����������ж� 20180329
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
// 20180411�޸ģ�ԭʼ���⣺�޸���ɺ�������ֵ��ˢ��Ϊԭʼ��ֵ
            //��ȡCO2_PB����ֵ
            Rd_83_Scr(0x0003, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x03) && (Com3RBuf[6] == 0x01))
                CO2_PB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO2_PB=%d\r\n", CO2_PB);

            //��ȡCO_ZERO����ֵ
            Rd_83_Scr(0x0004, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x04) && (Com3RBuf[6] == 0x01))
                CO_ZERO = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO_ZERO=%d\r\n", CO_ZERO);

            //��ȡR_XB����ֵ
            Rd_83_Scr(0x0005, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x05) && (Com3RBuf[6] == 0x01))
                R_XB = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("Kco(x-b)=%d\r\n", R_XB);

            //��ȡCO2�_Taijie����ֵ
            Rd_83_Scr(0x0006, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x06) && (Com3RBuf[6] == 0x01))
                CO2_Taijie = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO2_Taijie=%d\r\n", CO2_Taijie);

            //��ȡCO2_ZERO����ֵ
            Rd_83_Scr(0x0007, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x07) && (Com3RBuf[6] == 0x01))
                CO2_ZERO = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("CO2_ZERO=%d\r\n", CO2_ZERO);

            //��ȡATM_Coeffֵ
            Rd_83_Scr(0x0030, 1);
            Delay100us(10);
            if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x30) && (Com3RBuf[6] == 0x01))
                ATM_Coeff = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            printf("��ѹ����ϵ��=%d\r\n", ATM_Coeff);

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
  *@breif  ��׼��������
  *@param  none
  *@retval none
  */
void RUN_Program(void)//��׼����
{
    uint8_t i;
    uint8_t Zero_Num = 0; //�����ִ���Ĵ���
    uint8_t temp[2];
    uint8_t temp1[4];
    float CO_Cha;
    float CO_AD2_Z, CO_AD1_Z; //������CO��ƽ��������ƽ�Ĳ�ֵ����ʱ����
    float PcoR, Pco; // COʵ��ֵ
    float CO2_CONC; // CO2Ũ��
    uint16_t Diff_Zero;
    uint16_t RBC;
    uint16_t CO2_AD, CO2_ZTest; // ʵ��CO2̨���� �Լ�����CO2���
    uint16_t CO_ZERO1, CO_ZERO2;
    uint16_t CO_AD1, CO_AD2;
    uint16_t nnnn = 0;

    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
    temp[0] = Ring_Num >> 8;
    temp[1] = Ring_Num;
    Wr_82_Scr(0x0009, temp, 2);

    /*����������CO2*/
    if(F_PC == 1)
    {
        FS_RUN_step1_ON();
    }
    temp[0] = 0x00;
    temp[1] = 0x0D;
    Wr_80_Scr(0x03, temp, 2); //����13��׼����02����

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);    //�����¶Ⱥ���ѹ
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x000A, temp, 2);

    Purge_Cylinder();
    Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);// ��ϴCO2���Ҳ��������
    Paiqi_RR_PROM();// ��ϴCO2����10 S

    CO2_CONC = (float)(CO2_AD - CO2_ZTest) / CO2_Taijie;

    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);

    if(F_DC == 1)
    {
        printf("��ѹУ��ϵ��=%d\r\n", ATM_Coeff);
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
        printf("CO2���ֵ=%d,CO2��ƽֵ=%d\r\n", CO2_ZTest, CO2_AD);
        printf("CO2Ũ��M/V=%f\r\n", CO2_CONC);
    }

    CO2_CONC = CO2_CONC * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / 301;

    if(F_DC == 1)
    {
        printf("CO2Ũ��V/V=%f\r\n", CO2_CONC);
    }
    temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
    temp[1] = (uint16_t)(CO2_CONC * 100);
    Wr_82_Scr(0x0022, temp, 2);

    if(CO2_CONC < 0.50) //ʵ��ֵС�ڵ���0.5%
    {
        temp[0] = 0x00;
        temp[1] = 0x3C;
        Wr_80_Scr(0x03, temp, 2); //����60CO2Ũ�ȹ��ʹ������
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
        F_CO2_Low = 1; //������̼���ͱ�־
        F_CO2_Num = 0;
    }
    else
    {
        F_CO2_Num = 0;
    }
    temp[0] = 0x00;
    temp[1] = 0x0E;
    Wr_80_Scr(0x03, temp, 2);	//����14��׼����4����
    if(F_PC == 1)
    {
        FS_RUN_step1_OK();
    }
LAGE_TEST:
    temp[0] = 0x00;
    temp[1] = 0x0F;
    Wr_80_Scr(0x03, temp, 2); //	����15��׼����5����
    if(F_PC == 1)
    {
        FS_RUN_step2_ON();
    }

    /*��������CǪ��*/
    Purge_Cylinder();
    Paiqi_R_PROM();//��ϴCO����100S
    ADC(&CO_ZERO1); //��B��CO���ZERO1
    if(F_DC == 1)
    {
        printf("�������ֵ=%d\r\n", CO_ZERO1);
    }
    for(i = 0; i < 2; i++)
    {
        Chou_BGas();// �ӻ����������黷����2��E5->E1
    }
    Daiji(30);// ����ʱ30S
    ADC(&CO_AD1); //  ����������B����̨��
    if(F_DC == 1)
    {
        printf("������ƽֵ=%d\r\n", CO_AD1);
    }
    if(F_PC == 1)
    {
        FS_RUN_step2_OK();
    }

    temp[0] = 0x00;
    temp[1] = 0x10;
    Wr_80_Scr(0x03, temp, 2); //	����16��׼����6����

    /*��������CǪ��*/
    if(F_PC == 1)
    {
        FS_RUN_step3_ON();
    }
    temp[0] = 0x00;
    temp[1] = 0x11;
    Wr_80_Scr(0x03, temp, 2); //	����17��׼����7����
    Paiqi_R_PROM();//��ϴCO����100S

    ADC(&CO_ZERO2); //Y��CO���
    if(F_DC == 1)
    {
        printf("�������ֵ=%d\r\n", CO_ZERO2);
    }
    //�������ж�
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
                Daiji(180);//����180s
                goto LAGE_TEST;//�����ڶ���������������

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

    for(i = 0; i < 2; i++)
    {
        Chou_YGas();// �ӻ����������黷����2��  E4->E1
    }

    Daiji(30);// ����ʱ30S
    ADC(&CO_AD2); // ����Y����̨��
    if(F_DC == 1)
    {
        printf("������ƽֵ=%d\r\n", CO_AD2);
    }
    CO_AD2_Z = CO_AD2 - CO_ZERO2;
    if(CO_AD2_Z <= 0.00001) //�����ж���������㻹�͵����
    {
        CO_AD2_Z = 0.0;
    }
    //����CO������̨�ײ�
    CO_AD1_Z = CO_AD1 - CO_ZERO1;
    if(CO_AD1_Z <= 0.0001)
    {
        CO_AD1_Z = 0.0;
    }

    CO_Cha = CO_AD2 - CO_AD1;
    PcoR = CO_Cha * (float)R_XB / 10000; // ppm  ����ֵ��M/Vģʽ
    Pco = PcoR * ATM_Coeff * 0.0001 * (273 + BME_Temperature) / (273 + T0); //������ѹ���¶�У��
    if(F_DC == 1)
    {
        printf("CO_Cha=%f\r\n", CO_Cha);
        printf(" M/VУ��ǰ PcoR=%f\r\n", PcoR);
        printf("��ѹ���¶�У����Pco=%f\r\n", Pco);
    }
    Pco = Pco * (1 - CO2_CONC * 0.01) - CO2_CONC * 0.01 * R_XB / 10000 * CO_AD1_Z; //����CO2����Ũ��У��

    if(F_DC == 1)
    {
        printf("CO2Ũ��У����Pco=%f\r\n", Pco);
    }
    if(F_PC == 1)
    {
        FS_RUN_step3_OK();
    }

    /*�Ա�У��*/
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
            printf("����ϡ��У��Pco=%f\r\n", Pco);
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
            printf("Ů��ϡ��У��Pco=%f\r\n", Pco);
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
        Wr_80_Scr(0x03, temp, 2); //	����59���Ա�������
        goto LAGE1;

    }
    temp1[0] = (uint32_t)(Pco * 10000) >> 24;
    temp1[1] = (uint32_t)(Pco * 10000) >> 16;
    temp1[2] = (uint32_t)(Pco * 10000) >> 8;
    temp1[3] = (uint32_t)(Pco * 10000);
    Wr_82_Scr(0x000F, temp1, 4);

    RBC = (uint16_t)((1.38 * HB) / Pco) ; // �����RBC
    if(F_DC == 1)
    {
        printf("RBC=%d\r\n", RBC);
    }
    temp[0] = RBC >> 8;
    temp[1] = RBC;
    Wr_82_Scr(0x0011, temp, 2);

    /*�ж���ʾ���*/
    if(F_Zero_Error == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0082, temp, 2); //��ʾ*��
    }
    if(F_Zero_Num != 0)
    {
        temp[0] = 0x00;
        temp[1] = 0x02;
        Wr_82_Scr(0x0082, temp, 2); //��ʾ**��
    }
    if(F_CO2_Low == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0081, temp, 2); //��ʾ*��

    }
    if(F_CO2_Num != 0)
    {
        temp[0] = 0x00;
        temp[1] = 0x02;
        Wr_82_Scr(0x0081, temp, 2); //��ʾ**��
    }

    temp[0] = 0x00;
    temp[1] = 0x12;
    Wr_80_Scr(0x03, temp, 2); //	����18��׼��������07

    if(RBC < 251)
    {
        temp[0] = 0x00;
        temp[1] = 0x13;
        Wr_80_Scr(0x03, temp, 2); //	����19��׼��������08
    }
    else
    {
        temp[0] = 0x00;
        temp[1] = 0x47;
        Wr_80_Scr(0x03, temp, 2); //����71��׼�������250��
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
            SD_SAVE();   ///SD����������
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
            PC_ctr_Flag = 0; //PC���Ʊ�־��ʼ��
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            if(F_PC == 1)
            {
                FS_GasNo(0);
                Delay10ms(1);
            }
            PC_ctr_Flag = 0; //PC���Ʊ�־��ʼ��
            break;
        }

    }

    if((F_CO2_Num == 3) || (F_Zero_Num == 2)) ///���CO2��������3���쳣����zero�쳣2����ֱ�Ӿܾ���һ������
    {
        if(F_CO2_Num == 3)
        {
            if(F_Zero_Num == 2)
            {
                temp[0] = 0x00;
                temp[1] = 0x52;
                Wr_80_Scr(0x03, temp, 2); //��ת��82CO��CO2����ϵͳ���Ͻ���

            }
            else
            {
                temp[0] = 0x00;
                temp[1] = 0x51;
                Wr_80_Scr(0x03, temp, 2); //��ת��81CO2����ϵͳ���Ͻ���
            }

        }
        else
        {
            temp[0] = 0x00;
            temp[1] = 0x50;
            Wr_80_Scr(0x03, temp, 2); //��ת��80CO����ϵͳ���Ͻ���
        }
        while(1);
    }

    /*��������*/
    HB = 0; //HB����
    Zero_Num = 0;
    F_Zero_Error = 0;
    F_Timeout = 0; //��ʱ��������
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
    Wr_80_Scr(0x03, temp, 2); //����11��׼����1����
}
/**
  *@breif  ��ȷ����������
	*@param  none
	*@retval none
*/
void Adj_RUN_Program(void)
{
    uint8_t i;
    uint16_t CO2_AD, CO2_ZTest; // ʵ��CO2̨���� �Լ�����CO2���
    float CO2_CONC; // CO2Ũ��
    uint16_t CO_ZERO1;
    uint16_t CO_ZERO2;
    uint16_t CO_AD1;
    uint16_t CO_AD2;
    float CO_Cha[2];
    float CO_Cha_Ave;
    float CO_AD2_Zsub[2];
    float CO_AD1_Zsub[2];
    float PcoR, Pco; // COʵ��ֵ
    uint16_t RBC;
    uint8_t temp[2];
    uint8_t temp1[4];
    uint8_t m;
    uint16_t Diff_ZERO;
    uint16_t nnnn;

    /*����������CO2*/
    if(F_PC == 1)
    {
        FS_RUN_step1_ON();
    }
    temp[0] = 0x00;
    temp[1] = 0x16;
    Wr_80_Scr(0x03, temp, 2); //����22��ȷ����02����
    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity); //�����¶Ⱥ���ѹ
    temp[0] = (uint16_t)(BME_Temperature * 10) >> 8;
    temp[1] = (uint16_t)(BME_Temperature * 10);
    Wr_82_Scr(0x03, temp, 2);
    Purge_Cylinder();
    Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);// ��ϴCO2���Ҳ��������
    Paiqi_RR_PROM();// ��ϴCO2����10 S
    CO2_CONC = (float)(CO2_AD - CO2_ZTest) / CO2_Taijie;
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    if(F_DC == 1)
    {
        printf("��ѹУ��ϵ��=%d\r\n", ATM_Coeff);
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
        printf("CO2���ֵ=%d,CO2��ƽֵ=%d\r\n", CO2_ZTest, CO2_AD);
        printf("CO2Ũ��M/V=%f\r\n", CO2_CONC);
    }
    CO2_CONC = CO2_CONC * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / 301;
    if(F_DC == 1)
    {
        printf("CO2Ũ��V/V=%f\r\n", CO2_CONC);
    }
    temp[0] = (uint16_t)(CO2_CONC * 100) >> 8;
    temp[1] = (uint16_t)(CO2_CONC * 100);
    Wr_82_Scr(0x0022, temp, 2);

    if(CO2_CONC < 0.50) //ʵ��ֵС��0.5
    {
        temp[0] = 0x00;
        temp[1] = 0x3C;
        Wr_80_Scr(0x03, temp, 2); //����60CO2Ũ�ȹ��ʹ������
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
        F_CO2_Low = 1; //������̼���ͱ�־
        F_CO2_Num = 0;
    }
    else
    {
        F_CO2_Num = 0;
    }
    temp[0] = 0x00;
    temp[1] = 0x17;
    Wr_80_Scr(0x03, temp, 2);	//����23��ȷ����03����
    if(F_PC == 1)
    {
        FS_RUN_step1_OK();
    }

    /*��ʼCO����*/
    for(m = 0; m < 2; m++)
    {
        if(F_PC == 1)
        {
            FS_RUN_step2_ON();
        }

        temp[0] = 0x00;
        temp[1] = 0x18;
        Wr_80_Scr(0x03, temp, 2); //����24��ȷ����04����
        /*��������CǪ��*/
        Purge_Cylinder();
        Paiqi_R_PROM();//��ϴCO����100S
        ADC(&CO_ZERO1); //��B��CO���ZERO1
        for(i = 0; i < 2; i++)
        {
            Chou_BGas();// �ӻ����������黷����2��E5->E1
        }
        Daiji(30);// ����ʱ30S
        ADC(&CO_AD1); //  ����������B����̨��
        if(F_DC == 1)
        {
            printf(" ��%d�ε������=%d\r\n", m + 1, CO_ZERO1);
            printf(" ��%d�ε�����ƽ=%d\r\n", m + 1, CO_AD1);
        }
        if(F_PC == 1)
        {
            FS_RUN_step2_OK();
        }
        temp[0] = 0x00;
        temp[1] = 0x19;
        Wr_80_Scr(0x03, temp, 2); //����25��ȷ����05����

        /*����CǪ��*/
        if(F_PC == 1)
        {
            FS_RUN_step3_ON();
        }
        temp[0] = 0x00;
        temp[1] = 0x1A;
        Wr_80_Scr(0x03, temp, 2); //	����26��ȷ����06����

        Paiqi_R_PROM( );//��ϴCO����100S
        ADC(&CO_ZERO2); //��Y��CO���ZERO1
        if(F_DC == 1)
        {
            printf(" ��%d���������%d\r\n", m + 1, CO_ZERO2);
        }
        //�������ж�
        Diff_ZERO = abs(CO_ZERO2 - CO_ZERO1);
        if(Diff_ZERO > 10)
        {
            if(Diff_ZERO > 20)
            {
                temp[0] = Diff_ZERO >> 8;
                temp[1] = Diff_ZERO;
                Wr_82_Scr(0x0035, temp, 2); //�����д�뵽��������
                temp[0] = 0x00;
                temp[1] = 0x36;
                Wr_80_Scr(0x03, temp, 2); //	����54��㱨������
                if(F_PC == 1)
                {
                    FS_ZERO_ERRO();
                }
                goto ADJ_LAGE1;
            }
            else
            {
                F_Zero_Error = 1; //������
            }

        }

        for(i = 0; i < 2; i++)
        {
            Chou_YGas();// �������������黷����2��
        }

        Daiji(30);// ����ʱ30S
        ADC(&CO_AD2); // ����Y����̨��
        if(F_DC == 1)
        {
            printf("��%d��������ƽ=%d\r\n", m + 1, CO_AD2);
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
    PcoR = CO_Cha_Ave * R_XB / 10000.0; // ppm  ����ֵ��M/Vģʽ
    Pco = PcoR * ATM_Coeff * 0.0001 * (273 + BME_Temperature) / (273 + T0); //������ѹ���¶�У��
    if(F_DC == 1)
    {
        printf("CO_Cha=%f\r\n", CO_Cha_Ave);
        printf(" M/VУ��ǰ PcoR=%f\r\n", PcoR);
        printf("��ѹ���¶�У����Pco=%f\r\n", Pco);
    }
    Pco = Pco * (1 - CO2_CONC * 0.01) - CO2_CONC * 0.01 * R_XB / 10000 * ((CO_AD1_Zsub[0] + CO_AD1_Zsub[1]) / 2); //����CO2����Ũ��У��
    if(F_DC == 1)
    {
        printf("CO2Ũ��У����Pco=%f\r\n", Pco);
    }
    if(F_PC == 1)
    {
        FS_RUN_step3_OK();
    }

    /*ϡ��У�� ����Ů����*/
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
            printf("����ϡ��У��Pco=%f\r\n", Pco);
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
            printf("Ů��ϡ��У��Pco=%f\r\n", Pco);
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
        Wr_80_Scr(0x03, temp, 2); //	����59���Ա�������
        goto ADJ_LAGE1;

    }
    temp1[0] = (uint32_t)(Pco * 10000) >> 24;
    temp1[1] = (uint32_t)(Pco * 10000) >> 16;
    temp1[2] = (uint32_t)(Pco * 10000) >> 8;
    temp1[3] = (uint32_t)(Pco * 10000);
    Wr_82_Scr(0x000F, temp1, 4);
    // ���ݹ�ʽRBC=(HB*1.38)/PCO ����õ��������ˣ�RBC=120�죩
    RBC = (uint16_t)((1.38 * HB) / Pco) ; // �����RBC
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
        Wr_82_Scr(0x0082, temp, 2); //��ʾ*��
    }

    if(F_CO2_Low == 1)
    {
        temp[0] = 0x00;
        temp[1] = 0x01;
        Wr_82_Scr(0x0081, temp, 2); //��ʾ*��

    }
    if(F_CO2_Num != 0)
    {
        temp[0] = 0x00;
        temp[1] = 0x02;
        Wr_82_Scr(0x0081, temp, 2); //��ʾ**��
    }

    temp[0] = 0x00;
    temp[1] = 0x1B;
    Wr_80_Scr(0x03, temp, 2); //	����27��ȷ��������07

    if(RBC < 251)
    {
        temp[0] = 0x00;
        temp[1] = 0x1C;
        Wr_80_Scr(0x03, temp, 2); //	����28��ȷ��������08
    }
    else
    {
        temp[0] = 0x00;
        temp[1] = 0x46;
        Wr_80_Scr(0x03, temp, 2); //����70��ȷ��������������250
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
            SD_SAVE();   ///SD����������
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
            PC_ctr_Flag = 0; //PC���Ʊ�־��ʼ��
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            if(F_DC == 1)
            {
                FS_GasNo(0);
            }
            PC_ctr_Flag = 0; //PC���Ʊ�־��ʼ��
            break;
        }

    }
    if((F_CO2_Num == 3) || (F_Zero_Num == 2)) ///���CO2��������3���쳣
    {
        if(F_CO2_Num == 3)
        {
            if(F_Zero_Num == 2)
            {
                temp[0] = 0x00;
                temp[1] = 0x52;
                Wr_80_Scr(0x03, temp, 2); //��ת��82CO��CO2����ϵͳ���Ͻ���

            }
            else
            {
                temp[0] = 0x00;
                temp[1] = 0x51;
                Wr_80_Scr(0x03, temp, 2); //��ת��81CO2����ϵͳ���Ͻ���
            }

        }
        else
        {
            temp[0] = 0x00;
            temp[1] = 0x50;
            Wr_80_Scr(0x03, temp, 2); //��ת��80CO����ϵͳ���Ͻ���
        }
        while(1);
    }
    HB = 0; //HB����
    F_Timeout = 0; //��ʱ��������
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
    Wr_80_Scr(0x03, temp, 2); //����20��ȷ����00����

}
/**
  *@breif  �ʿ�����
	*@param  none
	*@retval none
*/
void QC_PROM(void)
{
    uint16_t QCCO2_AD, QC_ZTest ; // ��Ʒ���ĵ�ƽ��
    uint8_t i, n;
    uint8_t m = 0;
    uint8_t temp[2];
    uint8_t Zero_Num = 0; //���ִ���20�Ĵ���
    uint8_t sendNum = 0;
    uint16_t CO_ZERO1, CO_ZERO2, CO_AD1, CO_AD2; //CO_ZERO_V ��̬���
    uint16_t R_CO_Precision, R_CO_Accuracy; //���ս��ֵ
    uint16_t runTimeOut = 0;
    float CO_AD2_Z;
    float A_CO2_MV[3], A_CO2_VV[3], SUM_ACO2_VV; //CO2Ũ�ȣ���Ҫ��M/V��V/V����
    float PcoR[3], Pco[3], CO_Cha;
    float CO2_Precision, CO2_Accuracy; //CO2�����ľ��ܶȺ�׼ȷ��
    float Diff_ZERO;
    float Pco_Temp[3];//����ʢ��ɸѡ���Pcoֵ
    float CO_CONC_ACT;//COʵ�������ֵ*100

    while(!is_gasbag_exit(0));
    if(F_PC == 1)
    {
        Delay10ms(5);
        FS_GasYes(0);//���͵�һ���������뵽λ
        FS_GasYes(5);//����ȫ���嵽λ
        Delay10ms(1);
        if(F_PC == 0)
        {
            USART1_Timeout_UserCallback();
            PC_ctr_Flag = 0; //��λ��������λ����־����
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
                if(F_CO2 == 1) ///�յ���λ���������� 20190318
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
                        PC_ctr_Flag = 0; //��λ��������λ����־����
                        break;
                    }
                }
            }
            CO2_B = QC_CO2_CONC;
            CO_CONC_ACT = QC_CONC;
        }
    }

    /*��ʼ�ʿ�*/
    temp[0] = 0x00;
    temp[1] = 0x26;
    Wr_80_Scr(0x03, temp, 2); //����38�ʿ�01����
    Delay1ms(1);
    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2);
    temp[0] = Ring_Num >> 8;
    temp[1] = Ring_Num;
    Wr_82_Scr(0x0009, temp, 2);
    if(PC_ctr_Flag == 0)
    {
        /*��ȡ����ֵ*/
        Rd_83_Scr(0x0013, 1); //��ȡ��׼��COŨ��
        Delay1ms(5);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x13) && (Com3RBuf[6] == 0x01))
        {
            Cal_CO_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            if(F_DC == 1)
            {
                printf("������COŨ�Ȳ�ֵ=%f\r\n", Cal_CO_A * 0.0001);
            }
        }
        CO_CONC_ACT = Cal_CO_A * 0.01;

        Rd_83_Scr(0x0012, 1); //��ȡ��׼��CO2Ũ��
        Delay1ms(5);
        if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
        {
            CO2_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
            if(F_DC == 1)
            {
                printf("������ACO2���ֵ%d\r\n", CO2_B);
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
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
        printf("��ѹУ��ϵ��=%d\r\n", ATM_Coeff);
    }
    if(F_PC == 1)
    {
        Delay1ms(5);
        FS_QC_ON();
    }
    DaoQi_Z(150);//ͨ����Ĥ�òɼ���������������

    for(i = 0; i < 3; i++)
    {
        temp[0] = 0x00;
        temp[1] = 0x27;
        Wr_80_Scr(0x03, temp, 2); //����39�ʿ�02����
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
        Purge_Cylinder();//��ϴ����

        /*CO2����*/
        Celiang_ZhunBeiNew(&QC_ZTest, &QCCO2_AD);// ��ϴCO2���Ҳ�����CO2��ƽ
        if(F_DC == 1)
        {
            printf("��%d��CO2�������=%d,��ƽ=%d\r\n", i + 1, QC_ZTest, QCCO2_AD);
        }
        A_CO2_MV[i] = (float)(QCCO2_AD - QC_ZTest) / CO2_Taijie;
        if(A_CO2_MV[i] <= 0.0001)
        {
            A_CO2_MV[i] = 0.0;
        }
        if(F_DC == 1)
        {
            printf("��%d��CO2_MV����Ũ��=%f\r\n", i + 1, A_CO2_MV[i]);
        }
        A_CO2_VV[i] = ATM_Coeff * 0.0001 * A_CO2_MV[i] * (BME_Temperature + 273) / (T0 + 273);
        if(F_DC == 1)
        {
            printf("��%d��CO2_VVŨ��=%f\r\n", i + 1, A_CO2_VV[i]);
        }
        SUM_ACO2_VV += A_CO2_VV[i];

        /*CO����*/
        temp[0] = 0x00;
        temp[1] = 0x28;
        Wr_80_Scr(0x03, temp, 2); //����40�ʿ�03����
        Purge_Cylinder();//��ϴ����
        Paiqi_R_PROM();//��ϴCO����100S
        ADC(&CO_ZERO1); //��B��CO���
        if(F_DC == 1)
        {
            printf("��%d�ε������ֵ=%d\r\n", i + 1, CO_ZERO1);
        }
        for(n = 0; n < 2; n++)
        {
            Chou_BGas();
        }
        Daiji(30);
        ADC(&CO_AD1);//������������̨��
        if(F_DC == 1)
        {
            printf("��%d�ε�����ƽֵ=%d\r\n", m + 1, CO_AD1);
        }

        Paiqi_R_PROM();//��ϴ����100s
        ADC(&CO_ZERO2); //Y��CO���
        if(F_DC == 1)
        {
            printf("��%d���������ֵ=%d\r\n", i + 1, CO_ZERO2);
        }

        Diff_ZERO = abs(CO_ZERO2 - CO_ZERO1);//�������ж�
        if(Diff_ZERO > 10)
        {
            if(Diff_ZERO > 20)
            {
                if(Zero_Num == 2)
                {
                    temp[0] = (uint16_t)Diff_ZERO >> 8;
                    temp[1] = (uint16_t)Diff_ZERO;
                    Wr_82_Scr(0x0035, temp, 2); //�����д�뵽��������
                    temp[0] = 0x00;
                    temp[1] = 0x45;
                    Wr_80_Scr(0x03, temp, 2); //	����69��㱨������
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
                F_Zero_Error = 1; //������
            }

        }
        temp[0] = 0x00;
        temp[1] = 0x29;
        Wr_80_Scr(0x03, temp, 2); //����41�ʿ�04����

        for(n = 0; n < 2; n++)
        {
            Chou_YGas();// �ӷ�����2��ȡ����2��
        }
        Daiji(30);// ����ʱ30S
        ADC(&CO_AD2); // ����Y����̨��
        if(F_DC == 1)
        {
            printf(" ��%d��������ƽֵ=%d\r\n", i + 1, CO_AD2);
        }

        CO_AD2_Z = CO_AD2 - CO_ZERO2;
        if(CO_AD2_Z <= 0.00001)
        {
            CO_AD2_Z = 0.0;
        }

        CO_Cha = CO_AD2 - CO_AD1;
        if(F_DC == 1)
        {
            printf("��%d�ε�ƽ��=%f\r\n", i + 1, CO_Cha);
        }

        PcoR[i] = (float)R_XB / 10000 * CO_Cha; // ppm


        Pco[i] = PcoR[i] * ATM_Coeff * 0.0001 * (BME_Temperature + 273) / (T0 + 273) * (1 - A_CO2_VV[i] / 100); // ϡ��У��Pco
        if(F_DC == 1)
        {
            printf("��%d��У��ǰpcoR=%f\r\n", i + 1, PcoR[i]);
            printf("��%d��У���� Pco=%f\r\n", i + 1, Pco[i]);
        }
        if(Pco[i] <= 0.0001)
        {
            temp[0] = 0x00;
            temp[1] = 0x3B;
            Wr_80_Scr(0x03, temp, 2); //	����59���Ա�������
            goto QC_LAGE1;
        }
QC_LAGE2:
        if(i < 2)
        {
            Paiqi_RRR_PROM();
            Wr_80_Scr(0x02, &time, 1); //��������
            temp[0] = 0x00;
            temp[1] = 0x4D;
            Wr_80_Scr(0x03, temp, 2); //����77����������ʾ����
            while(is_gasbag_exit(0));
            Delay100ms(10);//��ʱ��ֹ�δ��ӹ����е�ƽ����
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
    CO2_Accuracy = 100 * StandardDeviation(A_CO2_VV, 3, 0, CO2_B); //׼ȷ�ȼ���  �����С��ֵ*10  ��������1.0%������ֵ�����10
    CO2_Precision = 100 * StandardDeviation(A_CO2_VV, 3, 1, CO2_B); //���ܶȼ���
    if(F_DC == 1)
    {
        printf("CO2׼ȷ��Ϊ��%f\r\n", CO2_Accuracy);
        printf("CO2���ܶ�Ϊ��%f\r\n", CO2_Precision);
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
        Wr_80_Scr(0x03, temp, 2); //����67CO2���ܶȺ�׼ȷ��δ������
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
    R_CO_Precision = (uint16_t)(100 * StandardDeviation(Pco_Temp, m, 1, CO_CONC_ACT)); //���ܶȼ��㣬����ֵ��λ��%
    R_CO_Accuracy = (uint16_t)(100 * StandardDeviation(Pco_Temp, m, 0, CO_CONC_ACT)); //׼ȷ�ȼ���
    if(F_DC == 1)
    {
        printf("CO���ܶ�*100=%d\r\n", R_CO_Precision);
        printf("CO׼ȷ��*100=%d\r\n", R_CO_Accuracy);
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
        Wr_80_Scr(0x03, temp, 2); //����43�ʿ�ͨ������
        if(F_Zero_Error == 1)
        {
            temp[0] = 0x00;
            temp[1] = 0x01;
            Wr_82_Scr(0x0080, temp, 2);
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
        temp[0] = 0x00;
        temp[1] = 0x2A;
        Wr_80_Scr(0x03, temp, 2); //����42�ʿ�δͨ������
        if(F_PC == 1)
        {
            FS_RA(&R_CO_Precision);
            Delay10ms(1);
            FS_RP(&R_CO_Accuracy);
            Delay10ms(1);
            FS_QC_RS_ERR();//��ʾ�ʿ�δͨ��
        }
    }
QC_LAGE1:
    if(F_PC == 1)
    {
        Delay10ms(10);
        FS_QC_OK();
    }
    Paiqi_RRR_PROM();
    Wr_80_Scr(0x02, &time, 1); //��������

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
            Wr_80_Scr(0x03, temp, 2); //����37�ʿ�00����
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
    PC_ctr_Flag = 0; //PC���Ʊ�־��ʼ��
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
  *@breif  CO�궨����
	*@param  none
	*@retval none
*/
void Curve_PROM(void)
{
    uint16_t CO_ZERO1, CO_ZERO2, CO_AD2, CO_ZERO_DIF, CO_AD1;
    uint16_t Cal_CO2_A;
    uint16_t Curve_CONC_MV;//�궨CO��M/VŨ��
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
    Wr_80_Scr(0x03, temp, 2); //����35CO�궨01����
    Delay1ms(5);

    Rd_83_Scr(0x0013, 1); //��ȡ������COŨ�Ȳ�ֵ
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x13) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        if(F_DC == 1)
        {
            printf("������COŨ�Ȳ�ֵ_V/V=%f\r\n", Cal_CO_A * 0.0001);
        }
    }

    Rd_83_Scr(0x0012, 1); //��ȡ��׼��CO2���ֵ
    Delay1ms(1);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
    {
        Cal_CO2_A = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        if(F_DC == 1)
        {
            printf("������A CO2���ֵ=%f\r\n", Cal_CO2_A * 0.01);
        }
    }

    Curve_CONC = Cal_CO_A * 0.01; //������100��

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
    ATM_Coeff = (uint16_t)(10000 * 1013.25 / BME_Pressure);
    if(F_DC == 1)
    {
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
        printf("��ѹ����ϵ��=%d\r\n", ATM_Coeff);
    }

    DaoQi_Z(150);//ͨ����Ĥ�òɼ���������������

    /*��ʼ����*/
    for(i = 0; i < 3; i++)
    {
        temp[0] = 0x00;
        temp[1] = 0x23;
        Wr_80_Scr(0x03, temp, 2); //����35CO�궨01����
        Purge_Cylinder();//��ϴ����

        /*CO2����*/
        Celiang_ZhunBeiNew(&CO2_ZTest, &CO2_AD);// ��ϴCO2���Ҳ�����CO2��ƽ
        if(F_DC == 1)
        {
            printf("��%d��CO2�������=%d,��ƽ=%d\r\n", i + 1, CO2_ZTest, CO2_AD);
        }
        CO2_CONC_MV[i] = (float)(CO2_AD - CO2_ZTest) / CO2_Taijie;
        if(CO2_CONC_MV[i] <= 0.0001)
        {
            CO2_CONC_MV[i] = 0.0;
        }
        if(F_DC == 1)
        {
            printf("��%d��CO2_MV����Ũ��=%f\r\n", i + 1, CO2_CONC_MV[i]);
        }
        CO2_CONC_MV_SUM += CO2_CONC_MV[i];
        CO2_CONC_VV[i] = ATM_Coeff * 0.0001 * CO2_CONC_MV[i] * (BME_Temperature + 273) / (T0 + 273);
        if(F_DC == 1)
        {
            printf("��%d��CO2_VVŨ��=%f\r\n", i + 1, CO2_CONC_VV[i]);
        }
        CO2_CONC_VV_SUM += CO2_CONC_VV[i];

        /*����������*/
        Purge_Cylinder();//��ϴ����
        Paiqi_R_PROM();//  100S������ϴ����
        ADC(&CO_ZERO1); // ����B�������
        if(F_DC == 1)
        {
            printf("��%d�ε������=%d\r\n", i + 1, CO_ZERO1);
        }
        for(m = 0; m < 2; m++)
        {
            Chou_BGas();// �ӻ����������黷����2��E6->E1
        }
        Daiji(30);// ����ʱ30S
        ADC(&CO_AD1); //  ����������B����̨��
        if(F_DC == 1)
        {
            printf("��%d�ε�����ƽֵ=%d\r\n", i + 1, CO_AD1);
        }

        /*��������*/
        Paiqi_R_PROM();//��ϴCO����100S
        ADC(&CO_ZERO2); // ����Y����̨��
        if(F_DC == 1)
        {
            printf("��%d���������=%d\r\n", i + 1, CO_ZERO2);
        }
        CO_ZERO_DIF = abs(CO_ZERO2 - CO_ZERO1);//�������ж�
        if(CO_ZERO_DIF > 10)
        {
            temp[0] = CO_ZERO_DIF >> 8;
            temp[1] = CO_ZERO_DIF;
            Wr_82_Scr(0x0002, temp, 2);
            temp[0] = 0x00;
            temp[1] = 0x37;
            Wr_80_Scr(0x03, temp, 2); //����55����
            goto LAGE_CURVE;
        }
        for(m = 0; m < 2; m++)
        {
            Chou_YGas();// ������ȡ2��
        }
        Daiji(30);//����30S
        ADC(&CO_AD2);
        CO_cha[i] = CO_AD2 - CO_AD1;
        if(F_DC == 1)
        {
            printf("��%d��������ƽ=%d\r\n", i + 1, CO_AD2);
            printf("��%d��������ƽ��=%f\r\n", i + 1, CO_cha[i]);
        }
        Sum_CO_cha += CO_cha[i];

        if(i < 2)
        {
            Paiqi_RRR_PROM();
            Wr_80_Scr(0x02, &time, 1); //��������
            temp[0] = 0x00;
            temp[1] = 0x4D;
            Wr_80_Scr(0x03, temp, 2); //����77����������ʾ����
            while(is_gasbag_exit(0));
            Delay100ms(2);//��ʱ��ֹ�δ��ӹ����е�ƽ����
            while(!is_gasbag_exit(0));
        }

    }

    /*ͳ�Ʋ�������*/
    CO2_Accuracy = 1000 * StandardDeviation(CO2_CONC_VV, 3, 0, Cal_CO2_A); //׼ȷ�ȼ���
    if(F_DC == 1)
    {
        printf("CO2׼ȷ��Ϊ=%f\r\n", CO2_Accuracy * 0.1);
    }
    if(CO2_Accuracy > 40)
    {
        temp[0] = (uint16_t)CO2_Accuracy >> 8;
        temp[1] = (uint16_t)CO2_Accuracy;
        Wr_82_Scr(0x0033, temp, 2);
        Wr_82_Scr(0x0034, temp, 2);
        temp[0] = 0x00;
        temp[1] = 0x42;
        Wr_80_Scr(0x03, temp, 2); //����66�궨δ��ɽ���
        goto LAGE_CURVE;
    }

    DCO_Precision = 1000 * StandardDeviation(CO_cha, 3, 1, 1); //���ܶȼ���
    if(F_DC == 1)
    {
        printf("CO��ƽ���ܶ�=%f\r\n", DCO_Precision * 0.1);
    }
    if(DCO_Precision > 100)
    {
        temp[0] = (uint16_t)DCO_Precision >> 8;
        temp[1] = (uint16_t)DCO_Precision;
        Wr_82_Scr(0x0036, temp, 2);
        temp[0] = 0x00;
        temp[1] = 0x44;
        Wr_80_Scr(0x03, temp, 2); //����68�궨δ��ɽ���
        goto LAGE_CURVE;
    }
    Curve_CONC_MV = Curve_CONC * 10000.0 / ATM_Coeff * (273 + T0) / (273 + BME_Temperature);    //��MV��ʽ��COŨ��
    Curve_CONC_MV = Curve_CONC_MV / (1 - CO2_CONC_VV_SUM / 300);

    R_XB = 10000 * Curve_CONC_MV * 0.03 / Sum_CO_cha;
    if(F_DC == 1)
    {

        printf("M/V��ʽCO��׼��Ũ�Ȳ�=%f\r\n", Curve_CONC_MV * 0.01);
        printf("����У��M/V��ʽCO��׼��Ũ�Ȳ�=%f\r\n", Curve_CONC_MV * 0.01);
        printf("CO�궨ϵ��=%d\r\n", R_XB);
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
    Wr_80_Scr(0x02, &time, 1); //��������
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
  *@breif  ��������
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
    Wr_80_Scr(0x03, temp, 2); //����48����01����
    Ring_Num++;
    ee_WriteBytes((uint8_t *)&Ring_Num, 10, 2); //20180228-lcy
    Rd_83_Scr(0x001E, 1); //��ȡ������������
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x1E) && (Com3RBuf[6] == 0x01))
    {
        BGas_Num = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
    }
    Rd_83_Scr(0x001F, 1); //��ȡ������������
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
    Wr_80_Scr(0x03, temp, 2); //����65����02����
    Wr_80_Scr(0x02, &time, 1); //��������
    while(1)
    {
        if(is_gasbag_exit(0) == 0) //�ε�����������47����00����
        {
            temp[0] = 0x00;
            temp[1] = 0x2F;
            Wr_82_Scr(0x001E, temp1, 2);
            Wr_82_Scr(0x001F, temp1, 2);
            Wr_80_Scr(0x03, temp, 2);
            break;
        }
        if(BtnPressedFlag == 0xAA) //������ҳ�������ش�������
        {
            Wr_82_Scr(0x001E, temp1, 2);
            Wr_82_Scr(0x001F, temp1, 2);
            break;
        }
    }

}

/**
  *@breif  CO2�궨����
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

    QiGuang_Reset();//���׸�λ

    BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);//��ȡ��ѹ���¶Ⱥ�ʪ��ֵ

    if(F_DC == 1)
    {
        printf("��ѹ=%f����\r\n", BME_Pressure);
        printf("�¶�=%f���϶�\r\n", BME_Temperature);
    }

    for(i = 0; i < 3; i++)
    {
        /*��������*/
        Purge_Cylinder();//������ϴ
        Open_CO2_Paiqi();
        Daiji(60);// 60S��ϴCO2����
        Close_CO2_Paiqi();
        Daiji(30);
        ADC2_ValConvt(&CO2_ZERO_Sub);//��������ƽ
        if(F_DC == 1)
        {
            printf("��%d��CO2���ֵ=%d\r\n", i + 1, CO2_ZERO_Sub);
        }
        CO2_ZERO_Sum += CO2_ZERO_Sub;

        /*��������*/
        Get_CO2_GAS();//��ȡ����
        Daiji(30);
        ADC2_ValConvt(&CO2_ADVal);
        if(F_DC == 1)
        {
            printf("��%d��CO2��ƽֵ=%d\r\n", i + 1, CO2_ADVal);
        }
        CO2_ADVal_Sum += CO2_ADVal;
        Paiqi_RR_PROM();//��ϴCO2������10s
    }
    CO2_ZERO = CO2_ZERO_Sum / 3;  //�����ƽ��ֵ
    temp[0] = CO2_ZERO >> 8;
    temp[1] = CO2_ZERO;
    Wr_82_Scr(0x0007, temp, 2);
    ee_WriteBytes((uint8_t *)&CO2_ZERO, 8, 2);

    if(F_DC == 1)
    {
        printf("���ƽ��ֵ=%d\r\n", CO2_ZERO);
    }

    CO2_B *= BME_Pressure / 1013.25 * (T0 + 273) / (BME_Temperature + 273);; //�Ա�׼����V/Vת��Ϊm/v
    if(F_DC == 1)
    {
        printf("����У����궨������Ũ��(*100):%d\r\n", CO2_B);
    }
    CO2_Taijie = (uint16_t)(CO2_ADVal_Sum - CO2_ZERO_Sum) / (CO2_B * 0.03); //��CO2�궨ϵ��

    if(F_DC == 1)
    {
        printf("CO2�궨ϵ��=%d\r\n", CO2_Taijie);
    }
    ee_WriteBytes((uint8_t *)&CO2_Taijie, 6, 2);
    temp[0] = CO2_Taijie >> 8;
    temp[1] = CO2_Taijie;
    Wr_82_Scr(0x0006, temp, 2);

    temp[0] = 0x00;
    temp[1] = 0x21;
    Wr_80_Scr(0x03, temp, 2); //����33CO2�궨03����
    Open_CO2_Paiqi();
    Daiji(60); // 60S��ϴCO2����
    Close_CO2_Paiqi();
}
/**
  *@breif  CO2�궨������
	*@param  none
	*@retval none
  */
void CO2_CON(void)
{
    uint8_t temp[2];
    uint8_t temp1[2] = {0, 0};

    while(!is_gasbag_exit(0));//����״̬�ж�

    temp[0] = 0x00;
    temp[1] = 0x1F;
    Wr_80_Scr(0x03, temp, 2); //����31CO2�궨01����
    Delay1ms(5);

    Rd_83_Scr(0x0012, 1); //��ȡ��׼��CO2Ũ��
    Delay1ms(5);
    if((Com3RBuf[2] == 0x06) && (Com3RBuf[3] == 0x83) && (Com3RBuf[4] == 0x00) && (Com3RBuf[5] == 0x12) && (Com3RBuf[6] == 0x01))
    {
        CO2_B = (uint16_t)Com3RBuf[7] << 8 | Com3RBuf[8];
        if(F_DC == 1)
        {
            printf("CO2��׼��Ũ��_V/V��%d\r\n", CO2_B);
        }
    }

    CO2_Zero();//���в���
    Wr_80_Scr(0x02, &time, 1); //��������
    while(1)
    {
        if(is_gasbag_exit(0) == 0)
        {
            temp[0] = 0x00;
            temp[1] = 0x1E;
            Wr_82_Scr(0x0012, temp1, 2); //��׼��Ũ������
            Wr_80_Scr(0x03, temp, 2); //����30CO2�궨00����
            break;
        }
        if(BtnPressedFlag == 0xAA)
        {
            temp[0] = 0x00;
            temp[1] = 0x1E;
            Wr_82_Scr(0x0012, temp1, 2); //��׼��Ũ������
            break;
        }
    }
}

/*
���ܶȺ�׼ȷ��
P[]�Ƕ�β��ԵĽ��
num�ǲ������ݵĴ���
select�Ǿ��ܶȺ�׼ȷ�ȵ�ѡ��Ϊ1ʱ���о��ܶȼ��㣬Ϊ0ʱ����׼ȷ�ȼ���
stand  ��ʵ����ֵ����100
20180818
*/

float StandardDeviation(float * P, uint8_t num, uint8_t select, uint16_t Stand)
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
//    printf("��ֵ=%f\r\n", avg);
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
//        printf("���ܶ�R_SD=%f\r\n", R_SD);
    }
    else
    {
        R_SD = stdd / (Stand * 0.01);
//        printf("׼ȷ��R_SD=%f\r\n", R_SD);
    }
    return R_SD;

}
