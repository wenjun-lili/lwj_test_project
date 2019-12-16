
#include "num2str.h"

/* 数值转换成字符串的缓存变量 */
char Num2StrTmp[16];

/*
	功能：将数值转换成字符串
	参数：数值
  返回值：无
	调用：外部
*/

char *Num2StrI(int val, const char *format)
{
    char i;
    for(i = 0; i < 16; i++)
    {
        Num2StrTmp[i] = '\0';
    }
    sprintf(Num2StrTmp, format, val);


    return (Num2StrTmp);

}

char *Num2StrF(float val, const char *format)
{
    char i;
    for(i = 0; i < 16; i++)
    {
        Num2StrTmp[i] = '\0';
    }
    sprintf(Num2StrTmp, format, val);

//		for(i = 0; i < 16; i++)
//	{
//	  printf("\r\n%c",Num2StrTmp[i]);
//	}
    return (Num2StrTmp);
}



