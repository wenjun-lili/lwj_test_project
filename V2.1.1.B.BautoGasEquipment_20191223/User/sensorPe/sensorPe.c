#include "sensorPe.h"
#include "evalve.h"
#include "bsp_led.h"

/**
  @brief  ������⴫������ʼ��
	@param  ��
	@retval ��
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
 @breif ������������ƽɨ��
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
                if(delayBegin > 0) //��ʱ������������Ϊ0
                {
                    SysTick_Delay_Ms(delayBegin);//����ǰ��ʱ
                }
                TIM_SetCompare3(TIM8, PWMDuty); //����ռ�ձ�,������

                while(breathVol == 1) //��ƽΪ��
                {
                    SysTick_Delay_Ms(1);
                }
                SysTick_Delay_Ms(1);
                while(breathVol == 1) //�ٴ��жϵ�ƽΪ��
                {
                    SysTick_Delay_Ms(1);
                }

                if(delayAfter > 0) //��ʱ������������Ϊ0
                {
                    SysTick_Delay_Ms(delayAfter);//��ʱ�ر�����
                }
//               TIM_SetCompare3(TIM8, 100); //����ռ�ձ�,�ر�����
                TIM_SetCompare3(TIM8,0); //����ռ��0,�͵�ƽ�ر�����

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
  *@breif  CO2������Ũ��ɨ��
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
                evalveClose(2);//�رյ�ŷ�2
								CO2LedDoing(1);//CO2Ũ��ָʾ������
								printf("�ɼ���Ч����=%d\r\n",CO2Con);
								for(i=0;i<7000;i++)
								{
								   SysTick_Delay_Ms(1);
									 if(CO2Con<cutOffCO2)
									 {
									   break;
									 }
									 if((i%50)==0)
									 {
									     printf("Ũ��=%d\r\n",CO2Con);									 
									 }
								
								}
//                SysTick_Delay_Ms(delayAfter);
								printf("ֹͣ�ɼ���Ч����=%d\r\n",CO2Con);
								evalveOpen(2);
								CO2LedDoing(0);//CO2Ũ��ָʾ��Ϩ��
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


