#include <8051.h>

volatile unsigned char shift;

void f() __interrupt(1) {

}
void main(void) {
    TMOD = 0b0000001;
    shift = 0xFF;
    while(1) {
        P1 = 0;
        TH0 = 1000/256;
        TL0 = 1000%256;
        TR0 = 1;
        while(TF0 == 0){};
        P1 = ~shift;
        shift << 1;
        TF0 = 0;
    }
}