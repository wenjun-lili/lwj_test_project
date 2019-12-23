#ifndef __I2C_EE_H
#define	__I2C_EE_H


#include "stm32f10x.h"
#include "string.h"
#include <math.h>
#include "utility.h"

/**************************BME280参数定义*****************************************/

#define BME_ADDRESS 0xEC 
#define BME280_RESET_VALUE 0xB6
#define BME280_DIG_T1_LSB_REG                0x88
#define BME280_DIG_T1_MSB_REG                0x89
#define BME280_DIG_T2_LSB_REG                0x8A
#define BME280_DIG_T2_MSB_REG                0x8B
#define BME280_DIG_T3_LSB_REG                0x8C
#define BME280_DIG_T3_MSB_REG                0x8D
#define BME280_DIG_P1_LSB_REG                0x8E
#define BME280_DIG_P1_MSB_REG                0x8F
#define BME280_DIG_P2_LSB_REG                0x90
#define BME280_DIG_P2_MSB_REG                0x91
#define BME280_DIG_P3_LSB_REG                0x92
#define BME280_DIG_P3_MSB_REG                0x93
#define BME280_DIG_P4_LSB_REG                0x94
#define BME280_DIG_P4_MSB_REG                0x95
#define BME280_DIG_P5_LSB_REG                0x96
#define BME280_DIG_P5_MSB_REG                0x97
#define BME280_DIG_P6_LSB_REG                0x98
#define BME280_DIG_P6_MSB_REG                0x99
#define BME280_DIG_P7_LSB_REG                0x9A
#define BME280_DIG_P7_MSB_REG                0x9B
#define BME280_DIG_P8_LSB_REG                0x9C
#define BME280_DIG_P8_MSB_REG                0x9D
#define BME280_DIG_P9_LSB_REG                0x9E
#define BME280_DIG_P9_MSB_REG                0x9F
#define BME280_DIG_H1_REG                    0xA1
#define BME280_DIG_H2_LSB_REG                0xE1
#define BME280_DIG_H2_MSB_REG                0xE2
#define BME280_DIG_H3_REG                    0xE3
#define BME280_DIG_H4_MSB_REG                0xE4
#define BME280_DIG_H4_LSB_REG                0xE5
#define BME280_DIG_H5_MSB_REG                0xE6
#define BME280_DIG_H6_REG                    0xE7

#define BME280_CHIPID_REG                    0xD0  /*Chip ID Register */
#define BME280_RESET_REG                     0xE0  /*Softreset Register */
#define BME280_CTRLHUM_REG                   0xF2  /*Ctrl humidity Register*/
#define BME280_STATUS_REG                    0xF3  /*Status Register */
#define BME280_CTRLMEAS_REG                  0xF4  /*Ctrl Measure Register */
#define BME280_CONFIG_REG                    0xF5  /*Configuration Register */
#define BME280_PRESSURE_MSB_REG              0xF7  /*Pressure MSB Register */
#define BME280_PRESSURE_LSB_REG              0xF8  /*Pressure LSB Register */
#define BME280_PRESSURE_XLSB_REG             0xF9  /*Pressure XLSB Register */
#define BME280_TEMPERATURE_MSB_REG           0xFA  /*Temperature MSB Reg */
#define BME280_TEMPERATURE_LSB_REG           0xFB  /*Temperature LSB Reg */
#define BME280_TEMPERATURE_XLSB_REG          0xFC  /*Temperature XLSB Reg */
#define BME280_HUMIDITY_MSB_REG              0xFD  /*humidity MSB Reg */
#define BME280_HUMIDITY_LSB_REG              0xFE  /*humidity LSB Reg */


typedef enum {
    BME280_T_MODE_SKIP = 0x0,   /*skipped*/
    BME280_T_MODE_1,            /*x1*/
    BME280_T_MODE_2,            /*x2*/
    BME280_T_MODE_3,            /*x4*/
    BME280_T_MODE_4,            /*x8*/
    BME280_T_MODE_5             /*x16*/
} BME280_T_OVERSAMPLING;  //

typedef enum {
    BME280_SLEEP_MODE = 0x00,
    BME280_FORCED_MODE = 0x01,
    BME280_NORMAL_MODE=0x03
} BME280_WORK_MODE;

typedef enum {
    BME280_P_MODE_SKIP = 0x0,   /*skipped*/
    BME280_P_MODE_x1,            /*x1*/
    BME280_P_MODE_x2,            /*x2*/
    BME280_P_MODE_x4,            /*x4*/
    BME280_P_MODE_x8,            /*x8*/
    BME280_P_MODE_x16             /*x16*/
} BME280_P_OVERSAMPLING;

typedef enum {
    BME280_H_MODE_SKIP = 0x0,   /*skipped*/
    BME280_H_MODE_x1,            /*x1*/
    BME280_H_MODE_x2,            /*x2*/
    BME280_H_MODE_x4,            /*x4*/
    BME280_H_MODE_x8,            /*x8*/
    BME280_H_MODE_x16             /*x16*/
} BME280_H_OVERSAMPLING;
/************************************************/
/**@name	          ??????                 */
/************************************************/
typedef enum
{
    BME280_ULTRA_LOW_POWER_MODE = 0x00,
    BME280_LOW_POWER_MODE = 0x01,
    BME280_STANDARD_RESOLUTION_MODE = 0x02,
    BME280_HIGH_RESOLUTION_MODE = 0x03,
    BME280_ULTRA_HIGH_RESOLUTION_MODE = 0x04
} WORKING_MODE;

typedef enum {
    BME280_FILTER_OFF = 0x0,    /*filter off*/
    BME280_FILTER_MODE_1,       /*0.223*ODR*/
    BME280_FILTER_MODE_2,       /*0.092*ODR*/
    BME280_FILTER_MODE_3,       /*0.042*ODR*/
    BME280_FILTER_MODE_4        /*0.021*ODR*/
} BME280_FILTER_COEFFICIENT;

typedef enum {
    BME280_T_SB_0_5MS = 0x00,      /*0.5ms */
    BME280_T_SB_62_5MS= 0x01,      /*62.5ms*/
    BME280_T_SB_125MS = 0x02,      /*125ms */
    BME280_T_SB_250MS = 0x03,      /*250ms */
    BME280_T_SB_500MS = 0x04,      /*500ms */
    BME280_T_SB_1000MS= 0x05,      /*1000ms*/
    BME280_T_SB_2000MS= 0x06,      /*2000ms*/
    BME280_T_SB_4000MS= 0x07,      /*4000ms*/
} BME280_T_SB;

typedef struct {
    u16 dig_T1;/*??T1??*/
    s16 dig_T2;/*??T2??*/
    s16 dig_T3;/*??T3??*/
    u16 dig_P1;/*??P1??*/
    s16 dig_P2;/*??P2??*/
    s16 dig_P3;/*??P3??*/
    s16 dig_P4;/*??P4??*/
    s16 dig_P5;/*??P5??*/
    s16 dig_P6;/*??P6??*/
    s16 dig_P7;/*??P7??*/
    s16 dig_P8;/*??P8??*/
    s16 dig_P9;/*??P9??*/
    s8  dig_H1;/*??H1??*/
    s16 dig_H2;/*??H2??*/
    s8  dig_H3;/*??H3??*/
    s16 dig_H4;/*??H4??*/
    s16 dig_H5;/*??H5??*/
    s8  dig_H6;/*??H6??*/
    s32 t_fine;/*??t_fine??*/
} BME280_calib_param_t;

typedef struct  {
    BME280_calib_param_t calib_param;/*????*/
    u8 chip_id;                      /*???ID*/
    u8 dev_addr;                     /*???IIC??*/
    u8 oversamp_temperature;         /*????*/
    u8 oversamp_pressure;            /*????*/
    u8 oversamp_humidity;            /*????*/

} BME280_t;

/**************************I2C参数定义，I2C1或I2C2********************************/
#define             BME280_I2Cx                                I2C1
#define             BME280_I2C_APBxClock_FUN                   RCC_APB1PeriphClockCmd
#define             BME280_I2C_CLK                             RCC_APB1Periph_I2C1
#define             BME280_I2C_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             BME280_I2C_GPIO_CLK                        RCC_APB2Periph_GPIOB     
#define             BME280_I2C_SCL_PORT                        GPIOB   
#define             BME280_I2C_SCL_PIN                         GPIO_Pin_6
#define             BME280_I2C_SDA_PORT                        GPIOB 
#define             BME280_I2C_SDA_PIN                         GPIO_Pin_7


/*等待超时时间*/
#define I2CT_FLAG_TIMEOUT         ((uint32_t)0x5000)
#define I2CT_LONG_TIMEOUT         ((uint32_t)(4 * I2CT_FLAG_TIMEOUT))


/*信息输出*/
#define EEPROM_DEBUG_ON         0

#define EEPROM_INFO(fmt,arg...)           printf("<<-EEPROM-INFO->> "fmt"\n",##arg)
#define EEPROM_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)
#define EEPROM_DEBUG(fmt,arg...)          do{\
                                          if(EEPROM_DEBUG_ON)\
                                          printf("<<-EEPROM-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)




/***************************BME280相关函数*******************************/
extern void  I2C_BME_Init(void);
extern u8  I2C_BME280_Init(void);
extern u8  BME280_CalibParam(void);//
extern u8  BME280_GetMode(void);
extern u8  BME280_SetMode(BME280_WORK_MODE mode);
extern u8  BME280_SetPowerMode(u8 mode);
extern u8  BME280_SetStandbyDurn(BME280_T_SB v_standby_durn_u8);
extern u8  BME280_GetStandbyDurn(u8* v_standby_durn_u8);
extern u8  BME280_ReadUncompTemperature(s32* un_temp);
extern u8  BME280_ReadUncompPressuree(s32 *un_press);
extern u8  BME280_ReadUncompPressureTemperature(s32 *un_press, s32 *un_temp,u32 *un_humi);
extern s32 BME280_CompensateTemperatureInt32(s32 un_temp);
extern u32 BME280_CompensatePressureInt32(s32 un_press);
extern u32 BME280_CompensateHumidityInt32(u32 un_humi);
extern u8  BME280_ReadPTH(float *press, float *temp, float *humi);
extern u8 BME280_ReadPressure(float *press);
extern void BME280_GPIO_Init(void);

uint32_t I2C_BME280_ByteWrite(u8 pBuffer, u8 WriteAddr);
uint32_t I2C_BME280_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead);
																					

#endif /* __I2C_EE_H */
