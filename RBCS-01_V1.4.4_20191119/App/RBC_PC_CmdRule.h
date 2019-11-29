#ifndef _RBC_PC_CMDRULE_H
#define _RBC_PC_CMDRULE_H

#include "stm32f10x.h"


void PC_Ack(void);
void RBCtoPC_Cmd_AA(void);
void RBCtoPC_Cmd_00(void);
void RBCtoPC_Cmd_FF(void);
void PC_Cmd_Analy(void);
void PC_Val_Analy(void);


void wr_cmd_array_6(uint8_t cmd,uint8_t data_class,uint8_t data1,uint8_t data23);
void RBCtoPC_Cmd_6(void);
void RBCtoPC_Cmd_61(void);

void  FS_GasYes(uint8_t i);
void  FS_GasNo(uint8_t i);
void  FS_GasNo_None(uint8_t i);
void  FS_GasYes_None(uint8_t i);

void FS_INT_ON(void);
void FS_INT_OK(void);
void FS_INT_ERR(void);

void FS_READY(void);
void FS_ZERO_ERRO(void);
void FS_TEST_ERRO(void);
void FS_CO2_ERRO(void);

void FS_RUN_ON(void);// 进行中
void FS_RUN_OK(void);// 完成
void FS_RUN_ERR(void) ;

void FS_RUN_step1_ON(void);//进行中
void FS_RUN_step1_OK(void);// 完成
void FS_RUN_step1_ERR(void);//ERRO

void FS_RUN_step2_ON(void);//进行中
void FS_RUN_step2_OK(void);// 完成
void FS_RUN_step2_ERR(void);//ERRO

void FS_RUN_step3_ON(void);//进行中
void FS_RUN_step3_OK(void);// 完成
void FS_RUN_step3_ERR(void);//ERRO

void FS_RUN_step4_ON(void);//进行中
void FS_RUN_step4_OK(void);// 完成
void FS_RUN_step4_ERR(void);//ERRO

void FS_QC_ON(void);
void FS_QC_OK(void);
void FS_QC_step1_ON(void);
void FS_QC_step1_OK(void);
void FS_QC_step2_ON(void);
void FS_QC_step2_OK(void);
void FS_QC_step3_ON(void);
void FS_QC_step3_OK(void);
void FS_QC_RS_OK(void);
void FS_QC_RS_ERR(void);
void FS_QC_RS_CRI_OK(void);
void FS_QC_RUN_ERR1(void);
void FS_QC_RUN_ERR2(void);
void FS_QC_BACK(void);
void FS_SampleType(uint8_t type);
void FS_SEX(uint8_t *temp);
void FS_RBC(uint16_t *temp);
void FS_Pco(float *PCO);
void FS_CO2(float *CO2);
void FS_HB(uint16_t *temp);
void FS_RP(uint16_t *temp);
void FS_RA(uint16_t *temp);
void FS_ZERO(int16_t *Zero);

void FS_QUA_ON(void);
void FS_QUA_OK(void);
void FS_QUA_ERR(void);
void FS_CO2_WARNING(void);
void FS_CO2_Fault(void);
void FS_Diff_ZERO(uint16_t *DIFF);
void FS_ZERO_Fault(uint16_t *DIFF);
#endif

































