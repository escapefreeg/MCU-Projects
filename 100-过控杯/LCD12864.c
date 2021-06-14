#include <REGX52.H>
#include <INTRINS.H>
//引脚配置：
sbit LCD12864_RS=P1^0;
sbit LCD12864_RW=P1^1;
sbit LCD12864_EN=P2^5;
#define LCD12864_DataPort P0

//函数定义：
/**
  * @brief  LCD12864延时函数，11.0592MHz调用可延时1ms
  * @param  无
  * @retval 无
  */
void LCD12864_Delay(unsigned char xms)
{
	unsigned char i, j;
	for(;xms>0;xms--){
		_nop_();
		i = 11;
		j = 199;	
		do
		{
			while (--j);
		} while (--i);
	}	
}

void Delay15us()		//@11.0592MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 38;
	while (--i);
}


/**
  * @brief  LCD12864写命令
  * @param  Command 要写入的命令
  * @retval 无
  */
void LCD12864_WriteCommand(unsigned char Command)
{
	LCD12864_RS=0;
	LCD12864_RW=0;
	LCD12864_DataPort=Command;
	LCD12864_EN=1;
	LCD12864_Delay(1);
	LCD12864_EN=0;
	LCD12864_Delay(1);
}

/**
  * @brief  LCD12864写数据
  * @param  Data 要写入的数据
  * @retval 无
  */
void LCD12864_WriteData(unsigned char Data)
{
	LCD12864_RS=1;
	LCD12864_RW=0;
	LCD12864_DataPort=Data;
	LCD12864_EN=1;
	LCD12864_Delay(1);
	LCD12864_EN=0;
	LCD12864_Delay(1);
}

/**
  * @brief  LCD12864读数据
  * @param  Data 要写出的数据
  * @retval 无
  */
unsigned char LCD12864_ReadData()
{
	unsigned char Data = 0;
	LCD12864_DataPort = 0xff;
	LCD12864_RS=1;
	LCD12864_RW=1;
	LCD12864_EN=1;
	Delay15us();
	Data = LCD12864_DataPort;
	
	LCD12864_EN=0;
	LCD12864_Delay(1);

	return Data;
}


/**
  * @brief  LCD12864设置光标位置
  * @param  Line 行位置，范围：1~4
  * @param  Column 列位置，范围：1~8
  * @retval 无
  */
void LCD12864_SetCursor(unsigned char Line,unsigned char Column)
{
	if(Line==1)
	{
		LCD12864_WriteCommand(0x80+(Column-1));
	}
	else if(Line==2)
	{
		LCD12864_WriteCommand(0x90+(Column-1));
	}
	else if(Line==3)
	{
		LCD12864_WriteCommand(0x88+(Column-1));
	}
	else if(Line==4)
	{
		LCD12864_WriteCommand(0x98+(Column-1));
	}
}

/**
  * @brief  LCD12864初始化函数
  * @param  无
  * @retval 无
  */
void LCD12864_Init()
{         
	LCD12864_WriteCommand(0x30);   //基本指令集          
	LCD12864_WriteCommand(0x02);   //地址归位          
	LCD12864_Delay(5);
	LCD12864_WriteCommand(0x0c);   //整体显示打开,游标关闭          
	LCD12864_WriteCommand(0x06);   //游标右移          
	LCD12864_WriteCommand(0x80);   //设定显示的起始地址          
	LCD12864_WriteCommand(0x01);   //清除显示 
	LCD12864_Delay(5);
}


/**
  * @brief  在LCD12864指定位置上显示一个字符
  * @param  Line 行位置，范围：1~2
  * @param  Column 列位置，范围：1~16
  * @param  Char 要显示的字符
  * @retval 无
  */
void LCD12864_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
	unsigned char HighData = 0;
	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		//first->high
		//second->high
		//third->low
		HighData = LCD12864_ReadData();
		//begin write
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		LCD12864_WriteData(Char);
	}
	//1 3 5 7 9 11 13 15
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		LCD12864_WriteData(Char);
	}
}


/**
  * @brief  在LCD12864指定位置开始显示所给字符串
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  String 要显示的字符串
  * @retval 无
  */
void LCD12864_ShowString(unsigned char Line,unsigned char Column,char *String)
{
	unsigned char i;
	unsigned char HighData = 0;
	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		HighData = LCD12864_ReadData();
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		for(i=0;String[i]!='\0';i++)
		{
			LCD12864_WriteData(String[i]);
		}
	}
	//1 3 5 7 9 11 13 15
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		for(i=0;String[i]!='\0';i++)
		{
			LCD12864_WriteData(String[i]);
		}
	}
	
	
}

/**
  * @brief  返回值=X的Y次方
  */
int LCD12864_Pow(int X,int Y)
{
	unsigned char i;
	int Result=1;
	for(i=0;i<Y;i++)
	{
		Result*=X;
	}
	return Result;
}

/**
  * @brief  在LCD12864指定位置开始显示所给数字
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~65535
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD12864_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	unsigned char HighData = 0;
	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		HighData = LCD12864_ReadData();
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		for(i=Length;i>0;i--)
		{
			LCD12864_WriteData(Number/LCD12864_Pow(10,i-1)%10+'0');
		}
	}
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		for(i=Length;i>0;i--)
		{
			LCD12864_WriteData(Number/LCD12864_Pow(10,i-1)%10+'0');
		}
	}
}

void LCD12864_ShowFanNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	unsigned char HighData = 0;
	unsigned char tem;
	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		HighData = LCD12864_ReadData();
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		for(i=Length;i>0;i--)
		{
			tem = Number/LCD12864_Pow(10,i-1)%10;
			if(tem == 0)
				LCD12864_WriteData(32);
			else
				LCD12864_WriteData(tem+'0');
		}
	}
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		for(i=Length;i>0;i--)
		{
			tem = Number/LCD12864_Pow(10,i-1)%10;
			if(tem == 0)
				LCD12864_WriteData(32);
			else
				LCD12864_WriteData(tem+'0');
		}
	}
}

/**
  * @brief  在LCD12864指定位置开始以有符号十进制显示所给数字
  * @param  Line 起始行位置，范围：1~4
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：-32768~32767
  * @param  Length 要显示数字的长度，范围：1~5
  * @retval 无
  */
void LCD12864_ShowSignedNum(unsigned char Line,unsigned char Column,int Number,unsigned char Length)
{
	unsigned char i;
	unsigned int Number1;
	unsigned char HighData = 0;

	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		HighData = LCD12864_ReadData();
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		if(Number>=0)
		{
			LCD12864_WriteData('+');
			Number1=Number;
		}
		else
		{
			LCD12864_WriteData('-');
			Number1=-Number;
		}
		for(i=Length;i>0;i--)
		{
			LCD12864_WriteData(Number1/LCD12864_Pow(10,i-1)%10+'0');
		}
	}
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		if(Number>=0)
		{
			LCD12864_WriteData('+');
			Number1=Number;
		}
		else
		{
			LCD12864_WriteData('-');
			Number1=-Number;
		}
		for(i=Length;i>0;i--)
		{
			LCD12864_WriteData(Number1/LCD12864_Pow(10,i-1)%10+'0');
		}
	}

	
	
}

/**
  * @brief  在LCD12864指定位置开始以十六进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~0xFFFF
  * @param  Length 要显示数字的长度，范围：1~4
  * @retval 无
  */
void LCD12864_ShowHexNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i,SingleNumber;
	unsigned char HighData = 0;
	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		HighData = LCD12864_ReadData();
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		for(i=Length;i>0;i--)
		{
			SingleNumber=Number/LCD12864_Pow(16,i-1)%16;
			if(SingleNumber<10)
			{
				LCD12864_WriteData(SingleNumber+'0');
			}
			else
			{
				LCD12864_WriteData(SingleNumber-10+'A');
			}
		}
	}
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		for(i=Length;i>0;i--)
		{
			SingleNumber=Number/LCD12864_Pow(16,i-1)%16;
			if(SingleNumber<10)
			{
				LCD12864_WriteData(SingleNumber+'0');
			}
			else
			{
				LCD12864_WriteData(SingleNumber-10+'A');
			}
		}
	}
	
}

/**
  * @brief  在LCD12864指定位置开始以二进制显示所给数字
  * @param  Line 起始行位置，范围：1~2
  * @param  Column 起始列位置，范围：1~16
  * @param  Number 要显示的数字，范围：0~1111 1111 1111 1111
  * @param  Length 要显示数字的长度，范围：1~16
  * @retval 无
  */
void LCD12864_ShowBinNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
	unsigned char i;
	unsigned char HighData = 0;
	//不好处理的地址
	if(Column % 2 == 0){
		LCD12864_SetCursor(Line,Column/2);
		HighData = LCD12864_ReadData();
		LCD12864_SetCursor(Line,Column/2);
		LCD12864_WriteData(HighData);
		for(i=Length;i>0;i--)
		{
			LCD12864_WriteData(Number/LCD12864_Pow(2,i-1)%2+'0');
		}
	}
	else{
		LCD12864_SetCursor(Line,Column/2 + 1);
		for(i=Length;i>0;i--)
		{
			LCD12864_WriteData(Number/LCD12864_Pow(2,i-1)%2+'0');
		}
	}
	
}

