
#include "utility.h"

static void delay_1us(void)
{

    uint8_t i;

//	__nop(); 	 //72M时钟频率，那么一个周期是 1/72微秒
//	__nop();// 标准库中提供的宏定义，可以直接使用
//	__nop();// __nop()是内嵌汇编，是个内联函数: 即编译时是复制函数里面的代码而不是调用
//	__nop();//  这样就加快了运行的速度。
//	__nop(); //nop()=1/72M=13.9ns
//	__nop();
//	__nop();
//	__nop();
    for(i=0; i<=8; i++)
    {
        __nop();
    }
    /*这类内核级的内联函数  在MDK环境下是__nop();其函数原型为void__nop(void)
    编译时会在每隔_nop();处生成一个NOP指令。*/
}

//==============================================================================
//Subroutine:	delay_us
//==============================================================================
void Delay1us(unsigned int Counter)
{
    while(Counter--)
    {
        delay_1us();
    }

}

//==============================================================================
//Subroutine:	Delay100us
//==============================================================================
void Delay100us(unsigned int Counter)
{
    while(Counter--)
    {
        Delay1us(100);
    }
}

//==============================================================================
//Subroutine:	Delay1ms
//==============================================================================
//void Delay1ms(unsigned int Counter)
//{
//		unsigned int i;
//
//		for(; Counter !=0; Counter--)
//		{
// 		  i = 10301;
//
//			while(i--);
//		}
//}



void Delay1ms(unsigned int nCount)
{   u16 i=0;
    while(nCount--)
    {
        i=12000;
        while(i--);
    }
}

//==============================================================================
//Subroutine:	Delay10ms
//==============================================================================
void Delay10ms(unsigned int Counter)
{
    while(Counter--)
    {
        Delay1ms(10);
    }
}

//==============================================================================
//Subroutine:	Delay100ms
//==============================================================================
void Delay100ms(unsigned int Counter)
{
    while(Counter--)
    {
        Delay1ms(100);
    }
}

void Delay(unsigned int nCount)
{
    for(; nCount != 0; nCount--);
}


