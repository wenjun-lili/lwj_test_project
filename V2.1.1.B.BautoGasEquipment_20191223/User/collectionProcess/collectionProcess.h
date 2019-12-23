#ifndef _COLLECTION_PROCESS_H
#define _COLLECTION_PROCESS_H

#include "stm32f10x.h"

void airCollection(void);//空气采集
void breathCollection(void);//肺泡气采集
void getRespiratoryCycle(void);//呼气周期采集
uint16_t rightAve(uint16_t m);
uint16_t leftAve(uint16_t m);
#endif
