#include <reg51.h>
#include "LCD12864.h"

unsigned char result = 0;
unsigned char result1 = 0;
void main(void)
{
	
	LCD12864_Init();
	
	
	#pragma asm
	MOV A,#57H
	MOV R0,#63H
	MOV 63H,#0A1H
	ANL	A,#63H	;A=43H
	ORL	63H,A		;A=0E3H
	XRL	A,@R0		;A=42H
	CPL	A			;A=0BDH
	MOV result,A
	#pragma endasm


	LCD12864_ShowHexNum(1,3,result1,3);
	LCD12864_ShowHexNum(2,3,result,3);
	
	while (1)
	{
		;
	}
	
// #pragma asm
// 		MOV 30H,#56H
// 		MOV 31H,#95H
// 		MOV 32H,#54H
// 		MOV 33H,#0E5H	;准备测试数据
// 		MOV 40H,#0
// 		MOV 41H,#0		;将结果空间值清0
// 		MOV R0,#30H
// 		MOV R1,#40H		;间址寄存器设置初值
// LOOP:	MOV A,@R1
// 		ADD A,@R0		
// 		MOV @R1,A		;将R0和结果低位相加
// 		INC R1
// 		MOV A,@R1
// 		ADDC A,#0
// 		MOV @R1,A		
// 		DEC R1			;将相加进位加入结果高位，并重置R1为40H
// 		INC R0			;R0加1，为之后运算做准备
// 	CJNE R0,#34H,LOOP	;循环跳转条件R0<#34H
// 	;结果是224
}