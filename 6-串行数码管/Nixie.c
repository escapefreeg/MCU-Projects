#include <REGX52.H>




sbit Nixie_DU = P2^0;
sbit Nixie_WE = P2^1;



//数码管段码表 0-9 a-f - 空
unsigned char NixieTable[]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
							,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40,0x00};
//数码管位码表
unsigned char NixieSelect[]={0xFF,0xFE,0xFD,0xFB,0xF7,0xEF,0xDF,0xBF,0x7F};
//数码管缓存表
unsigned char NixieBuf[] = {17,17,17,17,17,17,17,17,17};

//调用时，Number取 0-17
//16：-
//17：空
void Nixie(unsigned char Location,Number){
	NixieBuf[Location] = Number;
}

// //禁用数码管  -> 暂时没什么用了
// void Nixie_Freeze(void){
// 	//数码管一个也不选
// 	Nixie_WE = 1;
// 	P0 = 0xFF;
// 	Nixie_WE = 0;	
// }

//数码管显示子函数
void Nixie_Scan(unsigned char Location,Number)
{
	
	//段码清0，消影
	Nixie_DU = 1;
	P0 = 0x00;
	Nixie_DU = 0;
	
	Nixie_WE = 1;
	P0 = NixieSelect[Location];
	Nixie_WE = 0;
	Nixie_DU = 1;
	P0 = NixieTable[Number];
	Nixie_DU = 0;
	
	
}

void Nixie_Loop(){
	//每次只显示一个数码管内容
	//减少中断调用的负担
	//同时也节约CPU资源
	static unsigned char i = 1;
	Nixie_Scan(i,NixieBuf[i]);
	i++;
	if(i >= 9)i = 1;
}