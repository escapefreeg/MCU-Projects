//采用集成版板子开发
#include <REGX52.H>
#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "Delay.h"
#include "AT24C02.h"
#include "DS18B20.h"

float TheT = 0;
float TH = 0;
float TL = 100;

void ShowT(unsigned char Line,unsigned char Column,float tt){
    LCD12864_ShowNum(Line,Column,tt);
    
}
void main(void){

    LCD12864_Init();
    LCD12864_ShowString(1,7,"h007");
    Delayxms(1);

    //init
    LCD12864_Init();
    DS18B20_ConvertT();
        //显示摄氏度的基础信息
    LCD12864_ShowString(1,1,"S:   .    ");

    while (1)
    {
        TheT = DS18B20_ReadT();

    }
    
}


//定时器中断1
void Timer1_Routine() interrupt 3
{
	static unsigned int T1Count = 0;
	TL1 = 0x66;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	T1Count++;

	if(T1Count>=20)
	{
		MatrixKey_Loop();
        T1Count=0;
	}
}


