

#include "system.h"
#include "SysTick.h"
#include "led.h"
#include "key.h"
#include "exti.h"

int main()
{
	u8 i;
	SysTick_Init(72);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  //�ж����ȼ����� ��2��
	LED_Init();
	KEY_Init();
	My_EXTI_Init();  //�ⲿ�жϳ�ʼ��
	while(1)
	{
		i++;
		if(i%20==0)
		{
			led1=!led1;
		}
		delay_ms(10);	
	}
}
