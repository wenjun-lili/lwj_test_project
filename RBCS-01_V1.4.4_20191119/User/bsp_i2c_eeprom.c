/**
  * @file   bsp_eeprom_24xx.c
	* @brief  ����EEPROM 24xx����ģ��
	* @datail ʵ��24xxϵ��EEPROM�Ķ�д������д��������ҳдģʽ���д��Ч�ʡ�
	* @autor  ��������
	* @version V1.0
	* @date   2016-02-01
  */
/**
	Ӧ��˵�������ʴ���EEPROMǰ�����ȵ���һ�� bsp_InitI2C()�������ú�I2C��ص�GPIO.
  */
#include "bsp_i2c_eeprom.h"
#include "bsp_i2c_gpio.h"
#include "stdio.h"
#include "utility.h"

/**
  * @brief  I2CEEPROM_Init(void)
	* @datail eeprom��ʼ��
	* @param  none
	* @return none
	*/
void I2CEEPROM_Init(void)
{
    if (ee_CheckOk() == 0)
    {
//        printf("error");
        while (1);	/* �ȴ� */
    }
}
/**
  * @brief  ee_CheckOk(void)
	* @datail �жϴ���EERPOM�Ƿ�����
	* @param  none
  * @return 0��������
            1������
	*/

uint8_t ee_CheckOk(void)
{
    if (i2c_CheckDevice(EE_DEV_ADDR) == 0)
    {
        return 1;
    }
    else
    {
        /* ʧ�ܺ��мǷ���I2C����ֹͣ�ź� */
        i2c_Stop();
        return 0;
    }
}
/**
  * @brief  ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
	* @datail �Ӵ���EEPROMָ����ַ����ʼ��ȡ��������
	* @param  _usAddress : ��ʼ��ַ
  		      _usSize : ���ݳ��ȣ���λΪ�ֽ�
       		  _pReadBuf :��Ŷ��������ݻ�����ָ��
  * @return 0��ʧ��
            1���ɹ�
	*/

uint8_t ee_ReadBytes(uint8_t *_pReadBuf, uint16_t _usAddress, uint16_t _usSize)
{
    uint16_t i;

    /*  ���ô���EEPROM�漴��ȡָ�����У�������ȡ�����ֽ� */

    /* ��1��������I2C���������ź�*/
    i2c_Start();

    /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�ȡ��ַ��0��ʾд��1��ʾ�� */
#if EE_ADDR_A8 == 1
    i2c_SendByte(EE_DEV_ADDR | I2C_WR | ((_usAddress >> 7) & 0x0E));	/* �˴���дָ�� */
#else
    i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */
#endif

    /* ��3 ����ACK  */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM��������Ӧ */
    }

    /*  ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ����24C04���ϣ���ô�˴���Ҫ���������ַ */
    if (EE_ADDR_BYTES == 1)
    {
        i2c_SendByte((uint8_t)_usAddress);
        if (i2c_WaitAck() != 0)
        {
            goto cmd_fail;	/* EEPROM????? */
        }
    }
    else
    {
        i2c_SendByte(_usAddress >> 8);
        if (i2c_WaitAck() != 0)
        {
            goto cmd_fail;	/* EEPROM������Ӧ�� */
        }

        i2c_SendByte(_usAddress);
        if (i2c_WaitAck() != 0)
        {
            goto cmd_fail;	/* EEPROM������Ӧ�� */
        }
    }

    /* ��6������������I2C���ߡ����濪ʼ��ȡ���� */
    i2c_Start();

    /*  ��7������������ֽڣ���7bit�ǵ�ַ ,bit0�Ƕ�д����λ��0��ʾд��1��ʾ��*/
#if EE_ADDR_A8 == 1
    i2c_SendByte(EE_DEV_ADDR | I2C_RD | ((_usAddress >> 7) & 0x0E));	/* �˴���дָ�� */
#else
    i2c_SendByte(EE_DEV_ADDR | I2C_RD);	/* �˴���дָ�� */
#endif

    /* ��8��������ACk */
    if (i2c_WaitAck() != 0)
    {
        goto cmd_fail;	/* EEPROM��������Ӧ */
    }

    /* ��9����ѭ����ȡ���� */
    for (i = 0; i < _usSize; i++)
    {
        _pReadBuf[i] = i2c_ReadByte();	/* ��1���ֽ� */

        /* ÿ����1���ֽں���Ҫ����ACK�����һ���ֽڲ���ҪAck,��NAck*/
        if (i != _usSize - 1)
        {
            i2c_Ack();	/* �м��ֽڶ����CPU����ACK�ź�(����SDA=0)*/
        }
        else
        {
            i2c_NAck();	/* ���1���ֽڶ����cpu ����NACK�źţ�����SDA=1��) */
        }
    }
    /* ����I2C����ֹͣ�ź� */
    i2c_Stop();
    return 1;	/* ִ�гɹ� */

cmd_fail: /* ����ִ��ʧ�ܺ��мǷ���ֹͣ�ź� ������Ӱ��I2C�����������豸*/
    /* ����I2C����ֹͣ�ź�  */
    i2c_Stop();
    return 0;
}
/**
  * @brief  ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
	* @datail ����EEPROMָ����ַд���������ݣ�����ҳд����д��Ч��
	* @param  _usAddress : ��ʼ��ַ
  		      _usSize : ���ݳ��ȣ���λΪ�ֽ�
       		  pWriteBuf : ��Ŷ��������ݵĻ�����ָ��
  * @return 0��ʧ��
            1���ɹ�
	*/

uint8_t ee_WriteBytes(uint8_t *_pWriteBuf, uint16_t _usAddress, uint16_t _usSize)
{
    uint16_t i,m;
    uint16_t usAddr;

    /*
    	 д����EEPROM�������������������ȡ�ܶ��ֽڣ�ÿ��д����ֻ����ͬһ��page
       ����24XX02/page size=8
       �򵥵Ĵ�����Ϊ�����ֽ�д����ģʽ��ÿ��д1���ֽڣ������͵�ַ
       Ϊ���������д��Ч�ʣ�����������page Write����
    */

    usAddr = _usAddress;
    for (i = 0; i < _usSize; i++)
    {
        /*  �����͵�һ���ֽڻ�����ҳ���׵�ַʱ����Ҫ���·��������źź͵�ַ*/
        if ((i == 0) || (usAddr & (EE_PAGE_SIZE - 1)) == 0)
        {
            /* ��0������ֹͣ�źţ������ڲ����� */
            i2c_Stop();

            /*  ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ���ɣ�һ��С��10ms CLK

              Ƶ��Ϊ200KHzʱ����ѯ�״δ���Ϊ30��
            */
            for (m = 0; m < 1000; m++)
            {
                /* ��1��������I2C���������ź� */
                i2c_Start();

                /*  ��2�������Ϳ����ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ��*/

#if EE_ADDR_A8 == 1
                i2c_SendByte(EE_DEV_ADDR | I2C_WR | ((usAddr >> 7) & 0x0E));	/* ?????? */
#else
                i2c_SendByte(EE_DEV_ADDR | I2C_WR);
#endif

                /*  ����һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
                if (i2c_WaitAck() == 0)
                {
                    break;
                }
            }
            if (m  == 1000)
            {
                goto cmd_fail;	/* EEPROM����д��ʱ */
            }

            /*  ��4���������ֽڵ�ַ��24C02ֻ��256�ֽڣ����1���ֽھ͹��ˣ����24C04���ϣ���ô�˴���Ҫ���������ַ */
            if (EE_ADDR_BYTES == 1)
            {
                i2c_SendByte((uint8_t)usAddr);
                if (i2c_WaitAck() != 0)
                {
                    goto cmd_fail;	/* EEPROM������Ӧ��*/
                }
            }
            else
            {
                i2c_SendByte(usAddr >> 8);
                if (i2c_WaitAck() != 0)
                {
                    goto cmd_fail;	/* EEPROM������Ӧ�� */
                }

                i2c_SendByte(usAddr);
                if (i2c_WaitAck() != 0)
                {
                    goto cmd_fail;	/* EEPROM������Ӧ�� */
                }
            }
        }

        /* ��6������ʼд������ */
        i2c_SendByte(_pWriteBuf[i]);

        /* ����ACK */
        if (i2c_WaitAck() != 0)
        {
            goto cmd_fail;	/* EEPROM������Ӧ�� */
        }


        usAddr++;	/* ��ַ��1 */
    }

    /*  ����ִ�гɹ� ������I2C����ֹͣ�ź�*/
    i2c_Stop();

    /*  ͨ���������Ӧ��ķ�ʽ���ж��ڲ�д�����Ƿ���ɣ�һ��С��10ms

    CLKƵ��Ϊ200KHzʱ����ѯ����Ϊ30������
    */
    for (m = 0; m < 1000; m++)
    {
        /* ��1��������I2C���������ź�*/
        i2c_Start();

        /* ��2������������ֽڣ���7bit�ǵ�ַ��bit0�Ƕ�д����λ��0��ʾд��1��ʾ�� */
#if EE_ADDR_A8 == 1
        i2c_SendByte(EE_DEV_ADDR | I2C_WR | ((_usAddress >> 7) & 0x0E));	/* ?????? */
#else
        i2c_SendByte(EE_DEV_ADDR | I2C_WR);	/* �˴���дָ�� */
#endif

        /*  ��3�� ����һ��ʱ�ӣ��ж������Ƿ���ȷӦ�� */
        if (i2c_WaitAck() == 0)
        {
            break;
        }
    }
    if (m  == 1000)
    {
        goto cmd_fail;	/* EEPROM����д��ʱ */
    }

    return 1;

cmd_fail: /*  ����ִ��ʧ�ܺ��мǷ���ֹͣ�źţ�����Ӱ��I2C�����������豸 */
    /*  ����I2C����ֹͣ�ź� */
    i2c_Stop();

    return 0;
}
