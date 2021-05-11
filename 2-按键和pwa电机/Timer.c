#include <REGX52.H>

/**
  * @brief  定时器0初始化，1毫秒@11.0592MHz,适用于1T单片机
  * @param  无
  * @retval 无
  */
void Timer0_Init(void)
{
	//AUXR &= 0x7F;		//定时器时钟12T模式
	
	TMOD &= 0xF0;		//设置定时器模式
	TMOD |= 0x01;		//设置定时器模式
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;          //打开定时器0的中断
	EA=1;          //打开整体的中断开关
	
	//PT0=0;
}
/**
  * @brief  定时器0初始化，1毫秒@11.0592MHz,适用于1T单片机
  * @param  无
  * @retval 无
  */

void Timer1_Init(void)
{
	//AUXR &= 0x7F;		//定时器时钟12T模式
	
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x10;		//设置定时器模式
	TL1 = 0x66;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	TF1 = 0;		//清除TF0标志
	TR1 = 1;		//定时器0开始计时
	ET1=1;          //打开定时器0的中断
	EA=1;          //打开整体的中断开关
	
	//PT0=0;
}


/*定时器中断函数模板

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
	if(T0Count>=1000)
	{
		T0Count=0;
		
	}
}

void Timer1_Routine() interrupt 3
{
	static unsigned int T1Count;
	TL1 = 0x66;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	T1Count++;
	if(T1Count>=1000)
	{
		T1Count=0;
		
	}
}

*/
