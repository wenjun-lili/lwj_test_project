#include "CD4051.h"

// CD4051相当于一个单刀八掷开关，开关接通哪一通道，由输入的3位地址码ABC来决定。
/*
   INH 是禁止端，当“INH" =1时，各通道均不接通
   A0——A2地址
   IO/O0——I7/O7 输入输出端
   INH禁止端
	 O/I公共输出/输入端
	 VDD正电源
	 VEE模拟信号地

	 INH  C  B  A  选中输出通道
	  0   0  0  0       0    CO_put
		0   0  0  1       1    CO2_out
		0   0  1  0       2    GAS_TEM_IN
		0   0  1  1       3    SES_TEM_IN
*/
void CD4051_Init(void)
{
    /*定义一个 GPIO_InitTypeDef 类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOE GPIOF  时钟初始化*/
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG,ENABLE);

    /* 设置为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*引脚速度为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 |GPIO_Pin_3 |GPIO_Pin_4 ;  // INH INA  INB
    GPIO_Init(GPIOG, &GPIO_InitStructure);

    IN_Select(0); // 关闭所有的通道

}
/**
  * @breif  IN_Select(uint8_t i)
  * @datail 电磁阀选择
  * @param  i:这里的0 表示INH=1，关闭所有的通道
              这里的1 表示 选中CO的模拟量信号接入ADS7816
              这里的2 表示 选中CO2的模拟量信号接入ADS7816
  */

void IN_Select(uint8_t i)
{
    switch(i)
    {
    case 0:// 关闭所有通道
        GPIO_SetBits(GPIOG,GPIO_Pin_2);//INH=1
        GPIO_ResetBits(GPIOG,GPIO_Pin_3 | GPIO_Pin_4);// INA INB =00
        break;
    case 1:// 选中通道1，CO_OUT_PUT
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_ResetBits(GPIOG,GPIO_Pin_3 | GPIO_Pin_4);// INA INB =00
        break;
    case 2:// 选中通道2  CO2_OUT
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_SetBits(GPIOG,GPIO_Pin_3 );// INA INB =10
        GPIO_ResetBits(GPIOG,GPIO_Pin_4);
        break;
    case 3:// 选中通道3   GAS_TEM_IN
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_ResetBits(GPIOG,GPIO_Pin_3 );// INA INB =01
        GPIO_SetBits(GPIOG,GPIO_Pin_4);
        break;
    case 4: // 选中通道4   SES_TEM_IN
        GPIO_ResetBits(GPIOG,GPIO_Pin_2);//INH=0
        GPIO_SetBits(GPIOG,GPIO_Pin_3 | GPIO_Pin_4);// INA INB =00
        break;
    default:
        break;
    }

}


