#include <REGX52.H>
#include "LCD12864.h"
#include "Delay.h"
sfr P1ASF = 0x9D;
sfr ADC_CONTR = 0xBC;
sfr ADC_RES = 0xBD;
sfr ADC_RESL = 0xBE;
sfr AUXR1 = 0xA2;


unsigned char re = 0;
void main(void){
    
	//init
    LCD12864_Init();

    //选择ADC引脚
    P1ASF = 0x04;
    //上电
    //0b1110 0010
    ADC_CONTR = 0xE2;
    //延时
    Delayxms(5);
    //输出格式 ADRJ = 1
    AUXR1 &= 0xFB;
    //打开中断
    IE |= 0xA0; 
    //启动
    //0b1110 1010
    ADC_CONTR = 0xEA;


    //
    LCD12864_ShowNum(1,7,666,3);
    LCD12864_ShowString(2,7,"ADC");

    while(1){
        LCD12864_ShowNum(3,7,re,3);
    }
}

void adc () interrupt 5{
    re = ADC_RES;
    //软件清0
    //0b1110 1010
    ADC_CONTR = 0xEA;
}

