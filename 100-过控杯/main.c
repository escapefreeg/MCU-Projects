#include <REGX52.H>

#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "AT24C02.h"
#include "DS18B20.h"

sbit fan = P1^0;
unsigned char speed = 50;
unsigned char key;
unsigned char min = 20,mid=24,max=28;
unsigned char procCon=0;
unsigned char lastCon=0;


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

    while (1)
    {
        key = MatrixKey();
        //显示开关
        if(key == 1){
            //显示打开状态
            if(procCon & 0x01)//0000 0001
            {
                //设置标志位和关闪烁
                //默认选择位回0
                lastCon = procCon;
                procCon &= 0x0F;
            }
            //显示关闭状态
            else{
                //设置标志位和关闪烁
                //默认选择位回1
                procCon &= 0x0F;//高四位清零 
                lastCon &= 0xF0;//低四位清零
                procCon |= lastCon;//加载上次的选择位
                lastCon = 0x00;
            }
            procCon^=0x01;//反转标志位
        }
        //设置开关
        else if(key == 2){
            //关闪烁状态，打开闪烁
                //设置标志位，进入修改模式
            //开闪烁状态，关闭闪烁
                //设置标志位，推出修改模式
            procCon += 0x10;//0001 0000
            if(procCon == 0x40){
                //0100 0000
                procCon &= 0x0F;
            }
            lastCon = procCon;
        }
        //位置+1
        else if(key == 3){
            //闪烁情况下才可以使用
                //开显示的情况
                    //位置加1，注意到上限
                //关显示的情况
                    //没有变化
            procCon += 0x10;//0001 0000
            if(procCon == 0x40){
                //0100 0000
                procCon &= 0x0F;
            }
            lastCon = procCon;
        }
        //位置-1
        else if(key == 4){
            //闪烁情况下才可以使用
                //开显示的情况
                    //位置加1，注意到下限
                //关显示的情况
                    //没有变化
            if(procCon == 0x00){
                //0100 0000
                procCon &= 0x0F;
            }
            else{
                procCon -= 0x10;//0001 0000
            }
            
            
            lastCon = procCon;
        }
        //+1
        else if(key == 5){
            //闪烁的情况才可以用
                //选中位置+1
            switch(procCon & 0xF0){
                case 0x00: speed += 1;break;
                case 0x10: min += 1;break;
                case 0x20: mid += 1;break;
                case 0x30: max += 1;break;
            }
        }
        //-1
        else if(key == 6){
            //闪烁的情况才可以用
                //选中位置-1
            switch(procCon & 0xF0){
                case 0x00: speed -= 1;break;
                case 0x10: min -= 1;break;
                case 0x20: mid -= 1;break;
                case 0x30: max -= 1;break;
            }
        }
        //+5
        else if(key == 7){
            //闪烁的情况才可以用
                //选中位置+5
            switch(procCon & 0xF0){
                case 0x00: speed += 5;break;
                case 0x10: min += 5;break;
                case 0x20: mid += 5;break;
                case 0x30: max += 5;break;
            }
        }
        //-5
        else if(key == 8){
            //闪烁的情况才可以用
                //选中位置-5
            switch(procCon & 0xF0){
                case 0x00: speed -= 5;break;
                case 0x10: min -= 5;break;
                case 0x20: mid -= 5;break;
                case 0x30: max -= 5;break;
            }
        }
        //清空
        else if(key == 9){
            //闪烁的情况才可以用
                //选中位置清空
            switch(procCon & 0xF0){
                case 0x00: speed = 0;break;
                case 0x10: min = 0;break;
                case 0x20: mid = 0;break;
                case 0x30: max = 0;break;
            }
        }
        //取最大
        else if(key == 10){
            //闪烁的情况才可以用
                //选中位置清空
            switch(procCon & 0xF0){
                case 0x00: speed = 100;break;
                case 0x10: min = 40;break;
                case 0x20: mid = 40;break;
                case 0x30: max = 40;break;
            }
        }
        //风扇模式切换
        else if(key == 12){
            //选中位置清空 auto,scon
            LCD12864_ShowString(2,12,"auto");
            LCD12864_ShowString(2,12,"scon");
            //切换后更改显示模式
            
        }
        DS18B20_ConvertT();
        TheT = DS18B20_ReadT();
        ShowT(1,8,TheT);
        LCD12864_ShowFanNum(2,8,speed,3);
        //显示和不显示
        if(procCon & 0x0F){
            //显示
            //上次是显示就刷新数字
            //上次是不显示就刷新数字和字符
            LCD12864_ShowNum(3,6,min);
            LCD12864_ShowNum(3,14,mid);
            LCD12864_ShowNum(3,10,max);
            if(!(lastCon & 0x0F)){
                LCD12864_ShowString(3,2,"min:");
                LCD12864_ShowString(3,10,"mid:");
                LCD12864_ShowString(4,6,"max:");
            }
        }
        else{
            //不显示
            //上次是显示就刷新
            //上次是不显示就什么不做
            if(lastCon & 0x0F){
                LCD12864_ShowString(3,2,"      ");
                LCD12864_ShowString(3,10,"      ");
                LCD12864_ShowString(4,6,"      ");
            }
        }
        //闪烁效果





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
    static unsigned char slice;
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
    else if(line <= speed){
        fan = 1;
    }
    else if(line > speed){
        fan = 0;
    }
}

