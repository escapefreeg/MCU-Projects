#include <REGX52.H>
#include "LCD12864.h"
#include "Delay.h"
#include "AT24C02.h"

unsigned char byte1 = 1;
unsigned char byte2 = 2;
unsigned char byte3 = 3;
unsigned char byte4 = 4;

void main(void){
    
	//init
    LCD12864_Init();
    // //读取原来数据 并显示
    // LCD12864_ShowBinNum(1,1,AT24C02_ReadByte(0),8);
    // LCD12864_ShowBinNum(2,1,AT24C02_ReadByte(1),8);
    // LCD12864_ShowBinNum(3,1,AT24C02_ReadByte(2),8);
    // LCD12864_ShowBinNum(4,1,AT24C02_ReadByte(3),8);
    // Delayxms(1000);
    // //写入新的数据
    // AT24C02_WriteByte(0,byte1);
    // Delayxms(5);
    // AT24C02_WriteByte(1,byte2);
    // Delayxms(5);
    // AT24C02_WriteByte(2,byte3);
    // Delayxms(5);
    // AT24C02_WriteByte(3,byte4);
    // Delayxms(5);
    // //清屏 加 转场
    // LCD12864_Init();
    // Delayxms(1000);
    // //显示新的数据
    // LCD12864_ShowBinNum(1,1,AT24C02_ReadByte(0),8);
    // LCD12864_ShowBinNum(2,1,AT24C02_ReadByte(1),8);
    // LCD12864_ShowBinNum(3,1,AT24C02_ReadByte(2),8);
    // LCD12864_ShowBinNum(4,1,AT24C02_ReadByte(3),8);
	LCD12864_ShowNum(1,2,34,2);
	LCD12864_ShowString(2,2,"hello world!");
	LCD12864_ShowSignedNum(3,4,-55,3);
	LCD12864_ShowChar(4,16,'c');
    while(1){
        ;
    }
}


