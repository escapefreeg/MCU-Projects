#include <REGX52.H>
#include <INTRINS.H>

//引脚定义
sbit OneWire_DQ=P2^4;

/**
  * @brief  单总线初始化
  * @param  无
  * @retval 从机响应位，0为响应，1为未响应
  */
unsigned char OneWire_Init(void)
{
	
	unsigned char i,j;
	unsigned char AckBit;
	//ET0 = 0;
	OneWire_DQ=1;
	OneWire_DQ=0;
	i = 6;j = 93;//Delay 500us
	do{while (--j);} while (--i);
	OneWire_DQ=1;
	i = 1;j = 200;//Delay 70us
	do{while (--j);} while (--i);		
	AckBit=OneWire_DQ;
	_nop_();_nop_();_nop_();//Delay 500us
	i = 6;j = 81;
	do{while (--j);} while (--i);
	//ET0 = 1;
	return AckBit;
}

/**
  * @brief  单总线发送一位
  * @param  Bit 要发送的位
  * @retval 无
  */
void OneWire_SendBit(unsigned char Bit)
{
	unsigned char i,j;
	//ET0 = 0;
	OneWire_DQ=0;
	_nop_();_nop_();_nop_();//Delay 10us
	i = 35;while (--i);
	OneWire_DQ=Bit;
	_nop_();_nop_();_nop_();//Delay 50us
	i = 1;j = 133;
	do{while (--j);} while (--i);			
	OneWire_DQ=1;
	//ET0 = 1;
}

/**
  * @brief  单总线接收一位
  * @param  无
  * @retval 读取的位
  */
unsigned char OneWire_ReceiveBit(void)
{
	unsigned char i,j;
	unsigned char Bit;
	//ET0 = 0;
	OneWire_DQ=0;
	i = 22;while (--i);//Delay 5us
	OneWire_DQ=1;
	i = 22;while (--i);//Delay 5us
	Bit=OneWire_DQ;
	_nop_();_nop_();_nop_();//Delay 50us
	i = 1;j = 133;
	do{while (--j);} while (--i);
	//ET0 = 1;	
	return Bit;
}

/**
  * @brief  单总线发送一个字节,先低字节，然后高字节
  * @param  Byte 要发送的字节
  * @retval 无
  */
void OneWire_SendByte(unsigned char Byte)
{
	unsigned char i;
	//ET0 = 0;
	for(i=0;i<8;i++)
	{
		OneWire_SendBit(Byte&(0x01<<i));
	}
	//ET0 = 1;
}

/**
  * @brief  单总线接收一个字节,先低字节，然后高字节
  * @param  无
  * @retval 接收的一个字节
  */
unsigned char OneWire_ReceiveByte(void)
{
	unsigned char i;
	unsigned char Byte=0x00;
	//ET0 = 0;
	for(i=0;i<8;i++)
	{
		if(OneWire_ReceiveBit()){Byte|=(0x01<<i);}
	}
	//ET0 = 1;
	return Byte;
}


