/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0.0
  * @date    2019-07-29
  * @brief   自动采气装置程序
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "bsp.h"
#include "globalVar.h"
#include "collectionProcess.h"

/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
    bspInit();//底层初始化
//		TIM_SetCompare3(TIM8,0); //设置占空比,打开气泵
		TIM_SetCompare3(TIM8,100); //设置占空比,打开气泵
	  printf("完成初始化\r\n");
    printf("程序版本号：V1.0.6-20190927-采集周期\r\n");

    selfInspection();//自检
    printf("自检完成\r\n");
    printf("呼气采样占空比（高电平比例）=%d\r\n",PWMDuty);

    flagInit=1;//初始化完成标志置1
	
    while(1) 
    {
        switch(F_Key)
        {
		      case 2:
            F_Key=0;
					  printf("开始采集环境气\r\n");
            airCollection();//环境本底气采集流程
            break;
		      case 3:
            F_Key=0;
					  printf("开始采集肺泡气\r\n");
            flagLungRun=1;//肺泡气采气标志置1
            breathCollection();//肺泡气采集流程
            break;
		      default:
            break;
        }

    }
}
/*********************************************END OF FILE**********************/
