#include "sd_fat_app.h"
#include "ff.h"
#include "num2str.h"
#include <string.h>


FATFS fs;													/* FatFs�ļ�ϵͳ���� */
FIL fnew;													/* �ļ����� */
FRESULT res_flash;                /* �ļ�������� */
UINT fnum;            					  /* �ļ��ɹ���д���� */

/*
 * ???:NVIC_Configuration
 * ??  :SDIO ??????
 * ??  :?
 * ??  :?
 */
void SDIO_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

    NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void sd_fs_init(void)
{
    /* SD �ж����ȼ����� */
    SDIO_NVIC_Configuration();

    /* ��������������0*/
    res_flash = f_mount(&fs,"0:",1); //1:������������
}


void SD_SAVE(void)
{

    res_flash = f_mount(&fs,"0:",1);
    res_flash = f_open(&fnew, "0:��ϸ���������������.txt",FA_OPEN_ALWAYS | FA_WRITE );
    if ( res_flash == FR_OK )
    {
        //res_flash=f_write(&fnew,"��ϸ�������ⶨ�ǲ������\r\n",sizeof("��ϸ�������ⶨ�ǲ������\r\n"),&fnum);
    }
    /* ���ٶ�д���ر��ļ� */
    f_close(&fnew);

    memset(Write_Receive,0,255);
    memcpy(Write_Receive,RBC_pc_ComR+3,RBC_pc_ComR[2]);
    Write_Receive[RBC_pc_ComR[2]]=0x0D;
    Write_Receive[RBC_pc_ComR[2]+1]=0x0A;

    /* ���ļ���ÿ�ζ����½�����ʽ�򿪣�����Ϊ��д */

    f_mount(&fs,"0:",1); //��Sd����0�߼�
// 	printf("\r\n****** ���������ļ�д�����... ******\r\n");
    res_flash = f_open(&fnew, "0:��ϸ���������������.txt",FA_OPEN_ALWAYS | FA_WRITE );
    if ( res_flash == FR_OK )
    {
//		printf("����/����FatFs��д�����ļ�.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");
        /* ��ָ���洢������д�뵽�ļ��� */
////		res_flash=f_lseek(&fnew,fnew.fsize);  // ��ָ��ָ���ļ�ĩ��
//////		res_flash=f_write(&fnew,Num2StrI(i,"%d"),sizeof(Num2StrTmp),&fnum);
////		 res_flash=f_write(&fnew,Num2StrI(i,"%d"),10,&fnum);

        res_flash=f_lseek(&fnew,fnew.fsize);  // ��ָ��ָ���ļ�ĩ��
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        res_flash=f_write(&fnew,Write_Receive,RBC_pc_ComR[2]+2,&fnum);

        if(res_flash==FR_OK)
        {
//      printf("���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fnum);

//			printf("�����ļ�д�������Ϊ��\r\n%s\r\n",Write_Receive);
        }
        else
        {
//      printf("�����ļ�д��ʧ�ܣ�(%d)\n",res_flash);
        }
        /* ���ٶ�д���ر��ļ� */
        f_close(&fnew);
    }
    else
    {
//		printf("������/�����ļ�ʧ�ܡ�\r\n");
    }

    f_mount(NULL,"0:",1);
}
