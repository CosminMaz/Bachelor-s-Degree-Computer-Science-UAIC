#include <8051.h>

unsigned char shift;

void main(void){
    //P1 = 0xFF;
    shift = 0xFF;
    while(1){
        P1 = 0;
        //P1 = shift << 1;
        //shift = 0xFF;
        P1 = ~shift;
        shift << 1;   
        /*
        P1 = 0;
        P1 = 0xFF;
        */
    }
}
