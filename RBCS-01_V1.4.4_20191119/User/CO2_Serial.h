#ifndef _CO2_SERIAL_H_
#define _CO2_SERIAL_H_
#include "stm32f10x.h"

void USART3_Config(void);
void USART3_Variable(void);
void USART3_Byte( USART_TypeDef * pUSARTx, uint8_t ch);
void USART3_String( USART_TypeDef * pUSARTx, char *str);
void Rs0_Out(void);
uint8_t CharToDec(uint8_t bHex);
void USART3_Send_1(void);
void USART3_Send_2(void);
void USART3_Send_3(void);
void USART3_Send_4(void);

#endif

