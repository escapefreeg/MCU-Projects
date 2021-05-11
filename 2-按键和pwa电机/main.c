#include <REGX52.H>
#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"

unsigned char key = 0;
unsigned char speed = 0;
unsigned char theNum[] = {0,30,70,100};
sbit out = P2^0;

void main(void){
    
	//init
    Timer0_Init();

    //
    while(1){
        key = MatrixKey();
        //add
        if(key == 1){
            speed = (speed+1)%4;
        }
        //minus
        else if(key == 2){
            speed = (speed-1)%4;
        }

    }
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
    static unsigned char line;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
    line++;
	if(T0Count>=20)
	{
		T0Count=0;
        MatrixKey_Loop();
	}
    if(line == 100) line = 0;
    else if(line <= theNum[speed]){
        out = 1;
    }
    else if(line > theNum[speed]){
        out = 0;
    }
}