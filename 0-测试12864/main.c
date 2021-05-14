#include <REGX52.H>
#include "LCD12864.h"
#include <stdio.h>
//时序问题
sbit LCD_PSB =  P3^7;           
sbit LCD_reset =  P2^6; 
sbit LCD_reset1 =  P2^7; 

sfr P0M1 = 0x93;
sfr P0M0 = 0x94;

unsigned char High1 = 0,High2 = 0,High3 = 0;

void main(void){
	//init
    //设置io口为弱上拉模式
    P0M1 = 0x00;
    P0M0 = 0x00;

    //LCD_PSB = 1;
    
    LCD12864_Init();
    
    //
    while(1){
        LCD12864_ShowString(1,1,"zbcd:089.6");

        High1 = LCD12864_ReadData();
        High2 = LCD12864_ReadData();
        High3 = LCD12864_ReadData();
        LCD12864_ShowBinNum(2,1,High1,8);
        LCD12864_ShowBinNum(3,1,High2,8);
        LCD12864_ShowBinNum(4,1,High3,8);
    }
}