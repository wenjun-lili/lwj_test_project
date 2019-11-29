

#include "./evalve/evalve.h"


//电磁阀的GPIO口定义

// PF15  E1_CON
// PG0   E2_CON
// PG1   E3_CON
// PE11  E4_CON
// PE12  E5_CON
// PE13  E6_CON
// PE14  E7_CON

static void EValve_GPIO_Config( void )
{
		/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

		/*开启GPIOB和GPIOF的外设时钟*/
 
   RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOG, ENABLE);
		/*设置引脚模式为通用推挽输出*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //推挽输出
		/*设置引脚速率为50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	
	
		/* 初始化GPIOE引脚 11 12 13 14 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 ;
	GPIO_Init(GPIOE, &GPIO_InitStructure);
		/* ???GPIOF?? 15 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
		/* 初始化GPIOG引脚 0 1 */                
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* 关闭所有电磁阀 */
	EValve_All_Close();
}


void EValve_All_Close( void )
{
	GPIO_ResetBits( GPIOE, GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14);
	GPIO_ResetBits( GPIOF, GPIO_Pin_15 );
	GPIO_ResetBits( GPIOG, GPIO_Pin_0 | GPIO_Pin_1 );
	
	
 
}

void EValve_Init(void)
{
	EValve_GPIO_Config();
 	EValve_All_Close();
 
		 
  
}

void EV_Open(uint8_t evalveflag)
{
	switch (evalveflag)
	{
		case 1:
			OPEN_VO1(); 
			break;
		case 2:
			OPEN_VO2();
			break;
		case 3:
			OPEN_VO3();
			break;
		case 4:
			OPEN_VO4();
			break;
		case 5:
			OPEN_VO5();
			break;
		case 6:
			OPEN_VO6();
			break;
		case 7:
			OPEN_VO7();
			break;
		default :
			break;
	}

}
void EV_Close(uint8_t evalveflag)
{
	switch (evalveflag)
	{
		case 1:
			CLS_VO1();
			break;
		case 2:
			CLS_VO2();
			break;
		case 3:
			CLS_VO3();
			break;
		case 4:
			CLS_VO4();
			break;
		case 5:
			CLS_VO5();
			break;
		case 6:
			CLS_VO6();
			break;
		case 7:
			CLS_VO7();
			break;
		default :
			break;
	}
}
