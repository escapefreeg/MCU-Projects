#include <reg51.h>
#include "LCD12864.h"

unsigned char result = 0;
unsigned char result1 = 0;
void main(void)
{
	
	LCD12864_Init();
	
	#pragma asm
PROC2:	
		MOV R0,#0;存放次数
		MOV R1,#0;存放循环次数
		MOV DPTR,#3000H
LOOP:	MOVX A,@DPTR;
		CJNE A,#0AAH,NOEQU;不相等跳转
		;PUSH ACC	要不要都行
		MOV A,R0
		ADD A,#1
		MOV R0,A
		;POP ACC	要不要都行
NOEQU:		
		INC DPTR
		INC R1
		CJNE R1,#40H,LOOP
		;存放结果
		MOV A,R0
		MOV DPTR,#3040H
		MOVX @DPTR,A
		;设置标志位
		CJNE A,#05H,NEQ
EQ： 	SETB 0D5H
		LJMP NEXT
NEQ：	JNC BIG 
LOW： 	CLR 0D5H
		LJMP NEXT
BIG：	SETB 0D5H
NEXT:	RET

	#pragma asm

	LCD12864_ShowHexNum(1,3,result,3);
	LCD12864_ShowHexNum(2,3,result1,3);
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



#pragma asm
		;假设数据均已放入响应单元中
PROC1:	MOV DPTR,#2600H
		MOV A,#0
		MOV R0,#50H;数据地址
		MOV R1,#60H;结果地址
		MOV sp,#7FH;设置堆栈指针
LOOP:
		PUSH ACC
		MOVX A,@DPTR;
		ADD A,@R0
		MOV @R1,A
		JB C,CARRY
		LJMP NOCARRY
CARRY:	INC R1		;增加地址
		MOV @R1,#1
		DEC R1		;撤销修改
NOCARRY:
		POP ACC
		INC A
		INC DPTR
		INC R0
		INC R1
		INC R1
		CJNE A,#10H,LOOP
RET
	#pragma endasm
}