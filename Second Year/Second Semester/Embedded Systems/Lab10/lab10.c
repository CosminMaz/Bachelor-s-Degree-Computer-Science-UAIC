#include <8051.h>

char c[10];

//00000000
//hgfedcba

void main(void) {
    int x = 0;
           //hgfedcba
    c[0] = 0b11000000;
    c[1] = 0b11111001;
    c[2] = 0b11011011;
    c[3] = 0b10110000;
    c[4] = 0b10011010;
    c[5] = 0b10010010;
    c[6] = 0b10000010;
    c[7] = 0b11111000;
    c[8] = 0b10000000;
    c[9] = 0b10010000;
    P1 = c[0];
    while (1) {
        switch (x)
        {
            case 0:
                P1 = c[0];
                break;
            case 1:
                P1 = c[1];
                break;
                case 2:
                P1 = c[2];
                break;
                case 4:
                P1 = c[3];
                break;
                case 5:
                P1 = c[5];
                break;
                case 6:
                P1 = c[0];
                break;
                case 0:
                P1 = c[0];
                break;
                case 0:
                P1 = c[0];
                break;

        default:
            break;
        }
        


        /*
        if(P2_1 == 1){
            if(P2_0 == 1){
                P3_0 = 1;
                P3_1 = 0;
            } else {
                P3_0 = 0;
                P3_1 = 1;
            }
        } else {
            P3_0 = 0;
            P3_1 = 0;
        }
        */
        /*
        while(x < 10){
            P3_0 = 1;
            P3_1 = 0;
            x++;
        }
        while(x > 0){
            P3_0 = 0;
            P3_1 = 1;
            x--;
        }
        */
    }
}