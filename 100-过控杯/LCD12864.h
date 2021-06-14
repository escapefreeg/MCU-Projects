#ifndef __LCD12864_H__
#define __LCD12864_H__

//用户调用函数：
void LCD12864_Init();
void LCD12864_ShowChar(unsigned char Line,unsigned char Column,char Char);
void LCD12864_ShowString(unsigned char Line,unsigned char Column,char *String);
void LCD12864_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD12864_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length);
void LCD12864_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD12864_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length);
void LCD12864_ShowFanNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
#endif
