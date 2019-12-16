#ifndef _LED_H_
#define _LED_H_

#include "stm32f10x.h"

void LED_All_Close( void );

void led_GPIO_Init(void);

uint8_t is_gasbag_exit(uint8_t num);


void Send_Gas_Com(void);
#endif
