#include <REGX52.H>
#include "Key.h"
#include "Nixie.h"
#include "Delay.h"
#include "LCD1602.h"
#include "MatrixKey.h"

void main(void){
	unsigned char KeyNum;
	LCD_Init();							//LCD初始化
	LCD_ShowString(1,1,"MatrixKey:");	//LCD显示字符串
	while(1)
	{
		KeyNum=MatrixKey();				//获取矩阵键盘键码
		if(KeyNum)						//如果有按键按下
		{
			LCD_ShowNum(2,1,KeyNum,2);	//LCD显示键码
		}
	}
	
}

void test(void){
//	unsigned char sence = 0;
//	unsigned char key = 0;
//	unsigned char i,location;
//	while(1)
//	{
//		key = Key();
//		sence ^= 0x01<<key;
//		for(i = 0;i<8;i++){
//			location = scence&(0x01<<i);
//			Nixie(location&(i+1),i+1);
//		}
//		
//	}
}