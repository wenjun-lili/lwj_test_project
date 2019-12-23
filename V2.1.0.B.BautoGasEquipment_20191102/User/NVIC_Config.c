#include "NVIC_Config.h"

void NVIC_Config(void)
{
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
