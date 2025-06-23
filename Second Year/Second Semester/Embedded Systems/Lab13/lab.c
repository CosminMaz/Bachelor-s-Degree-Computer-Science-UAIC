#include "8051.h"

char a;
char c[10];
char stare[4];
char m[3][4] = {{1, 2, 3},
                {4, 5, 6}
                {7, 8, 9}};

void main(void) {
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
    stare[0] = 0b10011010;
    stare[1] = 0b10010010;
    stare[2] = 0b10000010;
    
   // stare[3]
    a = 0;
    int i;
    while(1) {
        for(i = 0; i < 4; ++i) {
            P0 = stare[i];
            if(P_6 == 0){
                a = m[i][2];
                break;
            }
            if(P_5 == 0){
                a = m[i][1];
                break;
            }
            if(P_4 == 0){
                a = m[i][0];
                break;
            }
        }                   
        while(1) {
            if(P0_4){
                a = 1;
                break;
            }
            if(P0_5){
                a = 2;
                break;
            }
            if(P0_6) {
                a = 3;
                break;   
            }
        }

        while(1) {
              if(P0_1){
                P1 = 0b11011011;
                break;
              }  
        }
    }
}