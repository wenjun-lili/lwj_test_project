#include "ADS7816.h"
#include "utility.h"
#include "stdio.h"

/**
  *@breif  ADS7816测试引脚配置
	*@detail PF11 ： ADC :ADS7816的片选信号 低电平有效
           PF12 ： DOUT：ADS7816 AD采样的数据————> STM32
           PF5  ： DCLOCK：同步时钟信号
	*@param  none
	*@retval none
	*/
void ADS7816_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_11 ;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOF, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;

    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_SetBits( GPIOF, GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12);// SPI 三根线都置高
}




/*　CS 有效后，过1.5至2个时钟周期，Dout使能并输出一个时钟周期的低电平的无效数据位，
此后的12个时钟周期中吗，每个DCLK的下降沿Dout输出对应位的有效数据位*/
static	int ADC_TIMingDiagrams(void)
{
    uint8_t i;
    uint16_t adc=0;

    ADCS_H; //CS设置成高 ，封闭ADS7816

    ADCS_L; // 启动ADS7816
    DCLOCK_L;
    Delay1us(1);//12us
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    DCLOCK_H;
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    Delay1us(1);
    DCLOCK_L;     // 第一个下降沿
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    Delay1us(1);
    DCLOCK_H;
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    Delay1us(1);
    DCLOCK_L;	    // 第二个下降沿
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();

    Delay1us(1);

    for(i=0; i<12; i++)
    {
        DCLOCK_H;
        Delay1us(1);
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();

        DCLOCK_L; // 第三个脉冲下降沿
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();

        Delay1us(1);
        adc=(adc<<1)|( ADS7816_DOUT_READ());



    }
    ADCS_H; // 封闭ADS7816，
    DCLOCK_H;
    Delay1us(30);
    DCLOCK_L;
//			printf("AD shuzhi=%d\r\n",adc);

    return(adc);

}

void ADC(uint16_t *p)
{
    uint16_t i;
    uint32_t ADS12=0;
    for(i=0; i<5000; i++)
    {
        ADS12+=ADC_TIMingDiagrams();
    }
    *p=ADS12/5000;
}



