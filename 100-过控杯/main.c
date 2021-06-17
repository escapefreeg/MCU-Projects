#include <REGX52.H>

#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "AT24C02.h"
#include "DS18B20.h"
#include "Delay.h"

sbit fan = P2^0;
unsigned char sFlag = 1;
unsigned char speed = 50;
unsigned char key;
unsigned char min = 20,mid=24,max=28;
unsigned char minc,midc,maxc,speedc;
unsigned char procCon=0;
unsigned char lastCon=0;
float TheT;


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

void ShowInfo(void)//时间显示功能
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

void SetInfo(void)//时间设置功能
{
        if(key == 3){
            //闪烁情况下才可以使用
                //开显示的情况
                    //位置加1，注意到上限
                //关显示的情况
                    //没有变化
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
                case 0x00: speedc += 1;break;
                case 0x10: minc += 1;break;
                case 0x20: midc += 1;break;
                case 0x30: maxc += 1;break;
            }
        }
        //-1
        else if(key == 6){
            //闪烁的情况才可以用
                //选中位置-1
            switch(procCon & 0xF0){
                case 0x00: speedc -= 1;break;
                case 0x10: minc -= 1;break;
                case 0x20: midc -= 1;break;
                case 0x30: maxc -= 1;break;
            }
        }
        //+5
        else if(key == 7){
            //闪烁的情况才可以用
                //选中位置+5
            switch(procCon & 0xF0){
                case 0x00: speedc += 5;break;
                case 0x10: minc += 5;break;
                case 0x20: midc += 5;break;
                case 0x30: maxc += 5;break;
            }
        }
        //-5
        else if(key == 8){
            //闪烁的情况才可以用
                //选中位置-5
            switch(procCon & 0xF0){
                case 0x00: speedc -= 5;break;
                case 0x10: minc -= 5;break;
                case 0x20: midc -= 5;break;
                case 0x30: maxc -= 5;break;
            }
        }
        //清空
        else if(key == 9){
            //闪烁的情况才可以用
                //选中位置清空
            switch(procCon & 0xF0){
                case 0x00: speedc = 0;break;
                case 0x10: minc = 0;break;
                case 0x20: midc = 0;break;
                case 0x30: maxc = 0;break;
            }
        }
        //取最大
        else if(key == 10){
            //闪烁的情况才可以用
                //选中位置清空
            switch(procCon & 0xF0){
                case 0x00: speedc = 100;break;
                case 0x10: minc = 40;break;
                case 0x20: midc = 40;break;
                case 0x30: maxc = 40;break;
            }
        }
        //显示和不显示
        if(procCon & 0x01){
            //显示
            LCD12864_ShowNum(2,8,speedc,3);
            LCD12864_ShowNum(3,5,minc,3);
            LCD12864_ShowNum(3,14,midc,3);
            LCD12864_ShowNum(4,10,maxc,3);
            switch(procCon & 0xF0){
                case 0x00:
                    if(sFlag) LCD12864_ShowString(2,8,"   ");break;
                case 0x10:
                    if(sFlag) LCD12864_ShowString(3,5,"   ");break;
                case 0x20: 
                    if(sFlag) LCD12864_ShowString(3,14,"   ");break;
                case 0x30:
                    if(sFlag) LCD12864_ShowString(4,10,"   ");break;
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
    LCD12864_ShowString(2,2,"Speed:");
    LCD12864_ShowString(2,12,"auto");

    while (1)
    {
        key = MatrixKey();
        //显示开关
        if(key == 1){
            lastCon = procCon;
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
            //关闪烁对应的操作
            min=minc;
            mid=midc;
            max=maxc;
            speed=speedc;
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
                    min=minc;
                    mid=midc;
                    max=maxc;
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

        
        //闪烁效果
        //闪烁打开
        if(procCon & 0x02){
            SetInfo();
        }
        //闪烁关闭
        else{
            ShowInfo();
        }




        // if(TheT < TL){
        //     TL = TheT;
        //     AT24C02_WriteByte(5,((unsigned char *)(&TL))[0]);
        //     Delayxms(5);
        //     AT24C02_WriteByte(6,((unsigned char *)(&TL))[1]);
        //     Delayxms(5);
        //     AT24C02_WriteByte(7,((unsigned char *)(&TL))[2]);
        //     Delayxms(5);
        //     AT24C02_WriteByte(8,((unsigned char *)(&TL))[3]);
        //     Delayxms(5);
        // }
        // if(TheT > TH){
        //     TH = TheT;
        //     //存储
        //     AT24C02_WriteByte(1,((unsigned char *)(&TH))[0]);
        //     Delayxms(5);
        //     AT24C02_WriteByte(2,((unsigned char *)(&TH))[1]);
        //     Delayxms(5);
        //     AT24C02_WriteByte(3,((unsigned char *)(&TH))[2]);
        //     Delayxms(5);
        //     AT24C02_WriteByte(4,((unsigned char *)(&TH))[3]);
        //     Delayxms(5);
        // }
        // //刷新华氏度
        // if((key1|nowkey1)){
        //     if(nowkey1 == 0){
        //         LCD12864_ShowString(2,1,"           ");
        //     }
        //     else{
        //         LCD12864_ShowString(2,1,"H:");
        //         TheTC = TheT*1.8+32;
        //         ShowT(2,3,TheTC);
        //     }
        // }
        // if((key2|nowkey2)){
        //     if(nowkey2 == 0){
        //         LCD12864_ShowString(3,1,"            ");
        //     }
        //     else{
        //         LCD12864_ShowString(3,1,"TH:");
        //         ShowT(3,4,TH);
        //     }
        // }
        // if((key3|nowkey3)){
        //     if(nowkey3 == 0){
        //         LCD12864_ShowString(4,1,"            ");
        //     }
        //     else{
        //         LCD12864_ShowString(4,1,"TL:");
        //         ShowT(4,4,TL);
        //     }
        // }
        // if((key4|nowkey4)){
        //     if(nowkey4 == 0){
        //         LCD12864_ShowString(1,14,"  ");
        //         LCD12864_ShowString(2,13,"   ");
        //         LCD12864_ShowString(3,14,"  ");
        //         LCD12864_ShowString(4,13,"   ");
        //     }
        //     else{
        //         LCD12864_ShowString(1,14,"HN");
        //         LCD12864_ShowChar(2,13,'+');
        //         LCD12864_ShowNum(2,14,THSet,2);
        //         LCD12864_ShowString(3,14,"LN");
        //         LCD12864_ShowChar(4,13,'+');
        //         LCD12864_ShowNum(4,14,TLSet,2);
        //     }
        // }
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
    if(shrink>=400)
	{
		shrink=0;
		sFlag = 1 - sFlag; 
	}
    if(speedV == 100) speedV = 0;
    else if(speedV <= speed){
        fan = 1;
    }
    else if(speedV > speed){
        fan = 0;
    }
}

