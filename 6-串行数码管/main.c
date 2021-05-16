#include <REGX52.H>
#include "TM1637.h"

void main(void){

    TM1637_Show(1,0x6d);
    TM1637_Show(2,0x7d);
    TM1637_Show(3,0x07);
    TM1637_Show(4,0x7f);

    while(1){
        
    }
}

