#include "NVIC_Config.h"

void NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
    NVIC_Init(&NVIC_InitStructure);
}

