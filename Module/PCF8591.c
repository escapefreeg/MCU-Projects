#include <REGX52.H>
#include "I2C.h"

#define PCF8591_ADDRESS 	0x90

void PCF8591_WriteByte(unsigned char Data)
{
	I2C_Start();
	I2C_SendByte(PCF8591_ADDRESS);
	I2C_ReceiveAck();
	
	I2C_SendByte(0x40);
	I2C_ReceiveAck();

	I2C_SendByte(Data);
	I2C_ReceiveAck();
	I2C_Stop();
}

unsigned char PCF8591_ReadByte(unsigned char Channel)
{
	unsigned char Data;
	I2C_Start();
	I2C_SendByte(PCF8591_ADDRESS);
	I2C_ReceiveAck();

	I2C_SendByte(Channel & 0x03);
	I2C_ReceiveAck();
	I2C_Start();
	I2C_SendByte(PCF8591_ADDRESS|0x01);
	I2C_ReceiveAck();
	Data = I2C_ReceiveByte();
	I2C_SendAck(1);
	I2C_Stop();
	return Data;
}
