#include "NVIC_Config.h"

/**
  *@breif  ���ȼ���������
	*@param  none
	*@retval none
*/
void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Init(&NVIC_InitStructure);
}

