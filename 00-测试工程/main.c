//项目中采用java命名法
#include <REGX52.H>
//要引入P4对应的地址！！！
sfr P4 = 0xC0;
sbit resetPin = P4^7;

unsigned char key = 0;

void Delay1000ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


void testReset(){
    resetPin = !resetPin;
    Delay1000ms();
}
void main(void){
    resetPin = 1;
	
    while(1){
        testReset();
    }
}