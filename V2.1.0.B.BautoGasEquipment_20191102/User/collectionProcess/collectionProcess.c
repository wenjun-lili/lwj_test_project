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
  *@breif  环境本底气采集程序
	*@param  none
	*@retval none
	*/
void airCollection(void)
{
    CO2LedDoing(0);//关闭CO2灯
    errorLedDoing(0);//关闭error灯
    runLedDoing(0);//关闭run灯

    timeCnt = 80; //总工周期80s

    TIM_Cmd(TIM7, ENABLE); //打开定时器TIM7

    GPIO_ResetBits(GPIOC, GPIO_Pin_9); //全速运行气泵

    while(1)
    {
        runLedDoing(timeCnt % 2); //运行状态闪烁亮起
        if(timeCnt == 0) //时间完成跳出
        {
            runLedDoing(0);
            break;
        }

    }
    GPIO_SetBits(GPIOC, GPIO_Pin_9); //关闭环境本底气采气气泵
    TIM_Cmd(TIM7, DISABLE); //关闭定时器
    beepMode(1);//蜂鸣器响1声
    printf("本底气采集完成\r\n");
}

/**
  *@breif  肺泡气采集程序
	*@param  none
	*@retval none
	*/
void breathCollection(void)
{
    float prePressure = 0.0; //气压前值
    float afterPressure = 0.0; //气压后值
    uint8_t flagEnvFinish = 0; //1代表本底气袋充满
    uint16_t beginTime = 0; //计时开始数
    uint16_t endTime = 0; //计时结束数

    flagOKSec = 0; //赋初始值
    overTimeCnt = 0;
    errorLedDoing(0);//故障灯灭
		printf("呼气采集占空比为：%d\r\n", PWMDuty);
		printf("始点阈值系数：%f  截止点阈值系数:%f\r\n",thresholdCoeff,cutOffCoeff);

    printf("开始采集呼气周期\r\n");
    getRespiratoryCycle();//采取呼气周期
    BME280_ReadPressure(&prePressure);//读取运行前气压值
    printf("运行前气压值为：%f\r\n", prePressure);

    timeCnt = 3600; //3600s时间设置
    GPIO_ResetBits(GPIOC, GPIO_Pin_9); //打开环境本底气气泵，全速运行
    TIM_Cmd(TIM7, ENABLE); //打开定时器TIM7
    while(1)
    {
        if(timeCnt == 0)
        {
            timeCnt = 3600; //重新赋值3600
        }
        if(overTimeCnt >= 900) //若超15分钟，则需要跳出来采气流程
        {
            runLedDoing(0);//状态灯关闭
            CO2LedDoing(0);//浓度灯关闭
            errorLedDoing(1);//故障灯亮起
            flagError = 1; //故障标志置1
            printf("超时15分钟\r\n");
            break;//跳出循环
        }
        else
        {
            if((flagEnvFinish == 0) && (timeCnt <= 3520)) //环境本底气气泵运行80s，不能取临界值
            {
                flagEnvFinish = 1; //赋值为1，避免重复采样
                printf("环境本底气已采集完成，关闭气泵\r\n");
                GPIO_SetBits(GPIOC, GPIO_Pin_9); //环境本底气采集完成，关闭气泵

            }
            if(flagOKSec == 1) //测量气压值
            {
                flagOKSec = 0;

#if 0
                printf("CO2浓度=%d\r\n", CO2Con);

                if(CO2Con > 250) //若采气大于2.5%
                {
                    evalveClose(2);//关闭电磁阀2

                    CO2LedDoing(1);//CO2浓度指示灯亮起

                    if(flagOKGas == 0)
                    {
                        flagOKGas = 1; //采集到合格气体标志置1
                        beginTime = timeCnt;
                    }
                    else
                    {
                        overTimeCnt += beginTime - timeCnt; //计时
                        beginTime = timeCnt;
                    }
                }
                else
                {
                    evalveOpen(2);//打开电磁阀2
                    CO2LedDoing(0);//CO2浓度指示灯熄灭
                    if(flagOKGas == 1) //若前面周期采气的是合格气体
                    {
                        flagOKGas = 0;
                        endTime = timeCnt; //计数结束
                        overTimeCnt += beginTime - endTime; //合格采气时间，开始时间大

                    }

                }
#endif
                BME280_ReadPressure(&afterPressure);//读取运行后气压值
                printf("测量气压值为：%f\r\n", afterPressure);

                if((afterPressure - prePressure) > 50) //如果气压差大于20hPa
                {
                    printf("气袋已满\r\n");
                    break;//结束采气
                }
            }
            breathScan();//进行采气
        }

    }

    /*采气完成，清洗气路*/
    printf("开始清洗\r\n");

    evalveOpen(2);//打开电磁阀2

    evalveClose(1);//关闭电磁阀1

    timeCnt = 10;

    TIM_SetCompare3(TIM8, 100); //全速运行气泵

    while(timeCnt != 0);

    TIM_SetCompare3(TIM8, 0); //关闭气泵

    TIM_Cmd(TIM7, DISABLE); //关闭定时器

    runLedDoing(0);//运行状态灯灭
    CO2LedDoing(0);
    beepMode(3);//蜂鸣器响3声

    printf("肺泡气整个流程完成\r\n");

    flagLungRun = 0; //标志位置0
    flagOKGas = 0;
    flagError = 0;
    F_Key = 0; //防止采气过程中按下其他键
}
#if 0
/**
  *@breif  呼吸周期采集
	*@datail 利用腹压传感器采集呼气和吸气总周期
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
    uint8_t  testNum = 0; //测量到的数据组数
    uint32_t sumTime = 0;
    uint16_t meanTime = 0;
    uint8_t  m = 0;
    uint16_t n = 0;
    uint8_t i = 0;
//    /*20s静置*/
    printf("先静置20s\r\n");
    timeCnt = 20;
    TIM_Cmd(TIM7, ENABLE);
    while(timeCnt);
    TIM_Cmd(TIM7, DISABLE); //关闭定时器


    /*开始测定*/
    timeCnt = 64000;
    TIM_Cmd(TIM6, ENABLE);
    printf("开始对呼气周期进行采样\r\n");
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

                    while(breathVol == 1) //电平为高
                    {
                        SysTick_Delay_Ms(1);
                        n++;
                    }
                    SysTick_Delay_Ms(100);

                    while(breathVol == 1) //再次判断电平为高
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
    TIM_Cmd(TIM6, DISABLE); //关闭定时器
    printf("完成采样\r\n");
    /*求平均值*/
    for(i = 0; i < testNum; i++)
    {
        printf("%d次：%d，%d，%d\r\n", i, startTime[i], endTime[i], expTime[i]);
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
    printf("呼气平均时间为=%d\r\n", meanTime);

}
#else
void getRespiratoryCycle(void)
{
    uint16_t i, j, m;
    uint8_t M = 0;
    uint8_t N = 0;
    uint16_t temp = 0;
    uint16_t prePeakRes[70] = {0}; //初步筛选的波峰值数组
    uint16_t preVallcyRes[70] = {0}; //初步筛选的波谷值数组
    uint16_t avePeakRes = 0; //平均波峰值
    uint16_t aveVallcyRes = 0; //平均波谷值
    uint16_t finalAvePV = 0; //平均峰谷差值


    USART_Cmd(USART3, ENABLE);//使能串口3，在DMA模式此时打开
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE); //打开串口DMA模式

    evalveOpen(1);//打开电磁阀1
    evalveOpen(2);//打开电磁阀2
    runLedDoing(1);//运行状态灯亮起

    printf("进入空载时间10s\r\n");
    timeCnt = 10;
    TIM_SetCompare3(TIM8, PWMDuty); //设置占空比,打开气泵
    TIM_Cmd(TIM7, ENABLE);
    while(timeCnt);
    TIM_Cmd(TIM7, DISABLE); //关闭定时器
    printf("开始学习\r\n");

    timeCnt = 1200; //60s时间1200s采样设置
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
        /*判断波峰情况*/
        if((CO2CONC[i] > CO2CONC[i - 1]) && (CO2CONC[i] >= CO2CONC[i + 1]))
        {
            if(CO2CONC[i] == CO2CONC[i + 1]) //若等于，则继续往右判断
            {
                for(m = i + 2; m < 1199; m++)
                {
                    if(CO2CONC[i] != CO2CONC[m])
                    {
                        if(CO2CONC[i] > CO2CONC[m])
                        {
                            prePeakRes[N] = CO2CONC[i];
                            printf("第%d个波峰数据=%d\r\n", N, prePeakRes[N]);
                            N++;
                        }
                        break;//跳出判断
                    }
                }
            }
            else   //若不等于，则说明是波峰
            {
                prePeakRes[N] = CO2CONC[i];
                printf("第%d个波峰数据=%d\r\n", N, prePeakRes[N]);
                N++;
            }
        }
        /*判断波谷情况*/
        if((CO2CONC[i] < CO2CONC[i - 1]) && (CO2CONC[i] <= CO2CONC[i + 1]))
        {
            if(CO2CONC[i] == CO2CONC[i + 1]) //若等于，则继续往右判断
            {
                for(m = i + 2; m < 1199; m++)
                {
                    if(CO2CONC[i] != CO2CONC[m])
                    {
                        if(CO2CONC[i] < CO2CONC[m])
                        {
                            preVallcyRes[M] = CO2CONC[i];
                            printf("第%d个波谷数据=%d\r\n", M, preVallcyRes[M]);
                            M++;
                        }
                        break;//跳出判断
                    }
                }
            }
            else
            {
                preVallcyRes[M] = CO2CONC[i];
                printf("第%d个波谷数据=%d\r\n", M, preVallcyRes[M]);
                M++;
            }
        }
    }
    /*对波峰值进行排序和求前六个最大数值的平均值*/
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
    printf("平均波峰值=%d\r\n", avePeakRes);

    /*对波谷值进行排序和求平均值*/
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
    printf("平均波谷值=%d\r\n", aveVallcyRes);

    /*求平均峰谷差值*/

    finalAvePV = avePeakRes - aveVallcyRes;
    printf("平均峰谷差值=%d\r\n", finalAvePV);
    /*求采气始点*/
    CO2Threshold = aveVallcyRes + finalAvePV * thresholdCoeff;
    printf("采气始点=%d\r\n", CO2Threshold);

    /*求采气截止点*/
    cutOffCO2 = aveVallcyRes + finalAvePV * cutOffCoeff;
    printf("采气截止点=%d\r\n", cutOffCO2);

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


