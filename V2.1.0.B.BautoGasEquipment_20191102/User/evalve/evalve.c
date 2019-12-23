#include "evalve.h"
//电磁阀的GPIO口定义

// PC7  V01
// PC6  V02


static void evalveConfig( void )
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOC的外设时钟*/
 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
		/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
		/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	
	
		/* 初始化GPIOC引脚7-12 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* 关闭所有电磁阀 */
	evalveAllClose();
}

/**
  *@breif： evalveAllClose
  *@datail：关闭所有电磁阀
  *@param:  void
  *@retval: void
**/
void evalveAllClose( void )
{
	GPIO_ResetBits( GPIOC, GPIO_Pin_6 | GPIO_Pin_7);
}

/**
  *@breif： evalveInit
  *@datail：电磁阀初始化
  *@param:  void
  *@retval: void
**/
void evalveInit(void)
{
	evalveConfig();
 	evalveAllClose();  
}

/**
  *@breif： evalveOpen
  *@datail：打开某个三通电磁阀，通电1和2通，断电2和3通
  *@param:  evalveFlag：某个电磁阀
  *@retval: void
**/
void evalveOpen(uint8_t evalveFlag)
{
	switch (evalveFlag)
	{
		case 1:
			OPEN_VO1(); 
			break;
		case 2:
			OPEN_VO2();
			break;
		default:
			break;
	}

}
/**
  *@breif： evalveClose
  *@datail：关闭某个电磁阀
  *@param:  evalveFlag：某个电磁阀
  *@retval: void
**/
void evalveClose(uint8_t evalveFlag)
{
	switch (evalveFlag)
	{
		case 1:
			CLS_VO1();
			break;
		case 2:
			CLS_VO2();
			break;
		default:
			break;
	}
}
