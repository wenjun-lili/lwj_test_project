#ifndef _T6963C_PRORT_H_
#define _T6963C_PRORT_H_
#include "stm32f10x.h"

void T6963C_WriteCmd(uint8_t _ucRegAddr);
uint8_t T6963C_ReadStatus(void);
void T6963C_WriteData(uint8_t _ucRegValue);
uint8_t T6963C_ReadData(void);

void StatusRW_Check(void);
void CheckAutoWrite(void);
void CheckScreen(void);

uint8_t LCD_TestStatus(uint8_t bitMatch);

void LcmWriteData(uint8_t uData);
void LcmWriteCommand(uint8_t Command);
void LcmWriteCommandWith1Par(uint8_t Parameter,uint8_t Command);
void LcmWriteCommandWith2Par(uint8_t Parameter1,uint8_t Parameter2,uint8_t Command);
#endif



