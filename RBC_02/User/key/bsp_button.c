
 
#include "./key/bsp_button.h"	
#include "stm32f10x.h"
#include "globalval.h"
#include <stdio.h>

/*定义每个按键BUTTON_T结构体  用于初始化每个按键的状态*/
static BUTTON_T s_BtnRUN;	 /*运行键*/	  
static BUTTON_T s_BtnPHOTOL;	 /*向前箭头*/ 
static BUTTON_T s_BtnDEC;	 /*减按键*/   
static BUTTON_T s_BtnINC;  /*加按键*/
static BUTTON_T s_BtnSET;  //"设置键"
static BUTTON_T s_BtnPHOTOR;	 //"向后箭头"

static KEY_FIFO_T s_Key;		/* 按键FIFO变量,结构体 */

static void bsp_InitButtonVar(void);
static void bsp_InitButtonHard(void);
static void bsp_DetectButton(BUTTON_T *_pBtn);

 
/*
	定义函数判断按键是否按下，返回值1 表示按下，0表示未按下
*/
static uint8_t IsKeyRUN(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_8) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyPHOTOL(void) 	{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_9) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyDEC(void) 	{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_10) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyINC(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_11) == Bit_SET) return 0; return 1;}
static uint8_t IsKeySET(void) 		{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_12) == Bit_SET) return 0; return 1;}
static uint8_t IsKeyPHOTOR(void) 	{if (GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_13) == Bit_SET) return 0; return 1;}
  


/*
*********************************************************************************************************
*	函 数 名: bsp_InitButton
*	功能说明: 初始化按键
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_InitButton(void)
{
	bsp_InitButtonVar();		/* 初始化按键变量 */
	bsp_InitButtonHard();		/* 初始化按键硬件 */
}

/*
*********************************************************************************************************
*	函 数 名: bsp_PutKey
*	功能说明: 将1个键值压入按键FIFO缓冲区。可用于模拟一个按键。
*	形    参：_KeyCode : 按键代码
*	返 回 值: 无
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
*	函 数 名: bsp_GetKey
*	功能说明: 从按键FIFO缓冲区读取一个键值。
*	形    参：无
*	返 回 值: 按键代码
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
*	函 数 名: bsp_KeyState
*	功能说明: 读取按键的状态
*	形    参：无
*	返 回 值: 无
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
*	函 数 名: MK_ClearKey
*	功能说明: 清空按键FIFO缓冲区
*	形    参：无
*	返 回 值: 按键代码
*********************************************************************************************************
*/
void MK_ClearKey(void)
{
	s_Key.Read = s_Key.Write;
}

/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonHard
*	功能说明: 初始化按键硬件
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonHard(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/*
	 STM32F103ZE-EK 按键口线分配：
   RUN_Key       PD8 (低电平表示按下)// RUN键
   PHOTOL_KEY	   PD9 (低电平表示按下)// L键
	 DEC_KEY       PD10(低电平表示按下)//"-"键
	 INC_KEY       PD11(低电平表示按下)//“+”键
	 SET_KEY       PD12(低电平表示按下)// "SET"键
	 PHOTOR_KEY    PD13(低电平表示按下)//R键
	 
	*/
			
	/* 第1步：打开  GPIOD  GPIOG的时钟
   注意：这个地方可以一次性全打开
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
	
	/* 第2步：配置所有的按键GPIO为浮动输入模式(实际上CPU复位后就是输入状态) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10| GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;              
	GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOD, &GPIO_InitStructure);	 

}
	
/*
*********************************************************************************************************
*	函 数 名: bsp_InitButtonVar
*	功能说明: 初始化按键变量
*	形    参：strName : 例程名称字符串
*			  strDate : 例程发布日期
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_InitButtonVar(void)
{ 
	uint8_t i;
	/* 对按键FIFO读写指针清零 */
	s_Key.Read = 0;
	s_Key.Write = 0;
  
	for(i=0;i<7;i++)
	{
		s_Key.Buf[i]=0;
	}
	
 /* 初始化 RUN 按键变量  */
	s_BtnRUN.IsKeyDownFunc = IsKeyRUN;		/* 判断按键按下的函数 */
	s_BtnRUN.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */ 
	s_BtnRUN.LongTime=0;/*长按时间*/
	s_BtnRUN.Count =s_BtnRUN.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnRUN.State = 0;	/* 按键缺省状态，0为未按下 */
	s_BtnRUN.KeyCodeDown=KEY_DOWN_RUN; /*按键按下的键值代码*/
	s_BtnRUN.KeyCodeUp=KEY_UP_RUN;/*按键弹起的键值代码*/
	s_BtnRUN.KeyCodeLong=0; 	/* 按键被持续按下的键值代码*/
	s_BtnRUN.RepeatSpeed=0;/* 按键连发的速度，0表示不支持连发 */
	s_BtnRUN.RepeatCount=0;/*连发计数器*/ 
	
 	/* 初始化PHOTOL按键变量  */
	s_BtnPHOTOL.IsKeyDownFunc = IsKeyPHOTOL;		/* 判断按键按下的函数 */
	s_BtnPHOTOL.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */ 
	s_BtnPHOTOL.LongTime=0;/*长按时间*/
	s_BtnPHOTOL.Count =s_BtnPHOTOL.FilterTime/ 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnPHOTOL.State = 0;	/* 按键缺省状态，0为未按下 */
  s_BtnPHOTOL.KeyCodeDown=KEY_DOWN_PHOTOL; /*按键按下的键值代码*/
	s_BtnPHOTOL.KeyCodeUp=KEY_UP_PHOTOL;/*按键弹起的键值代码*/
	s_BtnPHOTOL.KeyCodeLong=0; 	/* 按键被持续按下的键值代码*/
	s_BtnPHOTOL.RepeatSpeed=0;/* 按键连发的速度，0表示不支持连发 */
	s_BtnPHOTOL.RepeatCount=0;/*连发计数器*/ 
	
	
 	/* 初始化DEC按键变量  */
	s_BtnDEC.IsKeyDownFunc = IsKeyDEC;		/* 判断按键按下的函数 */
  s_BtnDEC.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */ 
  s_BtnDEC.LongTime=20; /*长按时间*/
	s_BtnDEC.Count =s_BtnDEC.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnDEC.State = 0;	/* 按键缺省状态，0为未按下 */
  s_BtnDEC.KeyCodeDown=KEY_DOWN_DEC; /*按键按下的键值代码*/
	s_BtnDEC.KeyCodeUp=KEY_UP_DEC;/*按键弹起的键值代码*/
	s_BtnDEC.KeyCodeLong=KEY_LONG_DEC; 	/* 按键被持续按下的键值代码*/
	s_BtnDEC.RepeatSpeed=4;/* 按键连发的速度，0表示不支持连发 */
	s_BtnDEC.RepeatCount=0;/*连发计数器*/ 


	 	/* 初始化INC按键变量  */
	s_BtnINC.IsKeyDownFunc = IsKeyINC;		/* 判断按键按下的函数 */
	s_BtnINC.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */ 
	s_BtnINC.LongTime=20;/*长按时间*/
	s_BtnINC.Count =s_BtnINC.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnINC.State = 0;	/* 按键缺省状态，0为未按下 */
  s_BtnINC.KeyCodeDown=KEY_DOWN_INC; /*按键按下的键值代码*/
	s_BtnINC.KeyCodeUp=KEY_UP_INC;/*按键弹起的键值代码*/
	s_BtnINC.KeyCodeLong=KEY_LONG_INC; 	/* 按键被持续按下的键值代码*/
	s_BtnINC.RepeatSpeed=4;/* 按键连发的速度，0表示不支持连发 */
	s_BtnINC.RepeatCount=0;/*连发计数器*/ 
	
	
	
		/* 初始化SET按键变量  */
	s_BtnSET.IsKeyDownFunc = IsKeySET;		/* 判断按键按下的函数 */
	s_BtnSET.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */ 
	s_BtnSET.LongTime=0;/*长按时间*/
	s_BtnSET.Count =s_BtnSET.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnSET.State = 0;	/* 按键缺省状态，0为未按下 */
	s_BtnSET.KeyCodeDown=KEY_DOWN_SET; /*按键按下的键值代码*/
	s_BtnSET.KeyCodeUp=KEY_UP_SET;/*按键弹起的键值代码*/
	s_BtnSET.KeyCodeLong=0; 	/* 按键被持续按下的键值代码*/
	s_BtnSET.RepeatSpeed=0;/* 按键连发的速度，0表示不支持连发 */
	s_BtnSET.RepeatCount=0;/*连发计数器*/
	
		/* 初始化PHOTOR按键变量  */
	s_BtnPHOTOR.IsKeyDownFunc = IsKeyPHOTOR;		/* 判断按键按下的函数 */
  s_BtnPHOTOR.FilterTime = BUTTON_FILTER_TIME;		/* 按键滤波时间 */ 
	s_BtnPHOTOR.LongTime=0;/*长按时间*/
	s_BtnPHOTOR.Count =s_BtnPHOTOR.FilterTime / 2;		/* 计数器设置为滤波时间的一半 */
	s_BtnPHOTOR.State = 0;	/* 按键缺省状态，0为未按下 */
  s_BtnPHOTOR.KeyCodeDown=KEY_DOWN_PHOTOR; /*按键按下的键值代码*/
	s_BtnPHOTOR.KeyCodeUp=KEY_UP_PHOTOR;/*按键弹起的键值代码*/
	s_BtnPHOTOR.KeyCodeLong=0; 	/* 按键被持续按下的键值代码*/
	s_BtnPHOTOR.RepeatSpeed=0;/* 按键连发的速度，0表示不支持连发 */
	s_BtnPHOTOR.RepeatCount=0;/*连发计数器*/
}

/*
*********************************************************************************************************
*	函 数 名: bsp_DetectButton
*	功能说明: 检测一个按键。非阻塞状态，必须被周期性的调用。
*	形    参：按键结构变量指针
*	返 回 值: 无
*********************************************************************************************************
*/
static void bsp_DetectButton(BUTTON_T *_pBtn)
{
	/* 如果没有初始化按键函数，则报错
	if (_pBtn->IsKeyDownFunc == 0)
	{
		printf("Fault : DetectButton(), _pBtn->IsKeyDownFunc undefine");
	}
	*/

	if (_pBtn->IsKeyDownFunc()) // 表示按下
	{
		 if (_pBtn->Count < _pBtn->FilterTime)   
		 {
			  _pBtn->Count = _pBtn->FilterTime;   
		 }
		 else if(_pBtn->Count < 2 * _pBtn->FilterTime)
		 {
			  _pBtn->Count++;
	   }
		 else  // _pBtn->Count > 2 * _pBtn->FilterTime 滤波10ms
		{
			  if (_pBtn->State == 0)
		  {
				 _pBtn->State = 1; // 表示键盘按下状态

				 /*发送按钮按下的消息*/
         if(_pBtn->KeyCodeDown>0)		
				 {
				   bsp_PutKey(_pBtn->KeyCodeDown);	
				 }					 
		  }
			if (_pBtn->LongTime > 0)
			{
				if (_pBtn->LongCount < _pBtn->LongTime)
				{
					/* 发送按钮持续按下的消息 */
					if (++_pBtn->LongCount == _pBtn->LongTime)
					{
						/* 键值放入按键的FIFO */
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
							/* 长按键后，每隔10ms发送1个按键 */
							bsp_PutKey(_pBtn->KeyCodeDown);														
						}
					}
				}
			}
		 
		}
	}
 
	else  // 按键释放状态
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
					/* 键值放入按键FIFO */
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
*	函 数 名: bsp_KeyPro
*	功能说明: 检测所有按键。非阻塞状态，必须被周期性的调用。
*	形    参：无
*	返 回 值: 无
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
	uint8_t ucKeyCode;		/* 按键代码 */
//	uint8_t ucKeyId;
	
			/* 按键滤波和检测由后台systick中断服务程序实现，我们只需要调用bsp_GetKey读取键值即可。 */
			ucKeyCode = bsp_GetKey();	/* 读取键值, 无键按下时返回 KEY_NONE = 0 */
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
