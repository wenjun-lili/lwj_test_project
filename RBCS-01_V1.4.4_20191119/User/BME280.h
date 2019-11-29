#ifndef _BME280_h_
#define _BME280_h_
#include "sys.h"
#define BME280_ADDRESS 0xEC

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
/* 在foreced mode下，1s的采样周期，温度和气压使用最低的精度采集并且使用最小的滤波器系数,
 * 数据的采集时间大概在6ms，平均功率为3.27uA。
 * */

/* 在foreced mode下，1s的采样周期, 温度和气压使用最高的精度采集并且使用最大的滤波器系数，
 * 数据的采集时间大概在70ms，平均功率为30uA。
 * */

typedef enum {
    BME280_T_MODE_SKIP = 0x0,   /*skipped*/
    BME280_T_MODE_1,            /*x1*/
    BME280_T_MODE_2,            /*x2*/
    BME280_T_MODE_3,            /*x4*/
    BME280_T_MODE_4,            /*x8*/
    BME280_T_MODE_5             /*x16*/
} BME280_T_OVERSAMPLING;  //过采样

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
/**@name	          工作模式定义                 */
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
    u16 dig_T1;/*校准T1数据*/
    s16 dig_T2;/*校准T2数据*/
    s16 dig_T3;/*校准T3数据*/
    u16 dig_P1;/*校准P1数据*/
    s16 dig_P2;/*校准P2数据*/
    s16 dig_P3;/*校准P3数据*/
    s16 dig_P4;/*校准P4数据*/
    s16 dig_P5;/*校准P5数据*/
    s16 dig_P6;/*校准P6数据*/
    s16 dig_P7;/*校准P7数据*/
    s16 dig_P8;/*校准P8数据*/
    s16 dig_P9;/*校准P9数据*/
    s8  dig_H1;/*校准H1数据*/
    s16 dig_H2;/*校准H2数据*/
    s8  dig_H3;/*校准H3数据*/
    s16 dig_H4;/*校准H4数据*/
    s16 dig_H5;/*校准H5数据*/
    s8  dig_H6;/*校准H6数据*/
    s32 t_fine;/*校准t_fine数据*/
} BME280_calib_param_t;

typedef struct  {
    BME280_calib_param_t calib_param;/*校准数据*/
    u8 chip_id;                      /*传感器ID*/
    u8 dev_addr;                     /*传感器IIC地址*/
    u8 oversamp_temperature;         /*温度采样*/
    u8 oversamp_pressure;            /*气压采样*/
    u8 oversamp_humidity;            /*湿度采样*/

} BME280_t;

#define BME280_SlaveAddr 0x76    //BME280的器件地址

#define BME280_Write_Byte(reg,data)  BME_Write_Byte(BME280_SlaveAddr,reg,data)
#define BME280_Read_Byte(reg) BME_Read_Byte(BME280_SlaveAddr,reg)
#define BME280_Read_Len(reg,len,buf) BME_Read_Len(BME280_SlaveAddr,reg,len,buf)

/*函数*/
extern u8  BME280_Init(void);
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
extern void BME280_GPIO_Init(void);

u8 BME_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 BME_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf);
u8 BME_Write_Byte(u8 addr,u8 reg,u8 data);
u8 BME_Read_Byte(u8 addr,u8 reg);
#endif



