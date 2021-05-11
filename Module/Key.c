#include <REGX52.H>

//最近按下的值
unsigned char Key_FreshKey = 0;

//返回最新鲜的Key值，取后清0
//相当与从缓存中取值
unsigned char Key(){
	
	unsigned char Temp = 0;
	Temp = Key_FreshKey;
	Key_FreshKey = 0;
	
	return Temp;
}
unsigned char Key_Scan(){
	
	unsigned char keyValue = 0;
	
	     if(P3_0 == 0) keyValue = 1;
	else if(P3_1 == 0) keyValue = 2;
	else if(P3_2 == 0) keyValue = 3;
	else if(P3_3 == 0) keyValue = 4;
	else if(P3_4 == 0) keyValue = 5;
	else if(P3_5 == 0) keyValue = 6;
	else if(P3_6 == 0) keyValue = 7;
	else if(P3_7 == 0) keyValue = 8;
	
	return keyValue;
}

void Key_Loop(){
	//设置静态变量，防止调用时清空数值
	static unsigned char LastStatus,NowStatus;
	LastStatus = NowStatus;
	NowStatus = Key_Scan();
	//开始判断逻辑
	if(LastStatus == 1 && NowStatus == 0){
		Key_FreshKey = 1;
	}
	else if(LastStatus == 2 && NowStatus == 0){
		Key_FreshKey = 2;
	}
	else if(LastStatus == 3 && NowStatus == 0){
		Key_FreshKey = 3;
	}
	else if(LastStatus == 4 && NowStatus == 0){
		Key_FreshKey = 4;
	}
	else if(LastStatus == 5 && NowStatus == 0){
		Key_FreshKey = 5;
	}
	else if(LastStatus == 6 && NowStatus == 0){
		Key_FreshKey = 6;
	}
	else if(LastStatus == 7 && NowStatus == 0){
		Key_FreshKey = 7;
	}
	else if(LastStatus == 8 && NowStatus == 0){
		Key_FreshKey = 8;
	}
}
