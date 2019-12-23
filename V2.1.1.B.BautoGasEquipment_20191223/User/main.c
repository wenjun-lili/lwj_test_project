/**
  ******************************************************************************
  * @file    main.c
  * @version V1.0.0
  * @date    2019-07-29
  * @brief   �Զ�����װ�ó���
  ******************************************************************************
  */
#include "stm32f10x.h"
#include "bsp.h"
#include "globalVar.h"
#include "collectionProcess.h"

/**
  * @brief  ������
  * @param  ��
  * @retval ��
  */
int main(void)
{
    bspInit();//�ײ��ʼ��
//		TIM_SetCompare3(TIM8,0); //����ռ�ձ�,������
		TIM_SetCompare3(TIM8,100); //����ռ�ձ�,������
	  printf("��ɳ�ʼ��\r\n");
    printf("����汾�ţ�V1.0.6-20190927-�ɼ�����\r\n");

    selfInspection();//�Լ�
    printf("�Լ����\r\n");
    printf("��������ռ�ձȣ��ߵ�ƽ������=%d\r\n",PWMDuty);

    flagInit=1;//��ʼ����ɱ�־��1
	
    while(1) 
    {
        switch(F_Key)
        {
		      case 2:
            F_Key=0;
					  printf("��ʼ�ɼ�������\r\n");
            airCollection();//�����������ɼ�����
            break;
		      case 3:
            F_Key=0;
					  printf("��ʼ�ɼ�������\r\n");
            flagLungRun=1;//������������־��1
            breathCollection();//�������ɼ�����
            break;
		      default:
            break;
        }

    }
}
/*********************************************END OF FILE**********************/
