//采用集成版板子开发
#include <REGX52.H>
#include "LCD12864.h"
#include "MatrixKey.h"
#include "Timer.h"
#include "Delay.h"

sbit Motor_A = P2^2;
sbit Motor_B = P1^5;
sbit Motor_C = P1^6;
sbit Motor_D = P1^7;

sbit DoorCtrl = P1^2;
//选一个引脚作为开门关门的指示灯即可

unsigned char CurLevel = 1,DisLevel = 7;
unsigned char KeyMatrix = 0;
unsigned char INTR1_Count = 0;
unsigned char LiftMode = 1;
unsigned char Motor_Speed = 2;
unsigned char Motor_Forward_Data[4];
unsigned char Motor_Reverse_Data[4];
unsigned char mfi = 0,mri = 0;
unsigned char WaitFlag = 0;
unsigned int WaitCount = 0;
unsigned char LastLiftMode = 1;

void INTR1_Init(){
    //use time
    IT1 = 1;
    EX1 = 1;
    EA = 1;
}

void Delay100us()		//@11.0592MHz
{
	unsigned char i, j;

	i = 2;
	j = 15;
	do
	{
		while (--j);
	} while (--i);
}

void Motor_DataInit(){
    Motor_Forward_Data[0] = 0x8C;//A AB 0b10001100
    Motor_Forward_Data[1] = 0x46;//B BC 0b01000110
    Motor_Forward_Data[2] = 0x23;//C CD 0b00100011
    Motor_Forward_Data[3] = 0x19;//D DA 0b00011001

    Motor_Reverse_Data[0] = 0x13;//D DC 0b00010011
    Motor_Reverse_Data[1] = 0x26;//C CB 0b00100110
    Motor_Reverse_Data[2] = 0x4C;//B BA 0b01001100
    Motor_Reverse_Data[3] = 0x89;//A AD 0b10001001
}

void MotorStop(){
    Motor_A = 0;
    Motor_B = 0;
    Motor_C = 0;
    Motor_D = 0;
}

void MotorForward(){

    Motor_A = (Motor_Forward_Data[mfi]&(0x80));
    Motor_B = (Motor_Forward_Data[mfi]&(0x40));
    Motor_C = (Motor_Forward_Data[mfi]&(0x20));
    Motor_D = (Motor_Forward_Data[mfi]&(0x10));
    Delay100us();
    Motor_A = (Motor_Forward_Data[mfi]&(0x08));
    Motor_B = (Motor_Forward_Data[mfi]&(0x04));
    Motor_C = (Motor_Forward_Data[mfi]&(0x02));
    Motor_D = (Motor_Forward_Data[mfi]&(0x01));
    Delay100us();
    mfi = (mfi + 1) % 4;

}

void MotorReverse(){
    
    Motor_A = (Motor_Reverse_Data[mri]&(0x80));
    Motor_B = (Motor_Reverse_Data[mri]&(0x40));
    Motor_C = (Motor_Reverse_Data[mri]&(0x20));
    Motor_D = (Motor_Reverse_Data[mri]&(0x10));
    Delay100us();
    Motor_A = (Motor_Reverse_Data[mri]&(0x08));
    Motor_B = (Motor_Reverse_Data[mri]&(0x04));
    Motor_C = (Motor_Reverse_Data[mri]&(0x02));
    Motor_D = (Motor_Reverse_Data[mri]&(0x01));
    Delay100us();
    mri = (mri + 1) % 4;
}

void main(void){

    //init
    LCD12864_Init();
    Timer1_Init();
    INTR1_Init();
    Motor_DataInit();
    //控制门相关，默认门是关闭的
    DoorCtrl = 0;
    //display info
    LCD12864_ShowString(1,3,"ZNDTKZQ");
    LCD12864_ShowString(2,1,"lihongke");
    LCD12864_ShowString(2,5,"changjun");
    Delayxms(1000);

    LCD12864_ShowString(3,1,"CurLevel:");
    LCD12864_ShowString(4,1,"DisLevel:");
    LCD12864_ShowNum(4,6,DisLevel,1);
    //main program
    while(1){
        KeyMatrix = MatrixKey();
        if(KeyMatrix > 0 && KeyMatrix <= 9) {
            DisLevel = KeyMatrix;
            WaitFlag = 0;
            WaitCount = 0;
            LCD12864_ShowNum(4,6,DisLevel,1);
            }

        if(CurLevel == DisLevel){
            //停止运行
            //步进电机停
            if(WaitCount >= 3000){
                DisLevel = 5;
                WaitFlag = 0;
                WaitCount = 0;
                LCD12864_ShowNum(4,6,DisLevel,1);
            }
            LastLiftMode = LiftMode;LiftMode = 2;
            if(!WaitFlag) WaitCount = 0;
            WaitFlag = 1;
            MotorStop();
            DoorCtrl = 1;
        }
        else if(CurLevel<DisLevel){
            //上升模式
            LastLiftMode = LiftMode;LiftMode = 1;
            //步进电机正转
            MotorForward();
            DoorCtrl = 0;
        }
        else{
            //下降模式
            LastLiftMode = LiftMode;LiftMode = 0;
            //步进电机反转
            MotorReverse();
            DoorCtrl = 0;
        }
        Motor_Speed = LastLiftMode ^ LiftMode;
        if(Motor_Speed != 0){P2_3 = 0;Delayxms(100);P2_3 = 1;}
        LCD12864_ShowNum(3,6,CurLevel,1);
    }
}

//外部中断1 p33
void INTR1_Routine() interrupt 2
{ 	
    INTR1_Count++;
    if(INTR1_Count>=3)
    {
        INTR1_Count = 0;
        switch(LiftMode){
            //电梯处于上升模式
            case 0:CurLevel = (CurLevel<=1)?1:CurLevel - 1;break;
            //电梯处于下降模式
            case 1:CurLevel = (CurLevel>=9)?9:CurLevel + 1;break;
            case 2:break;
        }
    }
}

//定时器中断1
void Timer1_Routine() interrupt 3
{
	static unsigned int T1Count = 0;
	TL1 = 0x66;		//设置定时初值
	TH1 = 0xFC;		//设置定时初值
	T1Count++;
    if(WaitFlag) {
        WaitCount++;
        }
	if(T1Count>=20)
	{
		MatrixKey_Loop();
        T1Count=0;
	}
}


