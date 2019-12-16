#include "NVIC_Config.h"

/**
  *@breif  优先级分组设置
	*@param  none
	*@retval none
*/
void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Init(&NVIC_InitStructure);
}

