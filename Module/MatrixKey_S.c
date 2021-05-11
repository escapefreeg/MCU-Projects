#include <REGX52.H>
#include "Delay.h"

/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
			如果按键按下不放，程序会停留在此函数，松手的一瞬间，返回按键键码，没有按键按下时，返回0
  */
unsigned char MatrixKey()
{
	unsigned char KeyNumber=0;
	
	P3=0xFF;
	P3_0=0;
	if(P3_7==0){Delayxms(20);while(P3_7==0);Delayxms(20);KeyNumber=4;}
	else if(P3_6==0){Delayxms(20);while(P3_6==0);Delayxms(20);KeyNumber=3;}
	else if(P3_5==0){Delayxms(20);while(P3_5==0);Delayxms(20);KeyNumber=2;}
	else if(P3_4==0){Delayxms(20);while(P3_4==0);Delayxms(20);KeyNumber=1;}
	
	P3=0xFF;
	P3_1=0;
	if(P3_7==0){Delayxms(20);while(P3_7==0);Delayxms(20);KeyNumber=8;}
	else if(P3_6==0){Delayxms(20);while(P3_6==0);Delayxms(20);KeyNumber=7;}
	else if(P3_5==0){Delayxms(20);while(P3_5==0);Delayxms(20);KeyNumber=6;}
	else if(P3_4==0){Delayxms(20);while(P3_4==0);Delayxms(20);KeyNumber=5;}
	
	P3=0xFF;
	P3_2=0;
	if(P3_7==0){Delayxms(20);while(P3_7==0);Delayxms(20);KeyNumber=12;}
	else if(P3_6==0){Delayxms(20);while(P3_6==0);Delayxms(20);KeyNumber=11;}
	else if(P3_5==0){Delayxms(20);while(P3_5==0);Delayxms(20);KeyNumber=10;}
	else if(P3_4==0){Delayxms(20);while(P3_4==0);Delayxms(20);KeyNumber=9;}
	
	P3=0xFF;
	P3_3=0;
	if(P3_7==0){Delayxms(20);while(P3_7==0);Delayxms(20);KeyNumber=16;}
	else if(P3_6==0){Delayxms(20);while(P3_6==0);Delayxms(20);KeyNumber=15;}
	else if(P3_5==0){Delayxms(20);while(P3_5==0);Delayxms(20);KeyNumber=14;}
	else if(P3_4==0){Delayxms(20);while(P3_4==0);Delayxms(20);KeyNumber=13;}
	
	return KeyNumber;
}
