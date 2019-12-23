/**
  ******************************************************************************
  * @file    bsp_i2c_ee.c
  * @version V1.0
	* @breif   硬件I2C
  ******************************************************************************
  ******************************************************************************
  */ 

#include "./i2c/bsp_i2c_ee.h"
#include "./usart/bsp_usart.h"		

/* STM32 I2C 模式 */
#define I2C_Speed              100000  //*

/* 这个地址只要与STM32外挂的I2C器件地址不一样即可 */
#define I2Cx_OWN_ADDRESS7      0X0A   

static BME280_t p_BME280;//校准参数

uint16_t EEPROM_ADDRESS;

static __IO uint32_t  I2CTimeout = I2CT_LONG_TIMEOUT;   

static uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode);

/**
  * @brief  I2C I/O配置
  * @param  无
  * @retval 无
  */
static void I2C_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 

	/* 使能与 I2C 有关的时钟 */
	BME280_I2C_APBxClock_FUN ( BME280_I2C_CLK, ENABLE );
	BME280_I2C_GPIO_APBxClock_FUN ( BME280_I2C_GPIO_CLK, ENABLE );
	   
  /* I2C_SCL、I2C_SDA*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(BME280_I2C_SCL_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;	       // 开漏输出
  GPIO_Init(BME280_I2C_SDA_PORT, &GPIO_InitStructure);	
	
}

/**
  * @brief  I2C 工作模式配置
  * @param  无
  * @retval 无
  */
static void I2C_Mode_Configu(void)
{
  I2C_InitTypeDef  I2C_InitStructure; 

  /* I2C 配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	
	/* 高电平数据稳定，低电平数据变化 SCL 时钟线的占空比 */
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	
  I2C_InitStructure.I2C_OwnAddress1 =I2Cx_OWN_ADDRESS7; 
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable ;
	
	/* I2C的寻址模式 */
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	
	/* 通信速率 */
  I2C_InitStructure.I2C_ClockSpeed = I2C_Speed;
  
	/* I2C 初始化 */
  I2C_Init(BME280_I2Cx, &I2C_InitStructure);
  
	/* 使能 I2C */
  I2C_Cmd(BME280_I2Cx, ENABLE);   
}

/**
  * @brief  I2C 外设(BME280)初始化
  * @param  无
  * @retval 无
  */
void I2C_BME_Init(void)
{

  I2C_GPIO_Config(); 
 
  I2C_Mode_Configu();

  EEPROM_ADDRESS = BME_ADDRESS;

}
/**
  * @brief   写一个字节到I2C bme280中
  * @param   
  *		@arg pBuffer:缓冲区指针
  *		@arg WriteAddr:写地址 
  * @retval  无
  */
uint32_t I2C_BME280_ByteWrite(u8 pBuffer, u8 WriteAddr) 
{
  /* Send STRAT condition */
  I2C_GenerateSTART(BME280_I2Cx, ENABLE);

  I2CTimeout = I2CT_FLAG_TIMEOUT;  
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))  
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(0);
  } 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Send bme280 address for write */
  I2C_Send7bitAddress(BME280_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);
  
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(1);
  }  
  /* Send the EEPROM's internal address to write to */
  I2C_SendData(BME280_I2Cx, WriteAddr);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(2);
  } 
  
  /* Send the byte to be written */
  I2C_SendData(BME280_I2Cx, pBuffer); 
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;  
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
  } 
  
  /* Send STOP condition */
  I2C_GenerateSTOP(BME280_I2Cx, ENABLE);
  
  return 1;
}

/**
  * @brief   从EEPROM里面读取一块数据 
  * @param   
  *		@arg pBuffer:存放从EEPROM读取的数据的缓冲区指针
  *		@arg WriteAddr:接收数据的EEPROM的地址
  *     @arg NumByteToWrite:要从EEPROM读取的字节数
  * @retval  无
  */
uint32_t I2C_BME280_BufferRead(u8* pBuffer, u8 ReadAddr, u16 NumByteToRead)
{  
  
  I2CTimeout = I2CT_LONG_TIMEOUT;
  
  //*((u8 *)0x4001080c) |=0x80; 
  while(I2C_GetFlagStatus(BME280_I2Cx, I2C_FLAG_BUSY))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(9);
   }
  
  /* Send START condition */
  I2C_GenerateSTART(BME280_I2Cx, ENABLE);
  //*((u8 *)0x4001080c) &=~0x80;
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(10);
   }
  
  /* Send EEPROM address for write */
  I2C_Send7bitAddress(BME280_I2Cx, EEPROM_ADDRESS, I2C_Direction_Transmitter);

  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(11);
   }
    
//  /* Clear EV6 by setting again the PE bit */
  I2C_Cmd(BME280_I2Cx, ENABLE);

  /* Send the EEPROM's internal address to write to */
  I2C_SendData(BME280_I2Cx, ReadAddr);  

   
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV8 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(12);
   }
    
  /* Send STRAT condition a second time */  
  I2C_GenerateSTART(BME280_I2Cx, ENABLE);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV5 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(13);
   }
    
  /* Send EEPROM address for read */
  I2C_Send7bitAddress(BME280_I2Cx, EEPROM_ADDRESS, I2C_Direction_Receiver);
  
  I2CTimeout = I2CT_FLAG_TIMEOUT;
  /* Test on EV6 and clear it */
  while(!I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
  {
    if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(14);
   }
  
  /* While there is data to be read */
  while(NumByteToRead)  
  {
    if(NumByteToRead == 1)
    {
      /* Disable Acknowledgement */
      I2C_AcknowledgeConfig(BME280_I2Cx, DISABLE);
      
      /* Send STOP Condition */
      I2C_GenerateSTOP(BME280_I2Cx, ENABLE);
    }

    /* Test on EV7 and clear it */    
    I2CTimeout = I2CT_LONG_TIMEOUT;
    
		while(I2C_CheckEvent(BME280_I2Cx, I2C_EVENT_MASTER_BYTE_RECEIVED)==0)  
		{
			if((I2CTimeout--) == 0) return I2C_TIMEOUT_UserCallback(3);
		} 
    {      
      /* Read a byte from the EEPROM */
      *pBuffer = I2C_ReceiveData(BME280_I2Cx);

      /* Point to the next location where the byte read will be saved */
      pBuffer++; 
      
      /* Decrement the read bytes counter */
      NumByteToRead--;        
    }   
  }

  /* Enable Acknowledgement to be ready for another reception */
  I2C_AcknowledgeConfig(BME280_I2Cx, ENABLE);
  
    return 1;
}
/**
  * @brief  Basic management of the timeout situation.
  * @param  errorCode：错误代码，可以用来定位是哪个环节出错.
  * @retval 返回0，表示IIC读取失败.
  */
static  uint32_t I2C_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* Block communication and all processes */
  EEPROM_ERROR("I2C 等待超时!errorCode = %d",errorCode);
  
  return 0;
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
	  u8 buffer[2];
    while(time<1000)
    {   
			  I2C_BME280_BufferRead(buffer,BME280_CHIPID_REG,1);
        chip_ID=buffer[0];
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
	  if(I2C_BME280_ByteWrite(BME280_RESET_VALUE,BME280_RESET_REG))
		{
		  return 1;
		}
		else
		{
		  return 0;
		}
}
/**
  * @brief  BME280_Init(void)
  * @datail BME280初始化
  * @param  None
  * @retval 0：正常
  *         1：连接不正常
  */

u8 I2C_BME280_Init(void)
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
    I2C_BME_Init();//引脚和模式初始化
    if(BME280_Chack())
		{
			printf("未检测到BME芯片\r\n");
			return 1;//BME280检测芯片
			
		}
    I2C_BME280_ByteWrite(ctrl_hum_reg,BME280_CTRLHUM_REG);
    I2C_BME280_ByteWrite(ctrl_meas_reg,BME280_CTRLMEAS_REG);
    I2C_BME280_ByteWrite(config_reg,BME280_CONFIG_REG);
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
    I2C_BME280_BufferRead(a_data_u8,BME280_DIG_T1_LSB_REG,26);
    I2C_BME280_BufferRead(b_data_u8,BME280_DIG_H2_LSB_REG,7);
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
        res=  I2C_BME280_ByteWrite(Ctrl_Humi_Reg,BME280_CTRLHUM_REG);
        res = I2C_BME280_ByteWrite(v_mode_u8,BME280_CTRLMEAS_REG);
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
	  u8 buffer[2];
    u8 v_data_u8 = 0;
    I2C_BME280_BufferRead(buffer,BME280_CONFIG_REG,1);//读取出寄存器的值
	  v_data_u8 = buffer[0];
    v_data_u8 = ((v_data_u8 & ~0xE0) | ((standby_durn<<5)&0xE0));//高3位
    return I2C_BME280_ByteWrite(v_data_u8,BME280_CONFIG_REG);
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
	  u8 buffer[2];
    u8 res  = 0,v_data_u8 = 0;
    I2C_BME280_BufferRead(buffer,BME280_CONFIG_REG,1);
	  v_data_u8=buffer[0];
	  res=v_data_u8;
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
    I2C_BME280_BufferRead(a_data_u8r,BME280_TEMPERATURE_MSB_REG,3);
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
    I2C_BME280_BufferRead(a_data_u8r,BME280_PRESSURE_MSB_REG,3);
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
    I2C_BME280_BufferRead(a_data_u8,BME280_PRESSURE_MSB_REG,8);
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
  * @brief  BME280_ReadPressure(float *press)
  * @datail 获取真实气压
  * @param  press：真实的气压指针
  * @return res
  * @retval 0：正常
  *         1：不正常
  */
u8 BME280_ReadPressure(float *press)
{
    s32 un_press = 0;
    s32 un_temp = 0;
    u32 un_humi =0;
    u8 res=0;
    res = BME280_ReadUncompPressureTemperature(&un_press,&un_temp,&un_humi);
    /* 读取真实的气压值*/
    *press = BME280_CompensatePressureInt32(un_press)/100.0;
    return res;
}

/*********************************************END OF FILE**********************/

