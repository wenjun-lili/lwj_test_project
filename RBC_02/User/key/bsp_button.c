
 
#include "./key/bsp_button.h"	
#include "stm32f10x.h"
#include "globalval.h"
#include <stdio.h>

/*����ÿ������BUTTON_T�ṹ��  ���ڳ�ʼ��ÿ��������״̬*/
static BUTTON_T s_BtnRUN;	 /*���м�*/	  
static BUTTON_T s_BtnPHOTOL;	 /*��ǰ��ͷ*/ 
static BUTTON_T s_BtnDEC;	 /*������*/   
static BUTTON_T s_BtnINC;  /*�Ӱ���*/
static BUTTON_T s_BtnSET;  //"���ü�"
static BUTTON_T s_BtnPHOTOR;	 //"����ͷ"

static KEY_FIFO_T s_Key;		/* ����FIFO����,�ṹ�� */

static void bsp_InitButtonVar(void);
static void bsp_InitButtonHard(void);
static void bsp_DetectButton(BUTTON_T *_pBtn);

 
/*
	���庯���жϰ����Ƿ��£�����ֵ1 ��ʾ���£�0��ʾδ����
*/
static uint8_t IsKeyRUN(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyPHOTOL(void) 	{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDEC(void) 	{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyINC(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_SET) return 0; return 1;}
static uint8_t IsKeySET(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyPHOTOR(void) 	{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == Bit_SET) return 0; return 1;}
  


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButton
*	����˵��: ��ʼ������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitButton(void)
{
	bsp_InitButtonVar();		/* ��ʼ���������� */
	bsp_InitButtonHard();		/* ��ʼ������Ӳ�� */
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_PutKey
*	����˵��: ��1����ֵѹ�밴��FIFO��������������ģ��һ��������
*	��    �Σ�_KeyCode : ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_PutKey(uint8_t _KeyCode)
{
	s_Key.Buf[s_Key.Write] = _KeyCode;

	if (++s_Key.Write  >= KEY_FIFO_SIZE)
	{
		s_Key.Write = 0;
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_GetKey
*	����˵��: �Ӱ���FIFO��������ȡһ����ֵ��
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
uint8_t bsp_GetKey(void)
{
	uint8_t ret;

	if (s_Key.Read == s_Key.Write)
	{
		return KEY_NONE;
	}
	else
	{
		ret = s_Key.Buf[s_Key.Read];
 
		if (++s_Key.Read >= KEY_FIFO_SIZE)
		{
			s_Key.Read = 0;
		}
		return ret;
	}
}

uint8_t MK_KeyValid(uint8_t fifokeyid, uint8_t matchedkeyid)
{
	if(fifokeyid == matchedkeyid)
		return 1;
	else
		return 0;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyState
*	����˵��: ��ȡ������״̬
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_KeyState(uint8_t _ucKeyID)
{
	uint8_t ucState = 0;

	switch (_ucKeyID)
	{
		case RUN_KEY:
			ucState = s_BtnRUN.State;
			break;
		
    case PHOTOL_KEY:
			ucState = s_BtnPHOTOL.State;
			break;

		case DEC_KEY:
			ucState = s_BtnDEC.State;
			break;

		case INC_KEY:
			ucState = s_BtnINC.State;
			break;
    
		case SET_KEY:
			ucState = s_BtnSET.State;
			break;

		case PHOTOR_KEY:
			ucState = s_BtnPHOTOR.State;
			break;
	}

	return ucState;
}

/*
*********************************************************************************************************
*	�� �� ��: MK_ClearKey
*	����˵��: ��հ���FIFO������
*	��    �Σ���
*	�� �� ֵ: ��������
*********************************************************************************************************
*/
void MK_ClearKey(void)
{
	s_Key.Read = s_Key.Write;
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButtonHard
*	����˵��: ��ʼ������Ӳ��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitButtonHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 STM32F103ZE-EK �������߷��䣺
   RUN_Key       PD8 (�͵�ƽ��ʾ����)// RUN��
   PHOTOL_KEY	   PD9 (�͵�ƽ��ʾ����)// L��
	 DEC_KEY       PD10(�͵�ƽ��ʾ����)//"-"��
	 INC_KEY       PD11(�͵�ƽ��ʾ����)//��+����
	 SET_KEY       PD12(�͵�ƽ��ʾ����)// "SET"��
	 PHOTOR_KEY    PD13(�͵�ƽ��ʾ����)//R��
	 
	*/
			
	/* ��1������  GPIOD  GPIOG��ʱ��
   ע�⣺����ط�����һ����ȫ��
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
	
	/* ��2�����������еİ���GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;              
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	 

}
	
/*
*********************************************************************************************************
*	�� �� ��: bsp_InitButtonVar
*	����˵��: ��ʼ����������
*	��    �Σ�strName : ���������ַ���
*			  strDate : ���̷�������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{ 
	uint8_t i;
	/* �԰���FIFO��дָ������ */
	s_Key.Read = 0;
	s_Key.Write = 0;
  
	for(i=0;i<7;i++)
	{
		s_Key.Buf[i]=0;
	}
	
 /* ��ʼ�� RUN ��������  */
	s_BtnRUN.IsKeyDownFunc = IsKeyRUN;		/* �жϰ������µĺ��� */
	s_BtnRUN.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */ 
	s_BtnRUN.LongTime=0;/*����ʱ��*/
	s_BtnRUN.Count =s_BtnRUN.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnRUN.State = 0;	/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnRUN.KeyCodeDown=KEY_DOWN_RUN; /*�������µļ�ֵ����*/
	s_BtnRUN.KeyCodeUp=KEY_UP_RUN;/*��������ļ�ֵ����*/
	s_BtnRUN.KeyCodeLong=0; 	/* �������������µļ�ֵ����*/
	s_BtnRUN.RepeatSpeed=0;/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnRUN.RepeatCount=0;/*����������*/ 
	
 	/* ��ʼ��PHOTOL��������  */
	s_BtnPHOTOL.IsKeyDownFunc = IsKeyPHOTOL;		/* �жϰ������µĺ��� */
	s_BtnPHOTOL.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */ 
	s_BtnPHOTOL.LongTime=0;/*����ʱ��*/
	s_BtnPHOTOL.Count =s_BtnPHOTOL.FilterTime/ 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnPHOTOL.State = 0;	/* ����ȱʡ״̬��0Ϊδ���� */
  s_BtnPHOTOL.KeyCodeDown=KEY_DOWN_PHOTOL; /*�������µļ�ֵ����*/
	s_BtnPHOTOL.KeyCodeUp=KEY_UP_PHOTOL;/*��������ļ�ֵ����*/
	s_BtnPHOTOL.KeyCodeLong=0; 	/* �������������µļ�ֵ����*/
	s_BtnPHOTOL.RepeatSpeed=0;/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnPHOTOL.RepeatCount=0;/*����������*/ 
	
	
 	/* ��ʼ��DEC��������  */
	s_BtnDEC.IsKeyDownFunc = IsKeyDEC;		/* �жϰ������µĺ��� */
  s_BtnDEC.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */ 
  s_BtnDEC.LongTime=20; /*����ʱ��*/
	s_BtnDEC.Count =s_BtnDEC.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnDEC.State = 0;	/* ����ȱʡ״̬��0Ϊδ���� */
  s_BtnDEC.KeyCodeDown=KEY_DOWN_DEC; /*�������µļ�ֵ����*/
	s_BtnDEC.KeyCodeUp=KEY_UP_DEC;/*��������ļ�ֵ����*/
	s_BtnDEC.KeyCodeLong=KEY_LONG_DEC; 	/* �������������µļ�ֵ����*/
	s_BtnDEC.RepeatSpeed=4;/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnDEC.RepeatCount=0;/*����������*/ 


	 	/* ��ʼ��INC��������  */
	s_BtnINC.IsKeyDownFunc = IsKeyINC;		/* �жϰ������µĺ��� */
	s_BtnINC.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */ 
	s_BtnINC.LongTime=20;/*����ʱ��*/
	s_BtnINC.Count =s_BtnINC.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnINC.State = 0;	/* ����ȱʡ״̬��0Ϊδ���� */
  s_BtnINC.KeyCodeDown=KEY_DOWN_INC; /*�������µļ�ֵ����*/
	s_BtnINC.KeyCodeUp=KEY_UP_INC;/*��������ļ�ֵ����*/
	s_BtnINC.KeyCodeLong=KEY_LONG_INC; 	/* �������������µļ�ֵ����*/
	s_BtnINC.RepeatSpeed=4;/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnINC.RepeatCount=0;/*����������*/ 
	
	
	
		/* ��ʼ��SET��������  */
	s_BtnSET.IsKeyDownFunc = IsKeySET;		/* �жϰ������µĺ��� */
	s_BtnSET.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */ 
	s_BtnSET.LongTime=0;/*����ʱ��*/
	s_BtnSET.Count =s_BtnSET.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnSET.State = 0;	/* ����ȱʡ״̬��0Ϊδ���� */
	s_BtnSET.KeyCodeDown=KEY_DOWN_SET; /*�������µļ�ֵ����*/
	s_BtnSET.KeyCodeUp=KEY_UP_SET;/*��������ļ�ֵ����*/
	s_BtnSET.KeyCodeLong=0; 	/* �������������µļ�ֵ����*/
	s_BtnSET.RepeatSpeed=0;/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnSET.RepeatCount=0;/*����������*/
	
		/* ��ʼ��PHOTOR��������  */
	s_BtnPHOTOR.IsKeyDownFunc = IsKeyPHOTOR;		/* �жϰ������µĺ��� */
  s_BtnPHOTOR.FilterTime = BUTTON_FILTER_TIME;		/* �����˲�ʱ�� */ 
	s_BtnPHOTOR.LongTime=0;/*����ʱ��*/
	s_BtnPHOTOR.Count =s_BtnPHOTOR.FilterTime / 2;		/* ����������Ϊ�˲�ʱ���һ�� */
	s_BtnPHOTOR.State = 0;	/* ����ȱʡ״̬��0Ϊδ���� */
  s_BtnPHOTOR.KeyCodeDown=KEY_DOWN_PHOTOR; /*�������µļ�ֵ����*/
	s_BtnPHOTOR.KeyCodeUp=KEY_UP_PHOTOR;/*��������ļ�ֵ����*/
	s_BtnPHOTOR.KeyCodeLong=0; 	/* �������������µļ�ֵ����*/
	s_BtnPHOTOR.RepeatSpeed=0;/* �����������ٶȣ�0��ʾ��֧������ */
	s_BtnPHOTOR.RepeatCount=0;/*����������*/
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_DetectButton
*	����˵��: ���һ��������������״̬�����뱻�����Եĵ��á�
*	��    �Σ������ṹ����ָ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void bsp_DetectButton(BUTTON_T *_pBtn)
{
	/* ���û�г�ʼ�������������򱨴�
	if (_pBtn->IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), _pBtn->IsKeyDownFunc undefine");
	}
	*/

	if (_pBtn->IsKeyDownFunc()) // ��ʾ����
	{
		 if (_pBtn->Count < _pBtn->FilterTime)   
		 {
			  _pBtn->Count = _pBtn->FilterTime;   
		 }
		 else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		 {
			  _pBtn->Count++;
	   }
		 else  // _pBtn->Count > 2 * _pBtn->FilterTime �˲�10ms
		{
			  if (_pBtn->State == 0)
		  {
				 _pBtn->State = 1; // ��ʾ���̰���״̬

				 /*���Ͱ�ť���µ���Ϣ*/
         if(_pBtn->KeyCodeDown>0)		
				 {
				   bsp_PutKey(_pBtn->KeyCodeDown);	
				 }					 
		  }
			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* ���Ͱ�ť�������µ���Ϣ */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* ��ֵ���밴����FIFO */
						bsp_PutKey(_pBtn->KeyCodeLong);						
					}
				}
				else
				{
					if (_pBtn->RepeatSpeed > 0)
					{
						if (++_pBtn->RepeatCount >= _pBtn->RepeatSpeed)
						{
							_pBtn->RepeatCount = 0;
							/* ��������ÿ��10ms����1������ */
							bsp_PutKey(_pBtn->KeyCodeDown);														
						}
					}
				}
			}
		 
		}
	}
 
	else  // �����ͷ�״̬
	{
		if(_pBtn->Count > _pBtn->FilterTime)
	  { 
			 _pBtn->Count = _pBtn->FilterTime;
		}
		else if(_pBtn->Count != 0)
		{
			_pBtn->Count--;
		}
		else 
		{
			if (_pBtn->State == 1)
			{
				_pBtn->State = 0;

				if(_pBtn->KeyCodeUp > 0)
				{
					/* ��ֵ���밴��FIFO */
				 bsp_PutKey(_pBtn->KeyCodeUp);						
				}	 	
	 
			}
		}
	   _pBtn->LongCount = 0;
		 _pBtn->RepeatCount = 0;
		
	}
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_KeyPro
*	����˵��: ������а�����������״̬�����뱻�����Եĵ��á�
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_KeyScan(void)
{
	bsp_DetectButton(&s_BtnRUN);		 
	bsp_DetectButton(&s_BtnPHOTOL);		 
	bsp_DetectButton(&s_BtnDEC);		 
	bsp_DetectButton(&s_BtnINC);	
 	bsp_DetectButton(&s_BtnSET);		 
	bsp_DetectButton(&s_BtnPHOTOR);		 
 		
 
}


#if 1
void MKeyProcess(void)
{
	uint8_t ucKeyCode;		/* �������� */
//	uint8_t ucKeyId;
	
			/* �����˲��ͼ���ɺ�̨systick�жϷ������ʵ�֣�����ֻ��Ҫ����bsp_GetKey��ȡ��ֵ���ɡ� */
			ucKeyCode = bsp_GetKey();	/* ��ȡ��ֵ, �޼�����ʱ���� KEY_NONE = 0 */
			if (ucKeyCode != KEY_NONE)
			{
				   if(ucKeyCode==KEY_UP_RUN)
					 {
 							BtnPressedFlag = 0; 
					 }
					 else if(ucKeyCode==KEY_UP_PHOTOL)
					 {
						  BtnPressedFlag = 1;
					 }
				   else if(ucKeyCode==KEY_UP_DEC)
					 {
						  BtnPressedFlag = 2;
					 }
					  else if(ucKeyCode==KEY_UP_INC)
					 {
						  BtnPressedFlag = 3;
					 }
					  else if(ucKeyCode==KEY_UP_SET)
					 {
						  BtnPressedFlag = 4;
					 }
					 	  else if(ucKeyCode==KEY_UP_PHOTOR)
					 {
						  BtnPressedFlag = 5;
					 }
					 	  else if(ucKeyCode==KEY_DOWN_DEC)
					 {
						  BtnPressedFlag = 6;
					 }
					 else if(ucKeyCode==KEY_DOWN_INC)
					 {
						  BtnPressedFlag = 7;
					 }
			 	 
				}
		 
			}	 
 
#endif 
