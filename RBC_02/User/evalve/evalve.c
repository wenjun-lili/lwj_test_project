#include "./evalve/evalve.h"

/*电磁阀的GPIO口定义
   PB0   V011
	 PB1   V012
	 PE1   V013
	 PE0   V014
	 PB9	 V015
	 PB8	 V016
	 PA8   V017
*/

static void EValve_GPIO_Config( void )
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启GPIOB和GPIOF的外设时钟*/

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
    /*设置引脚模式为通用推挽输出*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*设置引脚速率为50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


    /* 初始化GPIOB引脚 0 1 8 9 */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
    /* 初始化GPIOE引脚 0 1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* 初始化GPIOA引脚 8*/
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /* 关闭所有电磁阀 */
    EValve_All_Close();
}


void EValve_All_Close( void )
{
    GPIO_ResetBits( GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9);
    GPIO_ResetBits( GPIOE, GPIO_Pin_0 | GPIO_Pin_1 );
    GPIO_ResetBits(GPIOA,GPIO_Pin_8);
}

void EValve_All_Open( void )
{
    GPIO_SetBits( GPIOB, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9);
    GPIO_SetBits( GPIOE, GPIO_Pin_0 | GPIO_Pin_1 );
    GPIO_SetBits(GPIOA,GPIO_Pin_8);
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
