#include "sensorPe.h"
#include "evalve.h"
#include "bsp_led.h"

/**
  @brief  呼吸检测传感器初始化
	@param  无
	@retval 无
*/
void breathSensorInit(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 ;				//GPIOE2
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);
}

#if 0
/**
 @breif 呼吸传感器电平扫描
 @param none
 @retval none
 */
void breathScan(void)
{
    SysTick_Delay_Ms(1);
    if(breathVol == 0)
    {
        SysTick_Delay_Ms(1);
        while(1)
        {
            while(breathVol == 0)
            {
                SysTick_Delay_Ms(1);
            }

            SysTick_Delay_Ms(1);

            if(breathVol == 1)
            {
                if(delayBegin > 0) //延时函数参数不能为0
                {
                    SysTick_Delay_Ms(delayBegin);//采气前延时
                }
                TIM_SetCompare3(TIM8, PWMDuty); //设置占空比,打开气泵

                while(breathVol == 1) //电平为高
                {
                    SysTick_Delay_Ms(1);
                }
                SysTick_Delay_Ms(1);
                while(breathVol == 1) //再次判断电平为高
                {
                    SysTick_Delay_Ms(1);
                }

                if(delayAfter > 0) //延时函数参数不能为0
                {
                    SysTick_Delay_Ms(delayAfter);//延时关闭气泵
                }
//               TIM_SetCompare3(TIM8, 100); //设置占空比,关闭气泵
                TIM_SetCompare3(TIM8,0); //设置占空0,低电平关闭气泵

                SysTick_Delay_Ms(200);

                break;
            }
            else
            {
                break;
            }
        }
    }
}

#else
/**
  *@breif  CO2传感器浓度扫描
	*@detail
	*@param  none
	*@retval none
	*/
void breathScan(void)
{
    uint16_t i=0;
    SysTick_Delay_Ms(1);
    if(CO2Con < CO2Threshold)
    {
        SysTick_Delay_Ms(10);
        while(1)
        {
            while(CO2Con < CO2Threshold)
            {
                SysTick_Delay_Ms(10);
            }
            SysTick_Delay_Ms(50);
            if(CO2Con >= CO2Threshold)
            {
                evalveClose(2);//关闭电磁阀2
								CO2LedDoing(1);//CO2浓度指示灯亮起
								printf("采集有效气体=%d\r\n",CO2Con);
								for(i=0;i<7000;i++)
								{
								   SysTick_Delay_Ms(1);
									 if(CO2Con<cutOffCO2)
									 {
									   break;
									 }
									 if((i%50)==0)
									 {
									     printf("浓度=%d\r\n",CO2Con);									 
									 }
								
								}
//                SysTick_Delay_Ms(delayAfter);
								printf("停止采集有效气体=%d\r\n",CO2Con);
								evalveOpen(2);
								CO2LedDoing(0);//CO2浓度指示灯熄灭
								break;
            }
						else
						{
						break;
						}

        }


    }
}


#endif


