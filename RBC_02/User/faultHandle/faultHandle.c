#include "faultHandle.h"
#include "stm32f10x.h"
#include "globalval.h"
#include "BME280.h"
#include "utility.h"
#include "DWIN.h"
#include "bsp.h"

/**
  * @brief Humidity_Fault(void)
  * @datail �Ը����ʪ��Ԥ��
  * @prarm none
  * @return none
  */
void Humidity_Fault(void)
{
    uint8_t temp[2];
    if(BME_Humidity > HumidityThreshold)
    {
        Delay100ms(10);///�ȴ�1s��һ�β���
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        if(BME_Humidity > HumidityThreshold) ///��һ��ʪ�ȴ���40
        {
            temp[0] = 0x00;
            temp[1] = 0x49;
            Wr_80_Scr(0x03, temp, 2); //����73�����������ʾ����
            while(1)
            {
                if(BtnPressedFlag == 0xAA) //����ȷ�ϼ�
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }
        }
    }
}
