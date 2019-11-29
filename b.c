#include <stdio.h>

typedef enum
{
	warm	= 0,
	error	= 1,           
	debug  	= 2,           
}
seeya_debug_level;
#define __DEBUG__  
#ifdef __DEBUG__  
#define seekya_debug(arg,format,...)	if(arg>error)( printf(""__FILE__":[%3d]  "format"", __LINE__, ##__VA_ARGS__)  )
#else  
#define seekya_debug(format,...)  
#endif  

int main(void)
{
	int a = 10;
	unsigned char b= 230;
	printf("b= %d \r\n",b-240);
	seekya_debug(debug,"a= %d \r\n",a);
	return 0;
}