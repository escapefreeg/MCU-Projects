#include "I2C.h"

void TM1637_Show(unsigned char Pos,unsigned char Num){
    I2C_Start();
    I2C_SendByte(0x44);//0b0100 0100
    I2C_ReceiveAck();
    I2C_Stop();

    I2C_Start();
    I2C_SendByte(0xC0|(Pos-1));//0b1100 00 00
    I2C_ReceiveAck();
    I2C_SendByte(Num);//0b0100 0100
    I2C_ReceiveAck();
    I2C_Stop();
    I2C_Start();
    I2C_SendByte(0x8f);//0b1000 1000
    I2C_ReceiveAck();
    I2C_Stop();
}