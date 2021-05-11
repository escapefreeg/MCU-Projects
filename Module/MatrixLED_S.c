#include <REGX52.H>
#include "Delay.h"


//DU控制的是行
sbit MatrixLED_DU = P2^0;
//CS88控制的是列
sbit MatrixLED_CS88 = P2^2;



//禁用点阵
void MatrixLED_Freeze(void){
	
	//点阵列全部提供低电平(-)
	MatrixLED_CS88 = 1;
	P0 = 0xFF;
	MatrixLED_CS88 = 0;

}


/**
  * @brief  LED点阵屏显示一列数据
  * @param  Column 要选择的列，范围：1~8，1在最左边
  * @param  Data 选择列显示的数据，高位在上，1为亮，0为灭
  * @retval 无
  */
void MatrixLED_ShowColumn(unsigned char Column,Data)
{
	MatrixLED_CS88 = 1;
	P0 = ~(0x80>>(Column-1));
	MatrixLED_CS88 = 0;
	
	MatrixLED_DU = 1;
	P0 = Data;
	MatrixLED_DU = 0;
	
	Delayxms(2);
	MatrixLED_DU = 1;
	P0 = 0x00;
	MatrixLED_DU = 0;
}
