#include <reg51.h>
#include "LCD12864.h"

unsigned char result = 0;
unsigned char result1 = 0;
void main(void)
{
	
	LCD12864_Init();
	
	
	#pragma asm
		;假设50H和A中已存入数据
		MOV 51H,#0    ;51H清0,为了放入进位信息
		ADD A,50H
		MOV 50H,A 
		JC HaveC
		SJMP One
HaveC:	MOV 51H,#1
One:	MOV A,#0	  ;结束占位，没有其它意思
	#pragma endasm

	LCD12864_ShowNum(1,3,result,3);
	LCD12864_ShowNum(2,3,result1,3);
	while (1)
	{
		;
	}
	
// 	#pragma asm
// 		MOV 51H,#0    ;51H清0,为了放入进位信息
// 		MOV 50H,#256
// 		MOV A,#5
// 		ADD A,50H
// 		MOV 50H,A
// 		MOV result,A 
// 		JC HaveC
// 		SJMP One
// HaveC:	MOV 51H,#1
// 		MOV result1,#1  ;验证结果
// One:	MOV A,#254
// 	#pragma endasm
}