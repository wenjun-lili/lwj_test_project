
#include "num2str.h"

/* ��ֵת�����ַ����Ļ������ */
char Num2StrTmp[16];

/*
	���ܣ�����ֵת�����ַ���
	��������ֵ
  ����ֵ����
	���ã��ⲿ
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



