#include <REGX52.H>

sbit I2C_SCL = P1^7;
sbit I2C_SDA = P1^2;


void Delay_4us(void)
{
volatile unsigned char i=0;
i++,i++,i++,i++,i++,i++,i++,i++;
}

/**
  * @brief  I2C开始
  * @param  无
  * @retval 无
  */
void I2C_Start(void)
{
    I2C_SDA = 1;
	Delay_4us();
	I2C_SCL = 1;
	Delay_4us();
	I2C_SDA = 0;
	Delay_4us();
	I2C_SCL = 0;
	Delay_4us();
}

/**
  * @brief  I2C结束
  * @param  无
  * @retval 无
  */
void I2C_Stop(void)
{
    I2C_SDA = 0;
	Delay_4us();
	I2C_SCL = 1;
	Delay_4us();
	I2C_SDA = 1;
	Delay_4us();
}

/**
  * @brief  I2C发送一个字节
  * @param  byte 要发送的字节
  * @retval 无
  */
void I2C_SendByte(unsigned char Byte)
{
	unsigned char i;
	for(i = 0;i<8;i++)
	{
		I2C_SDA = Byte&(0x80>>i);
		Delay_4us();
		I2C_SCL = 1;
		Delay_4us();
		I2C_SCL = 0;
		Delay_4us();
	}
}

/**
  * @brief  I2C接收一个字节
  * @param  无
  * @retval Data 接收的字节
  */
unsigned char I2C_ReceiveByte()
{
	unsigned char Data = 0;
	unsigned char i;
	I2C_SDA = 1;
	Delay_4us();
	for(i = 0;i<8;i++)
	{
		I2C_SCL = 1;
		Delay_4us();
		if(I2C_SDA) Data |= (0x80>>i);
		Delay_4us();
		I2C_SCL = 0;
		Delay_4us();
	}
	return Data;
}

/**
  * @brief  I2C发送应答
  * @param  Byte 应答
  * @retval 无
  */
void I2C_SendAck(unsigned char Byte)
{
	I2C_SDA = Byte;
	Delay_4us();
	I2C_SCL = 1;
	Delay_4us();
	I2C_SCL = 0;
	Delay_4us();
}

/**
  * @brief  I2C接收应答
  * @param  无
  * @retval Data 接收的应答
  */
unsigned char I2C_ReceiveAck()
{
	unsigned char Data;
	I2C_SDA=1;
	Delay_4us();
	I2C_SCL = 1;
	Delay_4us();
	Data = I2C_SDA;
	Delay_4us();
	I2C_SCL = 0;
	Delay_4us();
	return Data;
}












