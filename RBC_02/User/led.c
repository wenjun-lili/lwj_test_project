
#include "led.h"
#include "bsp.h"


void led_GPIO_Init(void)
{
    // 定义联机ONLINE      PE6   rbcs-02已删除 20180321
    //与 FAULT            PF13

    /*定义一个 GPIO_InitTypeDef 类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOE GPIOF  时钟初始化*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOF, ENABLE);

    /* 设置为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*引脚速度为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13; //PF13  Fault
    GPIO_Init(GPIOF, &GPIO_InitStructure);


    /* 气袋插入检测接口 LAG1 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//  PF1
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOF,&GPIO_InitStructure);


    /*关闭 ONLINE_L 和FAULT_L 等  KR_L KL_L 指示灯灯不需要程序员设定，它根据限位开关的位置决定*/
    LED_All_Close();  //
}

void LED_All_Close( void )
{
    // 亮灯灭，而左到位和右到位指示等根据限位开关的位置决定
//	GPIO_ResetBits( GPIOE,  GPIO_Pin_6);//  ONLINE

    GPIO_ResetBits( GPIOF, GPIO_Pin_13);// FAULT
}

uint8_t is_gasbag_exit(uint8_t num)
{
    uint8_t ret = 0;

    switch (num)
    {
    case 0:
        ret = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8); //LAG1
        break;
//    case 1:
//        ret = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9);// LAG2
////        break;
////    case 2:
////        ret = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10); //LAG3
//        break;
    default :
        break;
    }

    return ret;
}


void Send_Gas_Com(void)
{

    if (is_gasbag_exit(0)==1) // LAG1 插入
    {
        FS_GasYes(0);
    }
    else                    // LAG1 空闲
    {
        FS_GasNo(0);
    }

//    if (is_gasbag_exit(1)==1)// LAG2 插入
//    {
//        FS_GasYes(1);
//        Delay10ms(6);
//    }
//    else                     // LAG2空闲
//    {
//        FS_GasNo(1);
//        Delay10ms(6);
//    }
//    if (is_gasbag_exit(2) == 1)// LAG3插入
//    {
//        FS_GasYes(2);
//        Delay10ms(6);
//    }
//    else                     // LAG3 空闲
//    {
//        FS_GasNo(2);
//        Delay10ms(6);
//    }
    F_Timeout=0;//通讯超时置0

}
