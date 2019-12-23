#include "collectionProcess.h"
#include "bsp_i2c_ee.h"
#include "globalVar.h"
#include "sensorPe.h"
#include "beep.h"
#include "pump.h"
#include "evalve.h"
#include "bsp_led.h"
#include "stdlib.h"

/**
  *@breif  �����������ɼ�����
	*@param  none
	*@retval none
	*/
void airCollection(void)
{
    CO2LedDoing(0);//�ر�CO2��
    errorLedDoing(0);//�ر�error��
    runLedDoing(0);//�ر�run��

    timeCnt = 80; //�ܹ�����80s

    TIM_Cmd(TIM7, ENABLE); //�򿪶�ʱ��TIM7

    GPIO_ResetBits(GPIOC, GPIO_Pin_9); //ȫ����������

    while(1)
    {
        runLedDoing(timeCnt % 2); //����״̬��˸����
        if(timeCnt == 0) //ʱ���������
        {
            runLedDoing(0);
            break;
        }

    }
    GPIO_SetBits(GPIOC, GPIO_Pin_9); //�رջ�����������������
    TIM_Cmd(TIM7, DISABLE); //�رն�ʱ��
    beepMode(1);//��������1��
    printf("�������ɼ����\r\n");
}

/**
  *@breif  �������ɼ�����
	*@param  none
	*@retval none
	*/
void breathCollection(void)
{
    float prePressure = 0.0; //��ѹǰֵ
    float afterPressure = 0.0; //��ѹ��ֵ
    uint8_t flagEnvFinish = 0; //1����������������
    uint16_t beginTime = 0; //��ʱ��ʼ��
    uint16_t endTime = 0; //��ʱ������

    flagOKSec = 0; //����ʼֵ
    overTimeCnt = 0;
    errorLedDoing(0);//���ϵ���
		printf("�����ɼ�ռ�ձ�Ϊ��%d\r\n", PWMDuty);
		printf("ʼ����ֵϵ����%f  ��ֹ����ֵϵ��:%f\r\n",thresholdCoeff,cutOffCoeff);

    printf("��ʼ�ɼ���������\r\n");
    getRespiratoryCycle();//��ȡ��������
    BME280_ReadPressure(&prePressure);//��ȡ����ǰ��ѹֵ
    printf("����ǰ��ѹֵΪ��%f\r\n", prePressure);

    timeCnt = 3600; //3600sʱ������
    GPIO_ResetBits(GPIOC, GPIO_Pin_9); //�򿪻������������ã�ȫ������
    TIM_Cmd(TIM7, ENABLE); //�򿪶�ʱ��TIM7
    while(1)
    {
        if(timeCnt == 0)
        {
            timeCnt = 3600; //���¸�ֵ3600
        }
        if(overTimeCnt >= 900) //����15���ӣ�����Ҫ��������������
        {
            runLedDoing(0);//״̬�ƹر�
            CO2LedDoing(0);//Ũ�ȵƹر�
            errorLedDoing(1);//���ϵ�����
            flagError = 1; //���ϱ�־��1
            printf("��ʱ15����\r\n");
            break;//����ѭ��
        }
        else
        {
            if((flagEnvFinish == 0) && (timeCnt <= 3520)) //������������������80s������ȡ�ٽ�ֵ
            {
                flagEnvFinish = 1; //��ֵΪ1�������ظ�����
                printf("�����������Ѳɼ���ɣ��ر�����\r\n");
                GPIO_SetBits(GPIOC, GPIO_Pin_9); //�����������ɼ���ɣ��ر�����

            }
            if(flagOKSec == 1) //������ѹֵ
            {
                flagOKSec = 0;

#if 0
                printf("CO2Ũ��=%d\r\n", CO2Con);

                if(CO2Con > 250) //����������2.5%
                {
                    evalveClose(2);//�رյ�ŷ�2

                    CO2LedDoing(1);//CO2Ũ��ָʾ������

                    if(flagOKGas == 0)
                    {
                        flagOKGas = 1; //�ɼ����ϸ������־��1
                        beginTime = timeCnt;
                    }
                    else
                    {
                        overTimeCnt += beginTime - timeCnt; //��ʱ
                        beginTime = timeCnt;
                    }
                }
                else
                {
                    evalveOpen(2);//�򿪵�ŷ�2
                    CO2LedDoing(0);//CO2Ũ��ָʾ��Ϩ��
                    if(flagOKGas == 1) //��ǰ�����ڲ������Ǻϸ�����
                    {
                        flagOKGas = 0;
                        endTime = timeCnt; //��������
                        overTimeCnt += beginTime - endTime; //�ϸ����ʱ�䣬��ʼʱ���

                    }

                }
#endif
                BME280_ReadPressure(&afterPressure);//��ȡ���к���ѹֵ
                printf("������ѹֵΪ��%f\r\n", afterPressure);

                if((afterPressure - prePressure) > 50) //�����ѹ�����20hPa
                {
                    printf("��������\r\n");
                    break;//��������
                }
            }
            breathScan();//���в���
        }

    }

    /*������ɣ���ϴ��·*/
    printf("��ʼ��ϴ\r\n");

    evalveOpen(2);//�򿪵�ŷ�2

    evalveClose(1);//�رյ�ŷ�1

    timeCnt = 10;

    TIM_SetCompare3(TIM8, 100); //ȫ����������

    while(timeCnt != 0);

    TIM_SetCompare3(TIM8, 0); //�ر�����

    TIM_Cmd(TIM7, DISABLE); //�رն�ʱ��

    runLedDoing(0);//����״̬����
    CO2LedDoing(0);
    beepMode(3);//��������3��

    printf("�����������������\r\n");

    flagLungRun = 0; //��־λ��0
    flagOKGas = 0;
    flagError = 0;
    F_Key = 0; //��ֹ���������а���������
}
#if 0
/**
  *@breif  �������ڲɼ�
	*@datail ���ø�ѹ�������ɼ�����������������
	*@param  none
	*@retval none
	*/
void getRespiratoryCycle(void)
{
    uint16_t startTime[100] = {0};
    uint16_t endTime[100] = {0};
    uint16_t expTime[100] = {0};
    uint16_t maxTime = 0;
    uint16_t minTime = 0;
    uint8_t  testNum = 0; //����������������
    uint32_t sumTime = 0;
    uint16_t meanTime = 0;
    uint8_t  m = 0;
    uint16_t n = 0;
    uint8_t i = 0;
//    /*20s����*/
    printf("�Ⱦ���20s\r\n");
    timeCnt = 20;
    TIM_Cmd(TIM7, ENABLE);
    while(timeCnt);
    TIM_Cmd(TIM7, DISABLE); //�رն�ʱ��


    /*��ʼ�ⶨ*/
    timeCnt = 64000;
    TIM_Cmd(TIM6, ENABLE);
    printf("��ʼ�Ժ������ڽ��в���\r\n");
    while(timeCnt > 4000)
    {
        if(breathVol == 0)
        {
            SysTick_Delay_Ms(1);
            while(1)
            {
                n = 0;
                while(breathVol == 0)
                {
                    SysTick_Delay_Ms(1);
                }

                SysTick_Delay_Ms(100);

                if(breathVol == 1)
                {
                    startTime[i] = timeCnt + 100;
                    testNum++;

                    while(breathVol == 1) //��ƽΪ��
                    {
                        SysTick_Delay_Ms(1);
                        n++;
                    }
                    SysTick_Delay_Ms(100);

                    while(breathVol == 1) //�ٴ��жϵ�ƽΪ��
                    {
                        SysTick_Delay_Ms(1);
                    }
                    endTime[i] = timeCnt + 100 + n;

                    expTime[i] = startTime[i] - endTime[i];

                    i++;

                    break;
                }
                else
                {
                    break;
                }
            }
        }
    }
    TIM_Cmd(TIM6, DISABLE); //�رն�ʱ��
    printf("��ɲ���\r\n");
    /*��ƽ��ֵ*/
    for(i = 0; i < testNum; i++)
    {
        printf("%d�Σ�%d��%d��%d\r\n", i, startTime[i], endTime[i], expTime[i]);
        Delay10ms(10);
        if(expTime[i] > 200)
        {
            expTime[m] = expTime[i];
            m++;
        }
        else
        {
            testNum--;
        }

    }
    for(i = 0; i < testNum; i++)
    {
        sumTime += expTime[i];
        if(expTime[i] > expTime[maxTime])
        {
            maxTime = i;
        }
        else if(expTime[i] < expTime[minTime])
        {
            minTime = i;
        }

    }
    meanTime = (sumTime - expTime[minTime] - expTime[maxTime]) / (testNum - 2);
    printf("����ƽ��ʱ��Ϊ=%d\r\n", meanTime);

}
#else
void getRespiratoryCycle(void)
{
    uint16_t i, j, m;
    uint8_t M = 0;
    uint8_t N = 0;
    uint16_t temp = 0;
    uint16_t prePeakRes[70] = {0}; //����ɸѡ�Ĳ���ֵ����
    uint16_t preVallcyRes[70] = {0}; //����ɸѡ�Ĳ���ֵ����
    uint16_t avePeakRes = 0; //ƽ������ֵ
    uint16_t aveVallcyRes = 0; //ƽ������ֵ
    uint16_t finalAvePV = 0; //ƽ����Ȳ�ֵ


    USART_Cmd(USART3, ENABLE);//ʹ�ܴ���3����DMAģʽ��ʱ��
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); //�򿪴���DMAģʽ

    evalveOpen(1);//�򿪵�ŷ�1
    evalveOpen(2);//�򿪵�ŷ�2
    runLedDoing(1);//����״̬������

    printf("�������ʱ��10s\r\n");
    timeCnt = 10;
    TIM_SetCompare3(TIM8, PWMDuty); //����ռ�ձ�,������
    TIM_Cmd(TIM7, ENABLE);
    while(timeCnt);
    TIM_Cmd(TIM7, DISABLE); //�رն�ʱ��
    printf("��ʼѧϰ\r\n");

    timeCnt = 1200; //60sʱ��1200s��������
    TIM_Cmd(TIM6, ENABLE);
    while(timeCnt);
    TIM_Cmd(TIM6, DISABLE);
    for(i = 0; i < 1200; i++)
    {
        printf("%d=%d\r\n", i, CO2CONC[i]);
        Delay1ms(21);
    }
    for(i = 1; i < 1198; i++)
    {
        /*�жϲ������*/
        if((CO2CONC[i] > CO2CONC[i - 1]) && (CO2CONC[i] >= CO2CONC[i + 1]))
        {
            if(CO2CONC[i] == CO2CONC[i + 1]) //�����ڣ�����������ж�
            {
                for(m = i + 2; m < 1199; m++)
                {
                    if(CO2CONC[i] != CO2CONC[m])
                    {
                        if(CO2CONC[i] > CO2CONC[m])
                        {
                            prePeakRes[N] = CO2CONC[i];
                            printf("��%d����������=%d\r\n", N, prePeakRes[N]);
                            N++;
                        }
                        break;//�����ж�
                    }
                }
            }
            else   //�������ڣ���˵���ǲ���
            {
                prePeakRes[N] = CO2CONC[i];
                printf("��%d����������=%d\r\n", N, prePeakRes[N]);
                N++;
            }
        }
        /*�жϲ������*/
        if((CO2CONC[i] < CO2CONC[i - 1]) && (CO2CONC[i] <= CO2CONC[i + 1]))
        {
            if(CO2CONC[i] == CO2CONC[i + 1]) //�����ڣ�����������ж�
            {
                for(m = i + 2; m < 1199; m++)
                {
                    if(CO2CONC[i] != CO2CONC[m])
                    {
                        if(CO2CONC[i] < CO2CONC[m])
                        {
                            preVallcyRes[M] = CO2CONC[i];
                            printf("��%d����������=%d\r\n", M, preVallcyRes[M]);
                            M++;
                        }
                        break;//�����ж�
                    }
                }
            }
            else
            {
                preVallcyRes[M] = CO2CONC[i];
                printf("��%d����������=%d\r\n", M, preVallcyRes[M]);
                M++;
            }
        }
    }
    /*�Բ���ֵ�����������ǰ���������ֵ��ƽ��ֵ*/
    for(i = 0; i < N - 1; i++)
    {
        for(j = 0; j < N - 1 - i; j++)
        {
            if(prePeakRes[j] < prePeakRes[j + 1])
            {
                temp = prePeakRes[j];
                prePeakRes[j] = prePeakRes[j + 1];
                prePeakRes[j + 1] = temp;
            }
        }
    }
    for(i = 0; i < 6; i++)
    {
        avePeakRes += prePeakRes[i];
    }
    avePeakRes = avePeakRes / 6;
    printf("ƽ������ֵ=%d\r\n", avePeakRes);

    /*�Բ���ֵ�����������ƽ��ֵ*/
    for(i = 0; i < M - 1; i++)
    {
        for(j = 0; j < M - 1 - i; j++)
        {
            if(preVallcyRes[j] > preVallcyRes[j + 1])
            {
                temp = preVallcyRes[j];
                preVallcyRes[j] = preVallcyRes[j + 1];
                preVallcyRes[j + 1] = temp;
            }
        }
    }
    for(i = 0; i < 6; i++)
    {
        aveVallcyRes += preVallcyRes[i];
    }
    aveVallcyRes = aveVallcyRes / 6;
    printf("ƽ������ֵ=%d\r\n", aveVallcyRes);

    /*��ƽ����Ȳ�ֵ*/

    finalAvePV = avePeakRes - aveVallcyRes;
    printf("ƽ����Ȳ�ֵ=%d\r\n", finalAvePV);
    /*�����ʼ��*/
    CO2Threshold = aveVallcyRes + finalAvePV * thresholdCoeff;
    printf("����ʼ��=%d\r\n", CO2Threshold);

    /*�������ֹ��*/
    cutOffCO2 = aveVallcyRes + finalAvePV * cutOffCoeff;
    printf("������ֹ��=%d\r\n", cutOffCO2);

}


#endif

uint16_t leftAve(uint16_t m)
{
    uint16_t ave;
    ave = (CO2CONC[m - 1] + CO2CONC[m - 2] + CO2CONC[m - 3]) / 3;
    return ave;
}

uint16_t rightAve(uint16_t m)
{
    uint16_t ave;
    ave = (CO2CONC[m + 1] + CO2CONC[m + 2] + CO2CONC[m + 3]) / 3;
    return ave;
}

