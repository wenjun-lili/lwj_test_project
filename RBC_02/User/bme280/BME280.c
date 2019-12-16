/**
  * @file BME280.c
	* @brief BME280传感器API
  */
#include "BME280.h"
#include "string.h"
#include <math.h>
#include "myiic.h"

static BME280_t p_BME280;//校准参数
u8  BME280_Init(void);
u8  BME280_CalibParam(void);//
u8  BME280_GetMode(void);
u8  BME280_SetMode(BME280_WORK_MODE mode);
u8  BME280_SetPowerMode(u8 mode);
u8  BME280_SetStandbyDurn(BME280_T_SB v_standby_durn_u8);
u8  BME280_GetStandbyDurn(u8* v_standby_durn_u8);
u8  BME280_ReadUncompTemperature(s32* un_temp);
u8  BME280_ReadUncompPressuree(s32 *un_press);
u8  BME280_ReadUncompPressureTemperature(s32 *un_press, s32 *un_temp, u32 *un_humi);
s32 BME280_CompensateTemperatureInt32(s32 un_temp);
u32 BME280_CompensatePressureInt32(s32 un_press);
u8  BME280_ReadPTH(float *press, float *temp,float *humi);
u32 BME280_CompensatePressureInt32(s32 un_press);

/**
  * @brief  BME280_GPIO_Init(void)
	* @datail BME280相关IO口初始化
  * @param  None
  * @retval None
  */
void BME280_GPIO_Init(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);                     //失能JTAG
    /*CSB初始化，防止IIC地址错误*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  BME280_Chack(void)
  * @datail 检查BME280连接正常
  * @param  None
  * @retval 0：正常
  *         1：连接不正常
  */
u8 BME280_Chack(void)
{
    u16 time = 0;
    u8 chip_ID = 0;
    while(time<1000)
    {
        chip_ID = BME280_Read_Byte(BME280_CHIPID_REG);
        if(chip_ID==0x60)break;//检测到芯片
        else time++;
        Delay1ms(1);
    }
    if(time==1000)return 1;//未检测到芯片
    else
    {
        p_BME280.chip_id = chip_ID;//记录芯片ID
        return 0;
    }
}
/**
  * @brief  BME280_SetSoftReset(void)
  * @datail BME280软件复位。
  * @param  None
  * @retval 0：正常
  *         1：连接不正常
  */
u8 BME280_SetSoftReset(void)
{
    if(BME280_Write_Byte(BME280_RESET_REG,BME280_RESET_VALUE))return 1;
    else return 0;
}
/**
  * @brief  BME280_Init(void)
  * @datail BME280初始化
  * @param  None
  * @retval 0：正常
  *         1：连接不正常
  */

u8 BME280_Init(void)
{
    uint8_t osrs_t = 1;             //Temperature oversampling x 1
    uint8_t osrs_p = 1;             //Pressure oversampling x 1
    uint8_t osrs_h = 1;             //Humidity oversampling x 1
    uint8_t mode = 3;               //Normal mode
    uint8_t t_sb = 5;               //Tstandby 1000ms
    uint8_t filter = 0;             //Filter off
    uint8_t spi3w_en = 0;           //3-wire SPI Disable
    uint8_t ctrl_meas_reg = (osrs_t << 5) | (osrs_p << 2) | mode;
    uint8_t config_reg    = (t_sb << 5) | (filter << 2) | spi3w_en;
    uint8_t ctrl_hum_reg  = osrs_h;
    BME_IIC_Init();
    if(BME280_Chack())
    {
        return 1;//BME280检测芯片
    }
    BME280_Write_Byte(BME280_CTRLHUM_REG,ctrl_hum_reg);
    BME280_Write_Byte(BME280_CTRLMEAS_REG,ctrl_meas_reg);
    BME280_Write_Byte(BME280_CONFIG_REG,config_reg);
    BME280_CalibParam();
    return 0;
}
/**
  * @brief  BME280_CalibParam(void)
  * @datail BME280校准参数
  * @param  None
  * @retval 0：正常
  *         1：连接不正常
  */
u8 BME280_CalibParam(void)
{
    u8 a_data_u8[26],res = 0;
    u8 b_data_u8[7];
    memset(a_data_u8,0,26*sizeof(u8));
    memset(b_data_u8,0,7*sizeof(u8));
    res =BME280_Read_Len(BME280_DIG_T1_LSB_REG,26,a_data_u8);
    res =BME280_Read_Len(BME280_DIG_H2_LSB_REG,7,b_data_u8);
    p_BME280.calib_param.dig_T1 = (u16)((((u16)((u8)a_data_u8[1]))<<8)|a_data_u8[0]);
    p_BME280.calib_param.dig_T2 = (s16)((((s16)((s8)a_data_u8[3]))<<8)|a_data_u8[2]);
    p_BME280.calib_param.dig_T3 = (s16)((((s16)((s8)a_data_u8[5]))<<8)|a_data_u8[4]);
    p_BME280.calib_param.dig_P1 = (u16)((((u16)((u8)a_data_u8[7]))<<8)|a_data_u8[6]);
    p_BME280.calib_param.dig_P2 = (s16)((((s16)((s8)a_data_u8[9]))<<8)|a_data_u8[8]);
    p_BME280.calib_param.dig_P3 = (s16)((((s16)((s8)a_data_u8[11]))<<8)|a_data_u8[10]);
    p_BME280.calib_param.dig_P4 = (s16)((((s16)((s8)a_data_u8[13]))<<8)|a_data_u8[12]);
    p_BME280.calib_param.dig_P5 = (s16)((((s16)((s8)a_data_u8[15]))<<8)|a_data_u8[14]);
    p_BME280.calib_param.dig_P6 = (s16)((((s16)((s8)a_data_u8[17]))<<8)|a_data_u8[16]);
    p_BME280.calib_param.dig_P7 = (s16)((((s16)((s8)a_data_u8[19]))<<8)|a_data_u8[18]);
    p_BME280.calib_param.dig_P8 = (s16)((((s16)((s8)a_data_u8[21]))<<8)|a_data_u8[20]);
    p_BME280.calib_param.dig_P9 = (s16)((((s16)((s8)a_data_u8[23]))<<8)|a_data_u8[22]);
    p_BME280.calib_param.dig_H1 = (s8)a_data_u8[25];
    p_BME280.calib_param.dig_H2 = (s16)((((s16)((s8)b_data_u8[1]))<<8)|b_data_u8[0]);
    p_BME280.calib_param.dig_H3 = (s8)b_data_u8[2];
    p_BME280.calib_param.dig_H4 = (s16)((((s16)((s8)b_data_u8[3]))<<4)|(0x0F & b_data_u8[4]));
    p_BME280.calib_param.dig_H5 = (s16)((((s16)((s8)b_data_u8[5]))<<4)|(b_data_u8[4]>>4));
    p_BME280.calib_param.dig_H6 = (s8)b_data_u8[6];
    return res;
}
/**
  * @brief  BME280_SetPowerMode(u8 mode)
  * @datail 设置BME280电源工作模式
  * @param  mode：0,1,2,3 ，
						0：SLEEP_MODE，休眠模式
						1OR2：FORCED_MODE，读取一次后进入SLEEP_MODE.
						3：正常工作模式
  * @return res
  * @retval 0：正常
  *         1：连接不正常
  *         2：参数错误
  */
u8 BME280_SetPowerMode(u8 mode)
{
    u8 v_mode_u8 = 0,res = 0;
    u8 Ctrl_Humi_Reg;
    if (mode <= BME280_NORMAL_MODE)
    {
        v_mode_u8 = (p_BME280.oversamp_temperature<<5)+(p_BME280.oversamp_pressure<<2)+mode;
        Ctrl_Humi_Reg=p_BME280.oversamp_humidity;
        res=  BME280_Write_Byte(BME280_CTRLHUM_REG,Ctrl_Humi_Reg);
        res = BME280_Write_Byte(BME280_CTRLMEAS_REG,v_mode_u8);
    } else res = 2;
    return res;
}
/**
  * @brief  BME280_SetStandbyDurn(BME280_T_SB standby_durn)
  * @datail 闲置时长设置，即两次获取温度和气压的间隔时间长度
  * @param  standby_durn：
  *  BME280_T_SB_0_5MS              ：0.5ms
  *  BME280_T_SB_62_5MS             ：62.5ms
  *  BME280_T_SB_125MS              ：125ms
  *  BME280_T_SB_250MS              ：250ms
  *  BME280_T_SB_500MS              ：500ms
  *  BME280_T_SB_1000MS             ：1000ms
  *  BME280_T_SB_2000MS             ：2000ms
  *  BME280_T_SB_4000MS             ：4000ms
  * @return res
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_SetStandbyDurn(BME280_T_SB standby_durn)
{
    u8 v_data_u8 = 0;
    v_data_u8 = BME280_Read_Byte(BME280_CONFIG_REG);//读取出寄存器的值
    v_data_u8 = ((v_data_u8 & ~0xE0) | ((standby_durn<<5)&0xE0));//高3位
    return BME280_Write_Byte(BME280_CONFIG_REG,v_data_u8);
}
/**
  * @brief  获取闲置时长，即两次获取温度和气压的间隔时间长度
  * @param  v_standby_durn_u8：
  *  BME280_T_SB_0_5MS              ：0.5ms
  *  BME280_T_SB_62_5MS             ：62.5ms
  *  BME280_T_SB_125MS              ：125ms
  *  BME280_T_SB_250MS              ：250ms
  *  BME280_T_SB_500MS              ：500ms
  *  BME280_T_SB_1000MS             ：1000ms
  *  BME280_T_SB_2000MS             ：2000ms
  *  BME280_T_SB_4000MS             ：4000ms
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_GetStandbyDurn(u8* v_standby_durn_u8)
{
    u8 res  = 0,v_data_u8 = 0;
    res = v_data_u8 = BME280_Read_Byte(BME280_CONFIG_REG);
    *v_standby_durn_u8 = (v_data_u8>>5);
    return res;
}
/**
  * @brief  BME280_ReadUncompTemperature(s32* un_temp)
  * @datail 获取未补偿温度
  * @param  un_temp：数据指针
  * @return res
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_ReadUncompTemperature(s32* un_temp)
{
    u8 a_data_u8r[3]= {0,0,0},res=0;
    res = BME280_Read_Len(BME280_TEMPERATURE_MSB_REG,3,a_data_u8r);
    *un_temp = (s32)((((u32)(a_data_u8r[0]))<<12)|(((u32)(a_data_u8r[1]))<<4)|((u32)a_data_u8r[2]>>4));
    return res;
}
/**
  * @brief  BME280_ReadUncompPressuree(s32 *un_press)
  * @datail 获取未补偿气压
  * @param  un_temp：数据指针
  * @return res
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_ReadUncompPressuree(s32 *un_press)
{
    u8 a_data_u8r[3]= {0,0,0},res = 0;
    res = BME280_Read_Len(BME280_PRESSURE_MSB_REG,3,a_data_u8r);
    *un_press = (s32)((((u32)(a_data_u8r[0]))<<12)|(((u32)(a_data_u8r[1]))<<4)|((u32)a_data_u8r[2]>>4));
    return res;
}
/**
  * @brief  BME280_ReadUncompPressureTemperature(s32 *un_press, s32 *un_temp, u32 *un_humi)
  * @datail 获取未补偿气压和温度（一起获取，一次读取6个字节数据，比分开读取速度快一丢丢）
  * @param  un_press：未补偿气压数据指针，un_temp：未补偿温度数据指针
  * @return res
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_ReadUncompPressureTemperature(s32 *un_press, s32 *un_temp, u32 *un_humi)
{
    u8 a_data_u8[8] = {0,0,0,0,0,0,0,0},res = 0;
    res = BME280_Read_Len(BME280_PRESSURE_MSB_REG,8,a_data_u8);
    *un_press = (s32)((((u32)(a_data_u8[0]))<<12)|(((u32)(a_data_u8[1]))<<4)|((u32)a_data_u8[2]>>4));/*气压*/
    *un_temp = (s32)((((u32)(a_data_u8[3]))<<12)| (((u32)(a_data_u8[4]))<<4)|((u32)a_data_u8[5]>>4));/* 温度 */
    *un_humi = (s32)((((u32)(a_data_u8[6]))<<8)| ((u32)(a_data_u8[7])));/* 温度 */
    return res;
}
/**
  * @brief  BME280_CompensateTemperatureInt32(s32 un_temp)
  * @datail 获取真实的气压
  * @param  un_temp：未补偿温度数据
  * @return temperature
  * @retval s32：温度值，例如：2255代表22.55 DegC
  *
  */
s32 BME280_CompensateTemperatureInt32(s32 un_temp)
{
    s32 v_x1_u32r = 0;
    s32 v_x2_u32r = 0;
    s32 temperature = 0;
    v_x1_u32r = ((((un_temp>>3)-((s32)p_BME280.calib_param.dig_T1<<1)))*((s32)p_BME280.calib_param.dig_T2))>>11;
    v_x2_u32r = (((((un_temp>>4)-((s32)p_BME280.calib_param.dig_T1))*((un_temp>>4)-((s32)p_BME280.calib_param.dig_T1)))>>12)*((s32)p_BME280.calib_param.dig_T3))>>14;
    p_BME280.calib_param.t_fine = v_x1_u32r + v_x2_u32r;
    temperature = (p_BME280.calib_param.t_fine * 5 + 128)>> 8;
    return temperature;
}
/**
  * @brief  BME280_CompensatePressureInt32(s32 un_press)
  * @datail 获取真实气压
  * @param  un_press：未补偿气压
  * @return v_pressure_u32：真实的气压值
  */
u32 BME280_CompensatePressureInt32(s32 un_press)
{
    s32 v_x1_u32r = 0;
    s32 v_x2_u32r = 0;
    u32 v_pressure_u32 = 0;
    v_x1_u32r = (((s32)p_BME280.calib_param.t_fine)>> 1) - (s32)64000;
    v_x2_u32r = (((v_x1_u32r >> 2)* (v_x1_u32r >> 2))>> 11)* ((s32)p_BME280.calib_param.dig_P6);
    v_x2_u32r = v_x2_u32r + ((v_x1_u32r *((s32)p_BME280.calib_param.dig_P5))<< 1);
    v_x2_u32r = (v_x2_u32r >> 2)+ (((s32)p_BME280.calib_param.dig_P4)<< 16);
    v_x1_u32r = (((p_BME280.calib_param.dig_P3*(((v_x1_u32r>>2)*(v_x1_u32r>>2))>>13))>>3)+((((s32)p_BME280.calib_param.dig_P2)* v_x1_u32r)>>1))>>18;
    v_x1_u32r = ((((32768 + v_x1_u32r))* ((s32)p_BME280.calib_param.dig_P1))>> 15);
    v_pressure_u32 = (((u32)(((s32)1048576) - un_press)- (v_x2_u32r >> 12)))* 3125;
    if (v_pressure_u32 < 0x80000000)
        if (v_x1_u32r != 0)
            v_pressure_u32 = (v_pressure_u32<< 1)/ ((u32)v_x1_u32r);
        else return 0;
    else if (v_x1_u32r != 0)
        v_pressure_u32 = (v_pressure_u32 / (u32)v_x1_u32r) * 2;
    else	return 0;
    v_x1_u32r = (((s32)p_BME280.calib_param.dig_P9) * ((s32)(((v_pressure_u32>> 3)* (v_pressure_u32>> 3))>> 13)))>> 12;
    v_x2_u32r = (((s32)(v_pressure_u32 >>	2))	* ((s32)p_BME280.calib_param.dig_P8))>> 13;
    v_pressure_u32 = (u32)((s32)v_pressure_u32 + ((v_x1_u32r + v_x2_u32r+ p_BME280.calib_param.dig_P7)>> 4));
    return v_pressure_u32;
}
/**
  * @brief  BME280_CompensateHumidityInt32(u32 un_humi)
  * @datail 获取真实湿度
  * @param  un_humidity：未补偿湿度
  * @return 真实的湿度值
  */
u32 BME280_CompensateHumidityInt32(u32 un_humi)
{
    s32 var1;
    s32 var2;
    s32 var3;
    s32 var4;
    s32 var5;
    u32 humidity;
    var1 = p_BME280.calib_param.t_fine - ((s32)76800);
    var2 = (s32)(un_humi * 16384);
    var3 = (s32)(((s32)p_BME280.calib_param.dig_H4) * 1048576);
    var4 = ((s32)p_BME280.calib_param.dig_H5) * var1;
    var5 = (((var2 - var3) - var4) + (s32)16384) / 32768;
    var2 = (var1 * ((s32)p_BME280.calib_param.dig_H6)) / 1024;
    var3 = (var1 * ((s32)p_BME280.calib_param.dig_H3)) / 2048;
    var4 = ((var2 * (var3 + (s32)32768)) / 1024) + (s32)2097152;
    var2 = ((var4 * ((s32)p_BME280.calib_param.dig_H2)) + 8192) / 16384;
    var3 = var5 * var2;
    var4 = ((var3 / 32768) * (var3 / 32768)) / 128;
    var5 = var3 - ((var4 * ((s32)p_BME280.calib_param.dig_H1)) / 16);
    var5 = (var5 < 0 ? 0 : var5);
    var5 = (var5 > 419430400 ? 419430400 : var5);
    humidity = (u32)(var5 / 4096);
    return humidity;
}

/**
  * @brief  BME280_ReadPTH(float *press, float *temp,float *humi)
  * @datail 获取真实气压和温度
  * @param  press：真实的气压指针，temp：真实的温度指针，humi：真实的湿度指针
  * @return res
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_ReadPTH(float *press, float *temp,float *humi)
{
    s32 un_press = 0;
    s32 un_temp = 0;
    u32 un_humi =0;
    u8 res=0;
    res = BME280_ReadUncompPressureTemperature(&un_press,&un_temp,&un_humi);
    /* 读取真实的温度值、气压值和湿度值*/
    *temp = BME280_CompensateTemperatureInt32(un_temp)/100.0;
    *press = BME280_CompensatePressureInt32(un_press)/100.0;
    *humi=BME280_CompensateHumidityInt32(un_humi)/1024.0;//20181214
    return res;
}


/**
  * @brief BME_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
  * @datail BME i2c连续写入
  * @prarm addr：BME器件地址
           reg：寄存器地址
           len：写入数据长度
           buf：缓冲区指针
  * @return 0：正常
            1：错误代码
  */
u8 BME_Write_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
    u8 i;
    BME_IIC_Start();
    BME_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(BME_IIC_Wait_Ack())	//等待应答
    {
        BME_IIC_Stop();
        return 1;
    }
    BME_IIC_Send_Byte(reg);	//写寄存器地址
    BME_IIC_Wait_Ack();		//等待应答
    for(i=0; i<len; i++)
    {
        BME_IIC_Send_Byte(buf[i]);	//发送数据
        if(BME_IIC_Wait_Ack())		//等待ACK
        {
            BME_IIC_Stop();
            return 1;
        }
    }
    BME_IIC_Stop();
    return 0;
}
/**
  * @brief BME_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
  * @datail BME i2c连续读取
  * @prarm addr：BME器件地址
           reg：寄存器地址
           len：读取数据长度
           buf：缓冲区指针
  * @return 0：正常
            1：错误代码
  */
u8 BME_Read_Len(u8 addr,u8 reg,u8 len,u8 *buf)
{
    BME_IIC_Start();
    BME_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(BME_IIC_Wait_Ack())	//等待应答
    {
        BME_IIC_Stop();
        return 1;
    }
    BME_IIC_Send_Byte(reg);	//写寄存器地址
    BME_IIC_Wait_Ack();		//等待应答
    BME_IIC_Start();
    BME_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令
    BME_IIC_Wait_Ack();		//等待应答
    while(len)
    {
        if(len==1)*buf=BME_IIC_Read_Byte(0);//读数据,发送nACK
        else *buf=BME_IIC_Read_Byte(1);		//读数据,发送ACK
        len--;
        buf++;
    }
    BME_IIC_Stop();	//产生一个停止条件
    return 0;
}
/**
  * @brief BME_Write_Byte(u8 addr,u8 reg,u8 data)
  * @datail BME i2c写入一个字节
  * @prarm addr：BME器件地址
           reg：寄存器地址
           data：写入的数据
  * @return 0：正常
            1：错误代码
  */
u8 BME_Write_Byte(u8 addr,u8 reg,u8 data)
{
    BME_IIC_Start();
    BME_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    if(BME_IIC_Wait_Ack())	//等待应答
    {
        BME_IIC_Stop();
        return 1;
    }
    BME_IIC_Send_Byte(reg);	//写寄存器地址
    BME_IIC_Wait_Ack();		//等待应答
    BME_IIC_Send_Byte(data);//发送数据
    if(BME_IIC_Wait_Ack())	//等待ACK
    {
        BME_IIC_Stop();
        return 1;
    }
    BME_IIC_Stop();
    return 0;
}
/**
  * @brief BME_Read_Byte(u8 addr,u8 reg)
  * @datail BME i2c读取一个字节
  * @prarm addr：BME器件地址
           reg：寄存器地址
  * @return 0：正常
            1：错误代码
  */
u8 BME_Read_Byte(u8 addr,u8 reg)
{
    u8 res;
    BME_IIC_Start();
    BME_IIC_Send_Byte((addr<<1)|0);//发送器件地址+写命令
    BME_IIC_Wait_Ack();		//等待应答
    BME_IIC_Send_Byte(reg);	//写寄存器地址
    BME_IIC_Wait_Ack();		//等待应答
    BME_IIC_Start();
    BME_IIC_Send_Byte((addr<<1)|1);//发送器件地址+读命令
    BME_IIC_Wait_Ack();		//等待应答
    res=BME_IIC_Read_Byte(0);//读取数据,发送nACK
    BME_IIC_Stop();			//产生一个停止条件
    return res;
}


