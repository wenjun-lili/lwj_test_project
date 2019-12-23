#ifndef _BSP_EXTI_H
#define _BSP_EXTI_H

#include "stm32f10x.h"
#include "stm32f10x_exti.h"
#include "bsp.h"
#include "bsp_SysTick.h"
#include "globalVar.h"

/*NA_RUN_KEY*/
#define KEY1                       PDin(12)
#define KEY1_INT_GPIO_PORT         GPIOD
#define KEY1_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY1_INT_GPIO_PIN          GPIO_Pin_12
#define KEY1_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY1_INT_EXTI_PINSOURCE    GPIO_PinSource12
#define KEY1_INT_EXTI_LINE         EXTI_Line12
#define KEY1_INT_EXTI_IRQ          EXTI15_10_IRQn

/*ENV_RUN_KEY*/
#define KEY2                       PDin(13)
#define KEY2_INT_GPIO_PORT         GPIOD
#define KEY2_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY2_INT_GPIO_PIN          GPIO_Pin_13
#define KEY2_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY2_INT_EXTI_PINSOURCE    GPIO_PinSource13
#define KEY2_INT_EXTI_LINE         EXTI_Line13
#define KEY2_INT_EXTI_IRQ          EXTI15_10_IRQn

/*LUNG_RUN_KEY*/
#define KEY3                       PDin(14)
#define KEY3_INT_GPIO_PORT         GPIOD
#define KEY3_INT_GPIO_CLK          (RCC_APB2Periph_GPIOD|RCC_APB2Periph_AFIO)
#define KEY3_INT_GPIO_PIN          GPIO_Pin_14
#define KEY3_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOD
#define KEY3_INT_EXTI_PINSOURCE    GPIO_PinSource14
#define KEY3_INT_EXTI_LINE         EXTI_Line14
#define KEY3_INT_EXTI_IRQ          EXTI15_10_IRQn

void EXTI_Key_Config(void);

#endif
