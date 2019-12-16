#include "faultHandle.h"
#include "stm32f10x.h"
#include "globalval.h"
#include "BME280.h"
#include "utility.h"
#include "DWIN.h"
#include "bsp.h"

/**
  * @brief Humidity_Fault(void)
  * @datail 对干燥管湿度预警
  * @prarm none
  * @return none
  */
void Humidity_Fault(void)
{
    uint8_t temp[2];
    if(BME_Humidity > HumidityThreshold)
    {
        Delay100ms(10);///等待1s再一次测试
        BME280_ReadPTH(&BME_Pressure, &BME_Temperature, &BME_Humidity);
        if(BME_Humidity > HumidityThreshold) ///再一次湿度大于40
        {
            temp[0] = 0x00;
            temp[1] = 0x49;
            Wr_80_Scr(0x03, temp, 2); //调到73更换干燥剂提示界面
            while(1)
            {
                if(BtnPressedFlag == 0xAA) //按下确认键
                {
                    BtnPressedFlag = 0;
                    break;
                }
            }
        }
    }
}

