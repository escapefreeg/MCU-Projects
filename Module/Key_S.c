//暂时和原来的保持一致
#include <REGX52.H>
#include "Delay.h"
unsigned char Key(){
	
	unsigned char keyValue = 0;
	
	     if(P3_0 == 0) {Delayxms(20);while(P3_0 == 0);Delayxms(20);keyValue = 1;}
	else if(P3_1 == 0) {Delayxms(20);while(P3_1 == 0);Delayxms(20);keyValue = 2;}
	else if(P3_2 == 0) {Delayxms(20);while(P3_2 == 0);Delayxms(20);keyValue = 3;}
	else if(P3_3 == 0) {Delayxms(20);while(P3_3 == 0);Delayxms(20);keyValue = 4;}
	else if(P3_4 == 0) {Delayxms(20);while(P3_4 == 0);Delayxms(20);keyValue = 5;}
	else if(P3_5 == 0) {Delayxms(20);while(P3_5 == 0);Delayxms(20);keyValue = 6;}
	else if(P3_6 == 0) {Delayxms(20);while(P3_6 == 0);Delayxms(20);keyValue = 7;}
	else if(P3_7 == 0) {Delayxms(20);while(P3_7 == 0);Delayxms(20);keyValue = 8;}
	
	return keyValue;
}

