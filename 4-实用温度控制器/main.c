//采用集成版板子开发
#include <REGX52.H>
#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "Delay.h"
#include "AT24C02.h"
#include "DS18B20.h"

bit key1 = 0;
bit key2 = 0;
bit key3 = 0;
bit key4 = 0;
unsigned char key;
unsigned char THSet;
unsigned char TLSet;
float TheT;
float TheTC;
float TH;
float TL;

//第几行第几列开始显示温度
void ShowT(unsigned char Line,unsigned char Column,float tt){
    if (tt>=0)
    {
        LCD12864_ShowChar(Line,Column,'+');
    }
    else{
        LCD12864_ShowChar(Line,Column,'-');
        tt = -tt;
    }
    //tt可以确定是正的
    //先显示整数部分
    LCD12864_ShowNum(Line,Column + 1 ,tt,3);
    //
    LCD12864_ShowChar(Line,Column + 4 ,'.');
    //小数部分
    LCD12864_ShowNum(Line,Column + 5 ,((unsigned long)(tt*10000))%10000,4);
}

void main(void){

    LCD12864_Init();
    Timer0_Init();
    LCD12864_ShowString(1,7,"h007");
    Delayxms(1000);

    //init
    LCD12864_Init();
    DS18B20_ConvertT();
    Delayxms(5);
    TH = 0;
    TL = 100;
        //显示摄氏度的基础信息
    LCD12864_ShowString(1,1,"S:");

    while (1)
    {
        DS18B20_ConvertT();
        TheT = DS18B20_ReadT();
        ShowT(1,3,TheT);
        if(TheT < TL){
            TL = TheT;
        }
        if(TheT > TH){
            TH = TheT;
        }
        key = MatrixKey();
        //温度转换
        if(key == 1){
            key1 = !key1;
        }
        //显示最高温度
        else if(key == 2){
            
            key2 = !key2;
        }
        //显示最低温度
        else if(key == 3){
            
            key3 = !key3;
        }
        else if(key == 4){
            key4 = !key4;
        }
        else if(key == 5){
            //h ins
            THSet++;
        }
        else if(key == 6){
            //h des
            THSet--;
        }
        else if(key == 7){
            //l ins
            TLSet++;
        }
        else if(key == 8){
            //l des
            TLSet--;
        }

        //按下显示 和消失
        if(key1){
            //11个空格
            LCD12864_ShowString(2,1,"           ");
        }
        else{
            LCD12864_ShowString(2,1,"H:");
            TheTC = TheT*1.8+32;
            ShowT(2,3,TheTC);
        }
        //按下显示 和消失
        if(key2){
            LCD12864_ShowString(3,1,"            ");
        }
        else{
            LCD12864_ShowString(3,1,"TH:");
            ShowT(3,4,TH);
        }
        //按下显示 和消失
        if(key3){
            LCD12864_ShowString(4,1,"            ");
        }
        else{
            LCD12864_ShowString(4,1,"TL:");
            ShowT(4,4,TL);
        }
        //显示和消失设定的最大最小
        if(key4){
            LCD12864_ShowString(1,14,"  ");
            LCD12864_ShowChar(2,13,"   ");
            LCD12864_ShowString(3,14,"  ");
            LCD12864_ShowChar(4,13,"   ");
        }
        else{
            LCD12864_ShowString(1,14,"HN");
            LCD12864_ShowChar(2,13,'+');
            LCD12864_ShowNum(2,14,THSet);
            LCD12864_ShowString(3,14,"LN");
            LCD12864_ShowChar(4,13,'+');
            LCD12864_ShowNum(4,14,TLSet);
        }

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
