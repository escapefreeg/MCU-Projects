#include <REGX52.H>
#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"

void main(void){
    unsigned char key = 0;
	//init
    Timer0_Init();
    LCD12864_Init();
    //
    LCD12864_ShowNum(2,1,15,3);
    while(1){
        key = MatrixKey();
        if(key != 0)
            LCD12864_ShowNum(1,1,key,3);
    }
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=20)
	{
		T0Count=0;
        MatrixKey_Loop();
	}
}