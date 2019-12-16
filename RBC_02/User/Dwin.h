#ifndef _DWIN_H_
#define _DWIN_H_

#include "stm32f10x.h"
//Ê¹ÓÃ´®¿Ú3

void Dwins_Variable(void);
void DGUS_Set_para(void);
void USART2_Config(void);
void Rs0_Out(uint8_t Cmd);
void send_Ti_scr(uint16_t d,uint8_t *p,uint8_t s);
void Wr_82_Scr(uint16_t d,uint8_t *p,uint8_t s);
void Wr_84_Scr(uint8_t d,uint16_t s);
void Rd_83_Scr(uint16_t d,uint8_t s);
void CQ_To_screen(uint8_t m,uint8_t z,uint16_t d,uint16_t g) ;
void Wr_80_Scr(uint8_t d,uint8_t *p,uint8_t s);
void Rd_81_Scr(uint8_t d,uint8_t s);
uint8_t BCDtoDec(uint8_t temp);
void Dwins_Variable(void);

#endif
