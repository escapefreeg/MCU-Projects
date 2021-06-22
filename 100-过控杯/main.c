#include <REGX52.H>

#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "AT24C02.h"
#include "DS18B20.h"
#include "Delay.h"

//todo list
//1 auto的判断的实现        完成
//2 存储配置参数            完成
//3 存贮最低温最高温        待定
//4 修改数值时的逻辑约束    完成
//6 修改下闪烁逻辑          完成

sbit fan = P2^0;
sbit WE = P2^1;
sbit CS88 = P2^2;
sbit LED = P1^5;
sbit BUZZ = P2^3;
unsigned char sFlag = 1;
unsigned char speed = 50;
unsigned char key;
unsigned char min = 20,mid=24,max=28;
unsigned char minc,midc,maxc,speedc;
unsigned char procCon=0;
unsigned char lastCon=0;
float TheT;

void saveValue(unsigned char vcode,unsigned char x){
    switch(vcode){
        case 0x01:
            min = x;
            AT24C02_WriteByte(1,min);
            Delayxms(5);
            break;
        case 0x02:
            mid = x;
            AT24C02_WriteByte(2,mid);
            Delayxms(5);
            break;
        case 0x03:
            max = x;
            AT24C02_WriteByte(3,max);
            Delayxms(5);
            break;
    }
    
}
void safeChange(unsigned char vcode,unsigned char x){
    switch (vcode){
        case 0x10:
            if(x<=100 && x>=0) speedc = x;break;
        case 0x11:
            if(x >= 18 && x < midc) minc = x;break;
        case 0x12:
            if(x>minc && x<maxc) midc = x;break;
        case 0x13:
            if(x<=40 && x>midc) maxc = x;break;
    }
}
//第几行第几列开始显示温度
void ShowT(unsigned char Line,unsigned char Column,float tt){
    //tt可以确定是正的
    //先显示整数部分
    LCD12864_ShowNum(Line,Column,tt,3);
    //
    LCD12864_ShowChar(Line,Column + 3 ,'.');
    //小数部分
    LCD12864_ShowNum(Line,Column + 4 ,((unsigned long)(tt*10000))%10000,4);
}

//时间显示功能
void ShowInfo(void)
{
    LCD12864_ShowNum(2,8,speed,3);
        //显示和不显示
        if(procCon & 0x01){
            //显示
            //上次是显示就刷新数字
            //上次是不显示就刷新数字和字符
            LCD12864_ShowNum(3,5,min,3);
            LCD12864_ShowNum(3,14,mid,3);
            LCD12864_ShowNum(4,10,max,3);
            if(!(lastCon & 0x01)){
                LCD12864_ShowString(3,1,"min:");
                LCD12864_ShowString(3,10,"mid:");
                LCD12864_ShowString(4,6,"max:");
            }
        }
        else{
            //不显示
            //上次是显示就刷新
            //上次是不显示就什么不做
            if(lastCon & 0x01){
                LCD12864_ShowString(3,1,"       ");
                LCD12864_ShowString(3,10,"       ");
                LCD12864_ShowString(4,6,"       ");
            }
        }
}

//时间设置功能
void SetInfo(void)
{
        if(key == 3){
            //闪烁情况下才可以使用
                //开显示的情况
                    //位置加1，注意到上限
                //关显示的情况
                    //没有变化
            //补上上一个选择位的内容
            switch(procCon & 0xF0){
                case 0x00:
                    LCD12864_ShowNum(2,8,speedc,3);break;
                case 0x10:
                    LCD12864_ShowNum(3,5,minc,3);break;
                case 0x20: 
                    LCD12864_ShowNum(3,14,midc,3);break;
                case 0x30:
                    LCD12864_ShowNum(4,10,maxc,3);break;
            }
            //如果是开显示情况
            if (procCon &0x01){
                //自动模式情况
                if(!(procCon & 0x04)){
                    procCon += 0x10;
                    if ((procCon & 0xF0) == 0x40) {procCon &= 0x0F;procCon |= 0x10;}
                }
                //手动模式情况
                else{
                    procCon = (procCon + 0x10) % 0x40;
                }
            }
            //如果是关显示情况
            else{
                ;
            }
        }
        else if(key == 4){
            //补上上一个选择位的内容
            switch(procCon & 0xF0){
                case 0x00:
                    LCD12864_ShowNum(2,8,speedc,3);break;
                case 0x10:
                    LCD12864_ShowNum(3,5,minc,3);break;
                case 0x20: 
                    LCD12864_ShowNum(3,14,midc,3);break;
                case 0x30:
                    LCD12864_ShowNum(4,10,maxc,3);break;
            }
            if (procCon &0x01){
                //自动模式情况
                if(!(procCon & 0x04)){
                    procCon -= 0x10;
                    if ((procCon & 0xF0) == 0x00) {procCon &= 0x0F;procCon |= 0x30;}
                }
                //手动模式情况
                else{
                    procCon = (procCon + 0x40 - 0x10 ) % 0x40;
                }
            }
            //如果是关显示情况
            else{
                ;
            }
        }
        //+1
        else if(key == 5){
            //闪烁的情况才可以用
                //选中位置+1
            switch(procCon & 0xF0){
                case 0x00: safeChange(0x10,speedc + 1);break;
                case 0x10: safeChange(0x11,minc + 1);break;
                case 0x20: safeChange(0x12,midc + 1);break;
                case 0x30: safeChange(0x13,maxc + 1);break;
            }
        }
        //-1
        else if(key == 6){
            //闪烁的情况才可以用
                //选中位置-1
            switch(procCon & 0xF0){
                case 0x00: safeChange(0x10,speedc - 1);break;
                case 0x10: safeChange(0x11,minc - 1);break;
                case 0x20: safeChange(0x12,midc - 1);break;
                case 0x30: safeChange(0x13,maxc - 1);break;
            }
        }
        //+5
        else if(key == 7){
            //闪烁的情况才可以用
                //选中位置+5
            switch(procCon & 0xF0){
                case 0x00: safeChange(0x10,speedc + 5);break;
                case 0x10: safeChange(0x11,minc + 5);break;
                case 0x20: safeChange(0x12,midc + 5);break;
                case 0x30: safeChange(0x13,maxc + 5);break;
            }
        }
        //-5
        else if(key == 8){
            //闪烁的情况才可以用
                //选中位置-5
            switch(procCon & 0xF0){
                case 0x00: safeChange(0x10,speedc - 5);break;
                case 0x10: safeChange(0x11,minc - 5);break;
                case 0x20: safeChange(0x12,midc - 5);break;
                case 0x30: safeChange(0x13,maxc - 5);break;
            }
        }
        //清空
        else if(key == 9){
            //闪烁的情况才可以用
                //选中位置清空
            switch(procCon & 0xF0){
                case 0x00: safeChange(0x10,0);break;
                case 0x10: safeChange(0x11,18);break;
                case 0x20: safeChange(0x12,minc + 1);break;
                case 0x30: safeChange(0x13,midc + 1);break;
            }
        }
        //取最大
        else if(key == 10){
            //闪烁的情况才可以用
                //选中位置清空
            switch(procCon & 0xF0){
                case 0x00: safeChange(0x10,100);break;
                case 0x10: safeChange(0x11,midc - 1);break;
                case 0x20: safeChange(0x12,maxc - 1);break;
                case 0x30: safeChange(0x13,40);break;
            }
        }
        //显示和不显示
        if(procCon & 0x01){
            //显示
            switch(procCon & 0xF0){
                case 0x00:
                    if(sFlag) LCD12864_ShowString(2,8,"   ");
                    else LCD12864_ShowNum(2,8,speedc,3);
                    break;
                case 0x10:
                    if(sFlag) LCD12864_ShowString(3,5,"   ");
                    else LCD12864_ShowNum(3,5,minc,3);
                    break;
                case 0x20: 
                    if(sFlag) LCD12864_ShowString(3,14,"   ");
                    else LCD12864_ShowNum(3,14,midc,3);
                    break;
                case 0x30:
                    if(sFlag) LCD12864_ShowString(4,10,"   ");
                    else LCD12864_ShowNum(4,10,maxc,3);
                    break;
            }
        }
        else{
            //不显示
            if(sFlag) LCD12864_ShowString(2,8,"   ");
            else      LCD12864_ShowNum(2,8,speedc,3);
        }
}

void main(void){

    LCD12864_Init();
    Timer0_Init();
    //关闭数码管和8*8
    WE = 1;
    P0 = 0xFF;
    WE = 0;
    CS88 = 1;
    P0 = 0xFF;
    CS88 = 0;
    DS18B20_ConvertT();
    Delayxms(1000);
    //从rom中读取配置参数
    min = AT24C02_ReadByte(1);
    mid = AT24C02_ReadByte(2);
    max = AT24C02_ReadByte(3);
    //显示基础信息
    LCD12864_ShowString(1,2,"Temp:+");
    LCD12864_ShowString(2,2,"Speed:");
    LCD12864_ShowString(2,12,"auto");

    while (1)
    {
        key = MatrixKey();
        //显示开关
        if(key == 1){
            lastCon = procCon;
            //关闪烁对应的操作
            if(procCon & 0x02)//0000 0001
            {
                saveValue(0x01,minc);
                saveValue(0x02,midc);
                saveValue(0x03,maxc);
                speed=speedc;
            }
            //显示打开状态
            if(procCon & 0x01)//0000 0001
            {
                //设置标志位
                //默认选择位回0和关闪烁
                procCon &= 0x0D;
            }
            //显示关闭状态
            else{
                //设置标志位
                //默认选择位回1和关闪烁
                procCon &= 0x0D;
                procCon |= 0x10;
            }
            
            
            procCon^=0x01;//反转标志位
        }
        //设置开关
        else if(key == 2){
            //关闪烁状态，打开闪烁
                //设置标志位，进入修改模式
            //开闪烁状态，关闭闪烁
                //设置标志位，推出修改模式
            //关显示自动模式无法打开闪烁
            if((procCon & 0x05) != 0x00){
                lastCon = procCon;
                //现在是闪烁打开
                if(procCon & 0x02)
                {
                    saveValue(0x01,minc);
                    saveValue(0x02,midc);
                    saveValue(0x03,maxc);
                    speed=speedc;
                }
                //现在是闪烁关闭
                else{
                    minc=min;
                    midc=mid;
                    maxc=max;
                    speedc=speed;
                }
                procCon ^= 0x02; //0000 0010
            }
        }
        //报警模式开关
        else if(key == 11){
            lastCon = procCon;
            procCon ^= 0x80;
        }
        //风扇模式切换
        else if(key == 12){
            lastCon = procCon;
            if(procCon & 0x04){
                //0000 0100,为1,风扇手动模式,切换为自动模式
                procCon &= 0xFB;//1111 1011
                LCD12864_ShowString(2,12,"auto");
            }
            else{
                //0000 0100,为0,风扇自动模式,切换为手动模式
                procCon |= 0x04;//0000 0100
                LCD12864_ShowString(2,12,"scon");
            } 
        }
        DS18B20_ConvertT();
        TheT = DS18B20_ReadT();
        ShowT(1,8,TheT);
        //超过最高温度，声光提醒
        if (!(procCon & 0x80))
        {
            if(TheT > max){
                //声光提醒
                if(sFlag){
                    LED = 1;
                    BUZZ = 1;
                }
                else{
                    LED = 0;
                    BUZZ = 0;
                }
            }
            else{
                //关闭声光提醒
                LED = 1;
                BUZZ = 1;
            }
        }
        //如果是auto模式，根据温度控制风扇的转速
            //如果是自动模式
        if(!(procCon & 0x04)){
            if(TheT<min) speed = 50;
            else if(TheT < mid) speed = 70;
            else if(TheT < max) speed = 100;
            else speed = 100;
        }

        //根据是否是修改模式选择不同的显示模式
        //闪烁打开
        if(procCon & 0x02){
            SetInfo();
        }
        //闪烁关闭
        else{
            ShowInfo();
        }
    }
    
}

void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count;
    static unsigned char speedV;
    static unsigned int shrink;
	TL0 = 0x66;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	T0Count++;
    speedV++;
    shrink++;
	if(T0Count>=20)
	{
		T0Count=0;
		MatrixKey_Loop();
	}
    if(shrink>=500)
	{
		shrink=0;
		sFlag = 1 - sFlag; 
	}
    if(speedV == 100) speedV = 0;
    else if(speedV < speed){
        fan = 1;
    }
    else if(speedV >= speed){
        fan = 0;
    }
}

