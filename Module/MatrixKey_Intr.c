#include <REGX52.H>

unsigned char MatrixKey_FreshKey = 0;

/**
  * @brief  矩阵键盘返回最新鲜的Key值，取后清0,相当与从缓存中取值
  * @param  无
  * @retval 最新鲜的Key值
  */
unsigned char MatrixKey(){
	unsigned char Temp = 0;
	Temp = MatrixKey_FreshKey;
	MatrixKey_FreshKey = 0;
	return Temp;
}

/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值
  */
unsigned char MatrixKey_Scan()
{
	
	//不修改P3_3的值，只改变P3其它引脚，防止影响外部中断
	unsigned char KeyNumber=0;
	
	P3_0 = 0;P3_1 = 1;P3_2 = 1;
	P3_4 = 1;P3_5 = 1;P3_6 = 1;P3_7 = 1;
	     if(P3_4==0){KeyNumber=1;}
	else if(P3_5==0){KeyNumber=2;}
	else if(P3_6==0){KeyNumber=3;}
	else if(P3_7==0){KeyNumber=4;}
	
	P3_0 = 1;P3_1 = 0;P3_2 = 1;
	P3_4 = 1;P3_5 = 1;P3_6 = 1;P3_7 = 1;
	     if(P3_4==0){KeyNumber=5;}
	else if(P3_5==0){KeyNumber=6;}
	else if(P3_6==0){KeyNumber=7;}
	else if(P3_7==0){KeyNumber=8;}
	
	P3_0 = 1;P3_1 = 1;P3_2 = 0;
	P3_4 = 1;P3_5 = 1;P3_6 = 1;P3_7 = 1;
	     if(P3_4==0){KeyNumber=9;}
	else if(P3_5==0){KeyNumber=10;}
	else if(P3_6==0){KeyNumber=11;}
	else if(P3_7==0){KeyNumber=12;}
	
	//因为光电模块没有阻挡时输出0，可以正好利用这一点扫描按键
	//若光电模块中有遮挡时，P3_3为1，会造成13-16按键按下无效
	//可以通过长按按键来让光电模块没有遮挡时捕获按键
	P3_0 = 1;P3_1 = 1;P3_2 = 1;
	P3_4 = 1;P3_5 = 1;P3_6 = 1;P3_7 = 1;
	     if(P3_4==0){KeyNumber=13;}
	else if(P3_5==0){KeyNumber=14;}
	else if(P3_6==0){KeyNumber=15;}
	else if(P3_7==0){KeyNumber=16;}
	
	return KeyNumber;
}

/**
  * @brief  矩阵键盘读取按键键码
  * @param  无
  * @retval KeyNumber 按下按键的键码值(利用定时扫描实现)
			松手的一瞬间，返回按键键码，没有按键按下时，返回0
  */

void MatrixKey_Loop(){
	static unsigned char LastStatus,NowStatus;
	LastStatus = NowStatus;
	NowStatus = MatrixKey_Scan();
		 if(LastStatus == 1 && NowStatus == 0) MatrixKey_FreshKey = 1;
	else if(LastStatus == 2 && NowStatus == 0) MatrixKey_FreshKey = 2;
	else if(LastStatus == 3 && NowStatus == 0) MatrixKey_FreshKey = 3;
	else if(LastStatus == 4 && NowStatus == 0) MatrixKey_FreshKey = 4;
	else if(LastStatus == 5 && NowStatus == 0) MatrixKey_FreshKey = 5;
	else if(LastStatus == 6 && NowStatus == 0) MatrixKey_FreshKey = 6;
	else if(LastStatus == 7 && NowStatus == 0) MatrixKey_FreshKey = 7;
	else if(LastStatus == 8 && NowStatus == 0) MatrixKey_FreshKey = 8;
	else if(LastStatus == 9 && NowStatus == 0) MatrixKey_FreshKey = 9;
	else if(LastStatus == 10 && NowStatus == 0) MatrixKey_FreshKey = 10;
	else if(LastStatus == 11 && NowStatus == 0) MatrixKey_FreshKey = 11;
	else if(LastStatus == 12 && NowStatus == 0) MatrixKey_FreshKey = 12;
	else if(LastStatus == 13 && NowStatus == 0) MatrixKey_FreshKey = 13;
	else if(LastStatus == 14 && NowStatus == 0) MatrixKey_FreshKey = 14;
	else if(LastStatus == 15 && NowStatus == 0) MatrixKey_FreshKey = 15;
	else if(LastStatus == 16 && NowStatus == 0) MatrixKey_FreshKey = 16;
}