#include "evalve.h"
//��ŷ���GPIO�ڶ���

// PC7  V01
// PC6  V02


static void evalveConfig( void )
{
		/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

		/*����GPIOC������ʱ��*/
 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);
		/*��������ģʽΪͨ���������*/
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //�������
		/*������������Ϊ50MHz */   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
	
	
		/* ��ʼ��GPIOC����7-12 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	/* �ر����е�ŷ� */
	evalveAllClose();
}

/**
  *@breif�� evalveAllClose
  *@datail���ر����е�ŷ�
  *@param:  void
  *@retval: void
**/
void evalveAllClose( void )
{
	GPIO_ResetBits( GPIOC, GPIO_Pin_6 | GPIO_Pin_7);
}

/**
  *@breif�� evalveInit
  *@datail����ŷ���ʼ��
  *@param:  void
  *@retval: void
**/
void evalveInit(void)
{
	evalveConfig();
 	evalveAllClose();  
}

/**
  *@breif�� evalveOpen
  *@datail����ĳ����ͨ��ŷ���ͨ��1��2ͨ���ϵ�2��3ͨ
  *@param:  evalveFlag��ĳ����ŷ�
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
  *@breif�� evalveClose
  *@datail���ر�ĳ����ŷ�
  *@param:  evalveFlag��ĳ����ŷ�
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
