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
    //上电、选择速度、选择通道、初始化标志和开始位
    //0b1110 0010
    ADC_CONTR = 0xE2;
    //延时
    Delayxms(5);
    //输出格式 ADRJ = 0
    AUXR1 &= 0xFB;
    //打开中断，没有位寻址的方式，因为ADC的位置不可以
    IE |= 0xA0; 
    //启动，修改ADC_CONTR的启动位
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
    //采用直接赋值的方法
    ADC_CONTR = 0xEA;
}

