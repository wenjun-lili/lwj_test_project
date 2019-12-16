#include "./evalve/evalve.h"

/*��ŷ���GPIO�ڶ���
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
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����GPIOB��GPIOF������ʱ��*/

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOE, ENABLE);
    /*��������ģʽΪͨ���������*/
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    /*������������Ϊ50MHz */
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;


    /* ��ʼ��GPIOB���� 0 1 8 9 */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 ;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
    /* ��ʼ��GPIOE���� 0 1*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* ��ʼ��GPIOA���� 8*/
    GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOA,&GPIO_InitStructure);

    /* �ر����е�ŷ� */
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
