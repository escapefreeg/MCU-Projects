#include <REGX52.H>
#include "LCD12864.h"

//时序问题
sbit LCD_PSB =  P3^7;           
sbit LCD_reset =  P2^6; 
sbit LCD_reset1 =  P2^7; 

void main(void){
	//init

    LCD_PSB = 1;
    LCD_reset =  1;
    LCD_reset1 =  1;
    LCD12864_Init();
    
    //
    while(1){
        LCD12864_ShowNum(1,1,57,2);
        LCD12864_ShowString(2,2,"abcd:089.6");
        }
}