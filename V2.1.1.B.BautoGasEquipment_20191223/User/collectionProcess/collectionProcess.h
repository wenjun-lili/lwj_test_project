#ifndef _COLLECTION_PROCESS_H
#define _COLLECTION_PROCESS_H

#include "stm32f10x.h"

void airCollection(void);//�����ɼ�
void breathCollection(void);//�������ɼ�
void getRespiratoryCycle(void);//�������ڲɼ�
uint16_t rightAve(uint16_t m);
uint16_t leftAve(uint16_t m);
#endif
