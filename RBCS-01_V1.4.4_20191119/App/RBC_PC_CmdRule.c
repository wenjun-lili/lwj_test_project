#include "RBC_PC_CmdRule.h"
#include "globalval.h"
#include "string.h"
#include "utility.h"
#include "./usart/bsp_usart.h"


void PC_Ack(void)
{
    uint16_t Timeout_num;
    Timeout_num=USART1Timeout;//��ʱ

ABC:
    RBCtoPC_Cmd_AA();
// RBCtoPC_Cmd_AA();//bert
    Delay1ms(10); //10ms
    while(!ReceiveFlag)// ���յ���0xBB
    {
        /*
        �ȴ���ʱ�ж�
        20180705
        */
        Delay1ms(1);
        Timeout_num--;
        if(Timeout_num==0)
        {
            F_Timeout=1;
            USART1_Timeout_UserCallback();//��ʱ����
            break;
        }
    }
    ReceiveFlag=0;
    if((F_Timeout==0)&&(RBC_pc_ComR[0]!=0xBB))
    {
        RBC_pc_ComR[0]=0;
        goto ABC;
    }
    else
    {
        RBC_pc_ComR[0]=0;// ����
    }

}


void PC_Cmd_Analy(void)
{
    if((RBC_pc_ComR[0]==0x20)&&(RBC_pc_ComR[1]==0x00)&&(RBC_pc_ComR[2]==0x00)&&(RBC_pc_ComR[3]==0x00)&&(RBC_pc_ComR[4]==0x00)&&(RBC_pc_ComR[5]==0x20))
    {
        RBCtoPC_Cmd_00();
        Delay10ms(10);
        F_PC_Ctrl=1;
        BtnPressedFlag=0;//�������ֵ

    }
    else if((RBC_pc_ComR[0]==0x20)&&(RBC_pc_ComR[1]==0x01)&&(RBC_pc_ComR[2]==0x00)&&(RBC_pc_ComR[3]==0x00)&&(RBC_pc_ComR[4]==0x00)&&(RBC_pc_ComR[5]==0x21))
    {
        RBCtoPC_Cmd_00();
        Delay10ms(10);
        F_PC_Ctrl=1;
        BtnPressedFlag=2;//�ʿؼ���ֵ
    }
    else
    {
        RBCtoPC_Cmd_FF();
        Delay10ms(5);
        F_PC_Ctrl=0;
    }
}

void PC_Val_Analy(void)
{
    uint8_t i;
    if(RBC_pc_ComR[0]==0xE0)
    {
        if(RBC_pc_ComR[5]==(uint8_t)(RBC_pc_ComR[0]+RBC_pc_ComR[1]+RBC_pc_ComR[2]+RBC_pc_ComR[3]+RBC_pc_ComR[4]))
        {
            switch(RBC_pc_ComR[1])
            {
            case 0x00:
                HB=256*RBC_pc_ComR[3]+RBC_pc_ComR[4];
                break;
            case 0x02:
                Gender=RBC_pc_ComR[4];
                F_Sex=1;
                break;
            case 0x03:
                QC_CONC=256*RBC_pc_ComR[3]+RBC_pc_ComR[4];
                break;
            case 0x04:
                QC_CO2_CONC=256*RBC_pc_ComR[3]+RBC_pc_ComR[4];
                F_CO2=1;
                break;
						case 0x05:
						    F_GasType=RBC_pc_ComR[4];
								break;
            default:
                break;
            }
            for(i=0; i<6; i++) //������
            {
                RBC_pc_ComR[i]=0;

            }
            RBCtoPC_Cmd_00();
        }
        else
        {
            RBCtoPC_Cmd_FF();
        }
    }
    else
    {
        RBCtoPC_Cmd_FF();
    }


}


/*��ϸ�������ⶨ�Ƿ���PC���������
����֡��ʽ: һ���ֽ������࣬һ���ֽ������࣬3���ֽ����ݣ� һ���ֽ�У���
*/
void wr_cmd_array_6(uint8_t cmd,uint8_t data_class,uint8_t data1,uint8_t data23)
{
    uint8_t i;
    uint8_t check = 0;
    RBC_pc_ComT[0] = cmd;
    RBC_pc_ComT[1] = data_class;
    RBC_pc_ComT[2]=data1;

    for (i = 3; i < 5; i++)
    {
        RBC_pc_ComT[i] = data23;

    }
    for (i = 0; i < 5; i++)
    {
        check+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5] = check;
    return;

}

/*������PC����������*/
// 6�ֽ�ָ��
void RBCtoPC_Cmd_6(void)
{
    uint8_t i;
    uint16_t Timeout_num1;
//	  uint8_t sendDataNum=0;//�ظ�����ָ�����

//    PC_Ack(); // �յ������ź�֮��ʼ����

CF:
    Timeout_num1=USART1Timeout;
    if(F_Timeout==0)
    {
        for(i=0; i<6; i++)
        {
            //		printf("%x", c13_pc_command[i]);
            USART_SendData(USART1, (uint8_t) RBC_pc_ComT[i]);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

        }
        Delay1ms(5); //5ms
        while(!ReceiveFlag)
        {
            /*
            �ȴ���ʱ�ж�
            20180705
            */
            Delay1ms(1);
            if((Timeout_num1--)==0)
            {
//							  if(sendDataNum<2)//���δ���������ٷ���
//								{
//								  sendDataNum++;
//	                goto CF;
//								}
//								else
//								{
                F_Timeout=1;
                USART1_Timeout_UserCallback();//��ʱ����
//				printf("timeout");
                break;
//								}
            }
        }
        ReceiveFlag=0;
        if((RBC_pc_ComR[0]!=0x00)&&(F_Timeout==0))
        {
//					if(m<2)
//					{
//						m++;
            RBC_pc_ComR[0]=0;
            goto CF;
//					}
//					else
//					{
//					  USART1_Timeout_UserCallback();//��ʱ����
//					}
        }
        RBC_pc_ComR[0]=0;
    }
    Delay1ms(1);
    return;

}
/**
  *@brief  RBCtoPC_Cmd_61
  *@datail �������ֽ����ݣ������޷������ݲ��Ͽ�ͨѶ
  *@param  void
  *@retval void
  */
void RBCtoPC_Cmd_61(void)
{
    uint8_t i;
    uint16_t Timeout_num1;
    Timeout_num1=USART1Timeout;

//    PC_Ack(); // �յ������ź�֮��ʼ����
    if(F_Timeout==0)
    {
        for(i=0; i<6; i++)
        {
            //		printf("%x", c13_pc_command[i]);
            USART_SendData(USART1, (uint8_t) RBC_pc_ComT[i]);
            while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

        }
        Delay1ms(20); //20ms
        while(!ReceiveFlag)
        {
            /*
            �ȴ���ʱ�ж�
            20180705
            */
            Delay1ms(1);
            if((Timeout_num1--)==0)
            {
                break;
            }
        }
        ReceiveFlag=0;
        RBC_pc_ComR[0]=0;
    }
    Delay10ms(1);
    return;
}
// 1�ֽ�ָ��
void RBCtoPC_Cmd_AA(void)
{
    USART_SendData(USART1, (uint8_t)0xAA);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC)==RESET);
    return ;
}

void RBCtoPC_Cmd_00(void)
{
    USART_SendData(USART1, 0x00);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

    return ;
}
void RBCtoPC_Cmd_FF(void)
{
    USART_SendData(USART1, 0xFF);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    return ;
}


void  FS_GasYes(uint8_t i)
{
    if(F_Timeout==0)
    {
        switch(i)
        {
        case 0:
            wr_cmd_array_6(0x80, 0x00, 0x01,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 1:
            wr_cmd_array_6(0x80, 0x01, 0x01,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 2:
            wr_cmd_array_6(0x80, 0x02, 0x01,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 3:
            wr_cmd_array_6(0x80, 0x03, 0x01,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 4:
            wr_cmd_array_6(0x80, 0x04, 0x00,0x00);//ֻ��Բ�������
            RBCtoPC_Cmd_6();
            break;
        case 5:
            wr_cmd_array_6(0x80, 0x04, 0x02,0x00);//ֻ����ʿ�����
            RBCtoPC_Cmd_6();
            break;
        default:
            break;
        }
    }
}

void  FS_GasNo(uint8_t i)
{
    if(F_Timeout==0)
    {
        switch(i)
        {
        case 0:
            wr_cmd_array_6(0x80, 0x00, 0x00,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 1:
            wr_cmd_array_6(0x80, 0x01, 0x00,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 2:
            wr_cmd_array_6(0x80, 0x02, 0x00,0x00);
            RBCtoPC_Cmd_6();
            break;
        case 3:
            wr_cmd_array_6(0x80, 0x03, 0x00,0x00);
            RBCtoPC_Cmd_6();
            break;

        default:
            break;
        }
    }
}

/*
����ͨѶ�Ͽ�����
*/
void  FS_GasYes_None(uint8_t i)
{
    if(F_Timeout==0)
    {
        switch(i)
        {
        case 0:
            wr_cmd_array_6(0x80, 0x00, 0x01,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 1:
            wr_cmd_array_6(0x80, 0x01, 0x01,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 2:
            wr_cmd_array_6(0x80, 0x02, 0x01,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 3:
            wr_cmd_array_6(0x80, 0x03, 0x01,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 4:
            wr_cmd_array_6(0x80, 0x04, 0x00,0x00);
            RBCtoPC_Cmd_61();
        default:
            break;
        }
    }
}
/*
����ͨѶ�Ͽ�����
*/
void  FS_GasNo_None(uint8_t i)
{
    if(F_Timeout==0)
    {
        switch(i)
        {
        case 0:
            wr_cmd_array_6(0x80, 0x00, 0x00,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 1:
            wr_cmd_array_6(0x80, 0x01, 0x00,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 2:
            wr_cmd_array_6(0x80, 0x02, 0x00,0x00);
            RBCtoPC_Cmd_61();
            break;
        case 3:
            wr_cmd_array_6(0x80, 0x03, 0x00,0x00);
            RBCtoPC_Cmd_61();
            break;

        default:
            break;
        }
    }
}
// ��ʼ��������
void FS_INT_ON(void)
{
    wr_cmd_array_6(0x90, 0x00, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;

}
// ��ʼ�����
void FS_INT_OK(void)
{
    wr_cmd_array_6(0x90, 0x00, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
// ��ʼ������
void FS_INT_ERR(void)
{
    wr_cmd_array_6(0x90, 0x00, 0x02,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}

void FS_READY(void)
{
    wr_cmd_array_6(0xD0, 0x00, 0x00,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
void FS_ZERO_ERRO(void)
{
    wr_cmd_array_6(0xD0, 0x01, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_TEST_ERRO(void)
{
    wr_cmd_array_6(0xD0, 0x02, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}

void FS_CO2_ERRO(void)
{
    wr_cmd_array_6(0xD0, 0x03, 0x00,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}


void FS_RUN_ON(void)// ������
{
    wr_cmd_array_6(0xA0, 0x00, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}

void FS_RUN_OK(void)// ���
{
    wr_cmd_array_6(0xA0, 0x00, 0x01,0x00);
    RBCtoPC_Cmd_61();
}

void FS_RUN_ERR(void)
{
    wr_cmd_array_6(0xA0, 0x00, 0x02,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}

void FS_RUN_step1_ON(void)//������
{
    wr_cmd_array_6(0xA0, 0x01, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step1_OK(void)// ���
{
    wr_cmd_array_6(0xA0, 0x01, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step1_ERR(void)//ERRO
{
    wr_cmd_array_6(0xA0, 0x01, 0x02,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}

void FS_RUN_step2_ON(void)//������
{
    wr_cmd_array_6(0xA0, 0x02, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step2_OK(void)// ���
{
    wr_cmd_array_6(0xA0, 0x02, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step2_ERR(void)//ERRO
{
    wr_cmd_array_6(0xA0, 0x02, 0x02,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;

}
void FS_RUN_step3_ON(void)//������
{
    wr_cmd_array_6(0xA0, 0x03, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step3_OK(void)// ���
{
    wr_cmd_array_6(0xA0, 0x03, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step3_ERR(void)//ERRO
{
    wr_cmd_array_6(0xA0, 0x03, 0x02,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}

void FS_RUN_step4_ON(void)//������
{
    wr_cmd_array_6(0xA0, 0x04, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step4_OK(void)// ���
{
    wr_cmd_array_6(0xA0, 0x04, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_RUN_step4_ERR(void)//ERRO
{
    wr_cmd_array_6(0xA0, 0x04, 0x02,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_ON(void)
{
    wr_cmd_array_6(0xB0, 0x00, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_OK(void)
{
    wr_cmd_array_6(0xB0, 0x00, 0x01,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
void FS_QC_step1_ON(void)
{
    wr_cmd_array_6(0xB0, 0x01, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_step1_OK(void)
{
    wr_cmd_array_6(0xB0, 0x01, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_step2_ON(void)
{
    wr_cmd_array_6(0xB0, 0x02, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_step2_OK(void)
{
    wr_cmd_array_6(0xB0, 0x02, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_step3_ON(void)
{
    wr_cmd_array_6(0xB0, 0x03, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_step3_OK(void)
{
    wr_cmd_array_6(0xB0, 0x03, 0x01,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
void FS_QC_RS_OK(void)
{
    wr_cmd_array_6(0xB0, 0x04, 0x00,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
//�ʿ�ͨ��*
void FS_QC_RS_CRI_OK(void)
{
    wr_cmd_array_6(0xB0, 0x04, 0x01,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
void FS_QC_RS_ERR(void)
{
    wr_cmd_array_6(0xD0, 0x06, 0x02,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
//�ʿط��ش���
void FS_QC_BACK(void)
{
    wr_cmd_array_6(0xB0, 0x05, 0x00,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
//������ʾ����ʾ"�ʿ�δ���(����CO2����ϵͳ)�ε������������������ش�������"
void FS_QC_RUN_ERR1(void)
{
    wr_cmd_array_6(0xD0, 0x06, 0x00,0x00);
    RBCtoPC_Cmd_61();
    F_Timeout=0;
}
//������ʾ����ʾ���ʿ�δ���(Zero Fault),�ε������������������ش������桱
void FS_QC_RUN_ERR2(void)
{
    wr_cmd_array_6(0xD0, 0x06, 0x01,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
static void Swap(uint8_t *p1, uint8_t *p2) // ��������
{
    uint8_t temp;
    temp=*p1;
    *p1=*p2;
    *p2=temp;
}
/**
  *@breif ������������
  *@param type:��������
          type=0Ϊ������
          temp=1Ϊ������
  *@retval ��
  */
void FS_SampleType(uint8_t type)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xE0;
    RBC_pc_ComT[1]=0x05;
    RBC_pc_ComT[2]=0x00;
    RBC_pc_ComT[3]=0x00;
    RBC_pc_ComT[4]=type;
    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();
}
/**
  *@breif �����Ա�
  *@param temp:�Ա�
          temp=0ΪŮ��
          temp=1Ϊ�У�
  *@retval ��
  */
void FS_SEX(uint8_t *temp)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xE0;
    RBC_pc_ComT[1]=0x02;
    RBC_pc_ComT[2]=0x01;
    RBC_pc_ComT[3]=0x00;
    RBC_pc_ComT[4]=*temp;
    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();
}
void FS_RBC(uint16_t *temp)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x00;
    RBC_pc_ComT[2]=0x00;

    memcpy(&RBC_pc_ComT[3],temp,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();
}

void FS_Pco(float *PCO)
{
    uint8_t cheak=0;
    uint32_t temp;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x02;
    temp=(uint32_t)(*PCO*10000+0.5);

    memcpy(&RBC_pc_ComT[2],&temp,4);  // 4�ֽ���Ҫ����  ��λ�ֽ���ǰ����λ�ֽ��ں�
    Swap(&RBC_pc_ComT[2], &RBC_pc_ComT[4]);
//  Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();
}

void FS_CO2(float *CO2)
{
    uint8_t cheak=0;
    uint16_t temp;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x03;

    if((((uint16_t)(*CO2*1000))%10)<5)
    {
        temp=(uint16_t)(*CO2*100);
    }
    else
        temp=(uint16_t)(*CO2*100)+1;

    memcpy(&RBC_pc_ComT[2],&temp,4);
    Swap(&RBC_pc_ComT[2], &RBC_pc_ComT[4]);
//  Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);
    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();
}

void FS_HB(uint16_t *temp)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x04;
    RBC_pc_ComT[2]=0x00;

    memcpy(&RBC_pc_ComT[3],temp,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();

}
//������Ծ��ܶ�����
void FS_RP(uint16_t *temp)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x07;
    RBC_pc_ComT[2]=0x00;

    memcpy(&RBC_pc_ComT[3],temp,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();

}
//�������׼ȷ������
void FS_RA(uint16_t *temp)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x08;
    RBC_pc_ComT[2]=0x00;

    memcpy(&RBC_pc_ComT[3],temp,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();
}
void FS_ZERO(int16_t *Zero)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xC0;
    RBC_pc_ComT[1]=0x05;
    RBC_pc_ComT[2]=0x00;

    memcpy(&RBC_pc_ComT[3],Zero,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();

}



void FS_QUA_ON(void)
{
    wr_cmd_array_6(0xB0, 0x00, 0x00,0x00);
    RBCtoPC_Cmd_6();
}
void FS_QUA_OK(void)
{
    wr_cmd_array_6(0xB0, 0x01, 0x00,0x00);
    RBCtoPC_Cmd_6();
}
void FS_QUA_ERR(void)
{
    wr_cmd_array_6(0xB0, 0x02, 0x00,0x00);
    RBCtoPC_Cmd_6();
    F_Timeout=0;
}
void FS_CO2_WARNING(void)//CO2������ʾ  20180628
{
    wr_cmd_array_6(0xD0,0x04,0x00,0x00);
    RBCtoPC_Cmd_6();
}

void FS_CO2_Fault(void)//CO2�����쳣��ʾ
{
    wr_cmd_array_6(0xD0,0x04,0x01,0x00);
    RBCtoPC_Cmd_6();
}
/*
���ͱ���������������ֵ 20180628
*/
void FS_Diff_ZERO(uint16_t *DIFF)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xD0;
    RBC_pc_ComT[1]=0x05;
    RBC_pc_ComT[2]=0x00;

    memcpy(&RBC_pc_ComT[3],DIFF,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();

}
/**
  *@breif ��������쳣����
  *@prarm DIFF:����ָ��
  *@retval ��
*/
void FS_ZERO_Fault(uint16_t *DIFF)
{
    uint8_t cheak=0;
    uint8_t i;
    RBC_pc_ComT[0]=0xD0;
    RBC_pc_ComT[1]=0x05;
    RBC_pc_ComT[2]=0x01;

    memcpy(&RBC_pc_ComT[3],DIFF,2);
    Swap(&RBC_pc_ComT[3], &RBC_pc_ComT[4]);

    for(i=0; i<5; i++)
    {
        cheak+=RBC_pc_ComT[i];
    }
    RBC_pc_ComT[5]=cheak;

    RBCtoPC_Cmd_6();

}
