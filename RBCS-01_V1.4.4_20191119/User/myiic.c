#include "myiic.h"

//#include "delay.h"

//初始化IIC
void BME_IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB,GPIO_Pin_10|GPIO_Pin_11); 	//PB10,PB11 输出高
}
//产生IIC起始信号
void BME_IIC_Start(void)
{
    BME_SDA_OUT();     //sda线输出
    BME_IIC_SDA=1;
    BME_IIC_SCL=1;
    Delay1us(4);
    BME_IIC_SDA=0;//START:when CLK is high,DATA change form high to low
    Delay1us(4);
    BME_IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void BME_IIC_Stop(void)
{
    BME_SDA_OUT();//sda线输出
    BME_IIC_SCL=0;
    BME_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
    Delay1us(4);
    BME_IIC_SCL=1;
    BME_IIC_SDA=1;//发送I2C总线结束信号
    Delay1us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
u8 BME_IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    BME_SDA_IN();      //SDA设置为输入
    BME_IIC_SDA=1;
    Delay1us(1);
    BME_IIC_SCL=1;
    Delay1us(1);
    while(BME_READ_SDA)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            BME_IIC_Stop();
            return 1;
        }
    }
    BME_IIC_SCL=0;//时钟输出0
    return 0;
}
//产生ACK应答
void BME_IIC_Ack(void)
{
    BME_IIC_SCL=0;
    BME_SDA_OUT();
    BME_IIC_SDA=0;
    Delay1us(2);
    BME_IIC_SCL=1;
    Delay1us(2);
    BME_IIC_SCL=0;
}
//不产生ACK应答
void BME_IIC_NAck(void)
{
    BME_IIC_SCL=0;
    BME_SDA_OUT();
    BME_IIC_SDA=1;
    Delay1us(2);
    BME_IIC_SCL=1;
    Delay1us(2);
    BME_IIC_SCL=0;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void BME_IIC_Send_Byte(u8 txd)
{
    u8 t;
    BME_SDA_OUT();
    BME_IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0; t<8; t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
        if((txd&0x80)>>7)
            BME_IIC_SDA=1;
        else
            BME_IIC_SDA=0;
        txd<<=1;
        Delay1us(2);  //对TEA5767这三个延时都是必须的
        BME_IIC_SCL=1;
        Delay1us(2);
        BME_IIC_SCL=0;
        Delay1us(2);
    }
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
u8 BME_IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
    BME_SDA_IN();//SDA设置为输入
    for(i=0; i<8; i++ )
    {
        BME_IIC_SCL=0;
        Delay1us(2);
        BME_IIC_SCL=1;
        receive<<=1;
        if(BME_READ_SDA)receive++;
        Delay1us(1);
    }
    if (!ack)
        BME_IIC_NAck();//发送nACK
    else
        BME_IIC_Ack(); //发送ACK
    return receive;
}



























