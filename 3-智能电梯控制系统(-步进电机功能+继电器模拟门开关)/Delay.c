#include<INTRINS.H>

void Delayxms(unsigned int x)		//@11.0592MHz
{
	unsigned char i, j;
	//执行x次
	for(;x>0;x--){
		_nop_();
		i = 11;
		j = 190;
		do
		{
			while (--j);
		} while (--i);
	}
}

