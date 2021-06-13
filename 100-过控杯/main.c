#include <REGX52.H>

#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "AT24C02.h"
#include "DS18B20.h"


bit key1 = 0;bit nowkey1 = 0;
bit key2 = 0;bit nowkey2 = 0;
bit key3 = 0;bit nowkey3 = 0;
bit key4 = 0;bit nowkey4 = 0;
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
    DS18B20_ConvertT();
    Delayxms(1000);

    // ((unsigned char *)(&TH))[0] = AT24C02_ReadByte(1);
    // ((unsigned char *)(&TH))[1] = AT24C02_ReadByte(2);
    // ((unsigned char *)(&TH))[2] = AT24C02_ReadByte(3);
    // ((unsigned char *)(&TH))[3] = AT24C02_ReadByte(4);
    
    // ((unsigned char *)(&TL))[0] = AT24C02_ReadByte(5);
    // ((unsigned char *)(&TL))[1] = AT24C02_ReadByte(6);
    // ((unsigned char *)(&TL))[2] = AT24C02_ReadByte(7);
    // ((unsigned char *)(&TL))[3] = AT24C02_ReadByte(8);
        //显示摄氏度的基础信息
    LCD12864_ShowString(1,2,"Temp:+");
    LCD12864_ShowString(2,4,"FSpeed:");

    while (1)
    {
        key = MatrixKey();
        //显示开关
        if(key == 1){
            
        }
        //选择更改的位置
        else if(key == 2){
            
        }
        //清0
        else if(key == 3){
            
        }
        //取最大
        else if(key == 4){

        }
        //+1
        else if(key == 5){
            
        }
        //-1
        else if(key == 6){
            
        }
        //+5
        else if(key == 7){
            
        }
        //-5
        else if(key == 8){
            
        }
        
        DS18B20_ConvertT();
        TheT = DS18B20_ReadT();
        ShowT(1,3,TheT);
        if(TheT < TL){
            TL = TheT;
            AT24C02_WriteByte(5,((unsigned char *)(&TL))[0]);
            Delayxms(5);
            AT24C02_WriteByte(6,((unsigned char *)(&TL))[1]);
            Delayxms(5);
            AT24C02_WriteByte(7,((unsigned char *)(&TL))[2]);
            Delayxms(5);
            AT24C02_WriteByte(8,((unsigned char *)(&TL))[3]);
            Delayxms(5);
        }
        if(TheT > TH){
            TH = TheT;
            //存储
            AT24C02_WriteByte(1,((unsigned char *)(&TH))[0]);
            Delayxms(5);
            AT24C02_WriteByte(2,((unsigned char *)(&TH))[1]);
            Delayxms(5);
            AT24C02_WriteByte(3,((unsigned char *)(&TH))[2]);
            Delayxms(5);
            AT24C02_WriteByte(4,((unsigned char *)(&TH))[3]);
            Delayxms(5);
        }
        //刷新华氏度
        if((key1|nowkey1)){
            if(nowkey1 == 0){
                LCD12864_ShowString(2,1,"           ");
            }
            else{
                LCD12864_ShowString(2,1,"H:");
                TheTC = TheT*1.8+32;
                ShowT(2,3,TheTC);
            }
        }
        if((key2|nowkey2)){
            if(nowkey2 == 0){
                LCD12864_ShowString(3,1,"            ");
            }
            else{
                LCD12864_ShowString(3,1,"TH:");
                ShowT(3,4,TH);
            }
        }
        if((key3|nowkey3)){
            if(nowkey3 == 0){
                LCD12864_ShowString(4,1,"            ");
            }
            else{
                LCD12864_ShowString(4,1,"TL:");
                ShowT(4,4,TL);
            }
        }
        if((key4|nowkey4)){
            if(nowkey4 == 0){
                LCD12864_ShowString(1,14,"  ");
                LCD12864_ShowString(2,13,"   ");
                LCD12864_ShowString(3,14,"  ");
                LCD12864_ShowString(4,13,"   ");
            }
            else{
                LCD12864_ShowString(1,14,"HN");
                LCD12864_ShowChar(2,13,'+');
                LCD12864_ShowNum(2,14,THSet,2);
                LCD12864_ShowString(3,14,"LN");
                LCD12864_ShowChar(4,13,'+');
                LCD12864_ShowNum(4,14,TLSet,2);
            }
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

