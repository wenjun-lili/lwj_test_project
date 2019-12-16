#include "ADS7816.h"
#include "utility.h"
#include "stdio.h"

/**
  *@breif  ADS7816������������
	*@detail PF11 �� ADC :ADS7816��Ƭѡ�ź� �͵�ƽ��Ч
           PF12 �� DOUT��ADS7816 AD���������ݡ�������> STM32
           PF5  �� DCLOCK��ͬ��ʱ���ź�
	*@param  none
	*@retval none
	*/
void ADS7816_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |GPIO_Pin_11 ;

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_Init(GPIOF, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;

    GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IN_FLOATING;

    GPIO_Init(GPIOF, &GPIO_InitStructure);

    GPIO_SetBits( GPIOF, GPIO_Pin_5 | GPIO_Pin_11 | GPIO_Pin_12);// SPI �����߶��ø�
}




/*��CS ��Ч�󣬹�1.5��2��ʱ�����ڣ�Doutʹ�ܲ����һ��ʱ�����ڵĵ͵�ƽ����Ч����λ��
�˺��12��ʱ����������ÿ��DCLK���½���Dout�����Ӧλ����Ч����λ*/
static	int ADC_TIMingDiagrams(void)
{
    uint8_t i;
    uint16_t adc=0;

    ADCS_H; //CS���óɸ� �����ADS7816

    ADCS_L; // ����ADS7816
    DCLOCK_L;
    Delay1us(1);//12us
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    DCLOCK_H;
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    Delay1us(1);
    DCLOCK_L;     // ��һ���½���
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    Delay1us(1);
    DCLOCK_H;
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    Delay1us(1);
    DCLOCK_L;	    // �ڶ����½���
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();
    __nop();

    Delay1us(1);

    for(i=0; i<12; i++)
    {
        DCLOCK_H;
        Delay1us(1);
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();

        DCLOCK_L; // �����������½���
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();
        __nop();

        Delay1us(1);
        adc=(adc<<1)|( ADS7816_DOUT_READ());



    }
    ADCS_H; // ���ADS7816��
    DCLOCK_H;
    Delay1us(30);
    DCLOCK_L;
//			printf("AD shuzhi=%d\r\n",adc);

    return(adc);

}

void ADC(uint16_t *p)
{
    uint16_t i;
    uint32_t ADS12=0;
    for(i=0; i<5000; i++)
    {
        ADS12+=ADC_TIMingDiagrams();
    }
    *p=ADS12/5000;
}


