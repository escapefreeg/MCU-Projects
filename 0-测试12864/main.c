#include <REGX52.H>
#include "LCD12864.h"

//时序问题
//测试奇数开头的功能是否正常

sbit LCD_PSB =  P3^7;           


void main(void){
	//init

    LCD_PSB = 1;
    LCD12864_Init();
    
    //
    while(1){
        LCD12864_ShowNum(1,1,57,2);
        LCD12864_ShowString(2,2,"zello world");
        LCD12864_ShowHexNum(3,2,48,3);
    }
}