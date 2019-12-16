#include "sd_fat_app.h"
#include "ff.h"
#include "num2str.h"
#include <string.h>


FATFS fs;													/* FatFs文件系统对象 */
FIL fnew;													/* 文件对象 */
FRESULT res_flash;                /* 文件操作结果 */
UINT fnum;            					  /* 文件成功读写数量 */

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
    /* SD 中断优先级设置 */
    SDIO_NVIC_Configuration();

    /* 创建磁盘驱动号0*/
    res_flash = f_mount(&fs,"0:",1); //1:代表立即创建
}


void SD_SAVE(void)
{

    res_flash = f_mount(&fs,"0:",1);
    res_flash = f_open(&fnew, "0:红细胞寿命测试仪软件.txt",FA_OPEN_ALWAYS | FA_WRITE );
    if ( res_flash == FR_OK )
    {
        //res_flash=f_write(&fnew,"红细胞寿命测定仪测量结果\r\n",sizeof("红细胞寿命测定仪测量结果\r\n"),&fnum);
    }
    /* 不再读写，关闭文件 */
    f_close(&fnew);

    memset(Write_Receive,0,255);
    memcpy(Write_Receive,RBC_pc_ComR+3,RBC_pc_ComR[2]);
    Write_Receive[RBC_pc_ComR[2]]=0x0D;
    Write_Receive[RBC_pc_ComR[2]+1]=0x0A;

    /* 打开文件，每次都以新建的形式打开，属性为可写 */

    f_mount(&fs,"0:",1); //在Sd卡的0逻辑
// 	printf("\r\n****** 即将进行文件写入测试... ******\r\n");
    res_flash = f_open(&fnew, "0:红细胞寿命测试仪软件.txt",FA_OPEN_ALWAYS | FA_WRITE );
    if ( res_flash == FR_OK )
    {
//		printf("》打开/创建FatFs读写测试文件.txt文件成功，向文件写入数据。\r\n");
        /* 将指定存储区内容写入到文件内 */
////		res_flash=f_lseek(&fnew,fnew.fsize);  // 把指针指向文件末端
//////		res_flash=f_write(&fnew,Num2StrI(i,"%d"),sizeof(Num2StrTmp),&fnum);
////		 res_flash=f_write(&fnew,Num2StrI(i,"%d"),10,&fnum);

        res_flash=f_lseek(&fnew,fnew.fsize);  // 把指针指向文件末端
//		res_flash=f_write(&fnew,WriteBuffer,sizeof(WriteBuffer),&fnum);
        res_flash=f_write(&fnew,Write_Receive,RBC_pc_ComR[2]+2,&fnum);

        if(res_flash==FR_OK)
        {
//      printf("》文件写入成功，写入字节数据：%d\n",fnum);

//			printf("》向文件写入的数据为：\r\n%s\r\n",Write_Receive);
        }
        else
        {
//      printf("！！文件写入失败：(%d)\n",res_flash);
        }
        /* 不再读写，关闭文件 */
        f_close(&fnew);
    }
    else
    {
//		printf("！！打开/创建文件失败。\r\n");
    }

    f_mount(NULL,"0:",1);
}
