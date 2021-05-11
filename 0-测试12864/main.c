#include <REGX52.H>
#include "LCD12864.h"

sbit LCD_PSB =  P3^7;           
sbit wela    =  P2^6;
sbit dula    =  P2^7;

void main(void){
    unsigned char key = 0;
	//init
    
    LCD12864_Init();
    LCD_PSB = 1;
    wela=0;
	dula=0;         
    //
    while(1){
        //key = MatrixKey();
        LCD12864_ShowNum(1,1,57,2);
        LCD12864_ShowString(2,2,"hello world");
        LCD12864_ShowHexNum(3,2,48,3);
    }
}