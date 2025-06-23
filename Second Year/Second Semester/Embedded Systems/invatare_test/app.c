#include "8051.h"

unsigned char i;


void main(void) {
    /*
    P0_7 = 1;
    P3_3 = 0;
    P3_4 = 0;
    P1 = 0b10100100;
    */
    i = 0;
    while(1) {
        i++;
       // if(i > 10){
           // i = 0;
           P0_7 = 0;
            P1 = 0b10110000;
            P3_3 = 0;
            P3_4 = 1;
            P0_7 = 1;
       // } 
            
        
        //if(i == 20){
            i = 0;
            P0_7 = 0;
            P1 = 0b10011001;
            P3_3 = 0;
            P3_4 = 0;
            P0_7 = 1;   
      //  }
    }
}


/*
#include <8051.h>

// 7-segment codes for digits 0-9 (common cathode)
const unsigned char segment_codes[10] = {
    0b11000000, // 0
    0b11111001, // 1
    0b10100100, // 2
    0b10110000, // 3
    0b10011001, // 4
    0b10010010, // 5
    0b10000010, // 6
    0b11111000, // 7
    0b10000000, // 8
    0b10010000  // 9
};

// Small delay for visual persistence
void delay() {
    unsigned int i, j;
    for(i = 0; i < 100; i++)
    for(j = 0; j < 100; j++);
}

// Extract 4 digits from a number (least significant digit first)
void extract_digits(unsigned int num, unsigned char digits[4]) {
    for(int i = 0; i < 4; i++) {
        digits[i] = num % 10;
        num /= 10;
    }
}

// Set which digit to activate via P3.3 and P3.4
void select_display(unsigned char digit_index) {
    unsigned char mask = P3 & 0b11100111; // Clear bits 3 and 4
    P3 = mask | ((digit_index & 0x03) << 3); // Set bits 3 and 4
}

int main() {
    P0_7 = 1;             
    unsigned int number = 4925;
    unsigned char digits[4];
    unsigned char i = 0;
    
    extract_digits(number, digits); // Break number into individual digits
    
    while(1) {
        P0_7 = 0;             // Disable all displays first
        P1 = segment_codes[digits[i]]; // Set segments for current digit
        select_display(i);   // Select correct display via P3.4:3
        P0_7 = 1;             // Enable the selected display
        delay();              // Let the digit be visible
        i = (i + 1) % 4;      // Next digit
    }
}

*/
/*
#include <8051.h>

// State definitions
#define STATE_WAIT 0
#define STATE_KEY_PRESSED 1

// Special key definitions
#define KEY_STAR 10
#define KEY_HASH 11
#define KEY_NONE 255

// Function to scan keypad and return pressed key
unsigned char getKeyPressed() {
    unsigned char key = KEY_NONE;
    
    // Scan Row 0 (keys 1, 2, 3)
    P0 = 0b11111110;  // Enable row 0, disable others
    if (P0_4 == 0) key = 1;      // Col 0
    else if (P0_5 == 0) key = 2; // Col 1  
    else if (P0_6 == 0) key = 3; // Col 2
    
    // Scan Row 1 (keys 4, 5, 6)
    if (key == KEY_NONE) {
        P0 = 0b11111101;  // Enable row 1, disable others
        if (P0_4 == 0) key = 4;      // Col 0
        else if (P0_5 == 0) key = 5; // Col 1
        else if (P0_6 == 0) key = 6; // Col 2
    }
    
    // Scan Row 2 (keys 7, 8, 9)
    if (key == KEY_NONE) {
        P0 = 0b11111011;  // Enable row 2, disable others
        if (P0_4 == 0) key = 7;      // Col 0
        else if (P0_5 == 0) key = 8; // Col 1
        else if (P0_6 == 0) key = 9; // Col 2
    }
    
    // Scan Row 3 (keys *, 0, #)
    if (key == KEY_NONE) {
        P0 = 0b11110111;  // Enable row 3, disable others
        if (P0_4 == 0) key = KEY_STAR;    // * key
        else if (P0_5 == 0) key = 0;      // 0 key
        else if (P0_6 == 0) key = KEY_HASH; // # key
    }
    
    return key;
}

// Function to display a digit on specified 7-segment display
void displayDigit(unsigned char display_pos, unsigned char digit) {
    // Select display (P3.3 and P3.4 control display selection)
    // Display 0: P3.3=0, P3.4=0
    // Display 1: P3.3=1, P3.4=0  
    // Display 2: P3.3=0, P3.4=1
    // Display 3: P3.3=1, P3.4=1
    P3_3 = (display_pos & 0x01) ? 1 : 0;
    P3_4 = (display_pos & 0x02) ? 1 : 0;
    
    // 7-segment patterns (common cathode, 0 = segment on)
    switch (digit) {
        case 0: P1 = 0b11000000; break;  // 0
        case 1: P1 = 0b11111001; break;  // 1
        case 2: P1 = 0b10100100; break;  // 2
        case 3: P1 = 0b10110000; break;  // 3
        case 4: P1 = 0b10011001; break;  // 4
        case 5: P1 = 0b10010010; break;  // 5
        case 6: P1 = 0b10000010; break;  // 6
        case 7: P1 = 0b11111000; break;  // 7
        case 8: P1 = 0b10000000; break;  // 8
        case 9: P1 = 0b10010000; break;  // 9
        case KEY_STAR: P1 = 0b01111111; break;  // * (display as dash)
        case KEY_HASH: P1 = 0b10001001; break;  // # (display as H)
        default: P1 = 0b11111111; break;        // blank
    }
}

// Simple delay function
void delay(unsigned int count) {
    unsigned int i, j;
    for (i = 0; i < count; i++) {
        for (j = 0; j < 100; j++);
    }
}

void main(void) {
    unsigned char keys[4];
    unsigned char key_count = 0;
    unsigned char current_key;
    unsigned char state = STATE_WAIT;
    unsigned char display_index = 0;
    unsigned char i;
    
    // Initialize arrays
    for (i = 0; i < 4; i++) {
        keys[i] = 255; // Initialize with blank
    }
    
    // Key reading phase
    while (key_count < 4) {
        current_key = getKeyPressed();
        
        switch (state) {
            case STATE_WAIT:
                if (current_key != KEY_NONE) {
                    // Key pressed
                    if (current_key == KEY_STAR || current_key == KEY_HASH) {
                        // Terminate early on * or #
                        break;
                    }
                    
                    // Store the key
                    keys[key_count] = current_key;
                    key_count++;
                    state = STATE_KEY_PRESSED;
                }
                break;
                
            case STATE_KEY_PRESSED:
                if (current_key == KEY_NONE) {
                    // Key released, wait for next key
                    state = STATE_WAIT;
                }
                break;
        }
        
        delay(10); // Small delay for debouncing
    }
    
    // Display phase - multiplex the displays
    while (1) {
        // Display current digit
        if (keys[display_index] != 255) {
            displayDigit(display_index, keys[display_index]);
        } else {
            displayDigit(display_index, 255); // Blank display
        }
        
        delay(5); // Display time for each digit
        
        // Move to next display
        display_index++;
        if (display_index >= 4) {
            display_index = 0;
        }
    }
}
*/
/*
#include <8051.h>

unsigned char value;
char c[10];
void main(void){
    c[0] = 0b11000000;
    c[1] = 0b11111001;
    c[2] = 0b10100100;
    c[3] = 0b10110000;
    c[4] = 0b10011001;
    c[5] = 0b10010010;
    c[6] = 0b10000010;
    c[7] = 0b11111000;
    c[8] = 0b10000000;
    c[9] = 0b10010000;
    P3 = 0;
    
    P0 = 0b01110111;///P0_7 = 0, P0_3 = 0
    if(P0_6 == 0){
        value = 1;
    }
    else if(P0_5 == 0){
        value = 2;
    }
    else if(P0_4 == 0){
        value = 3;
    }
    
    P0 = 0b01111011; ///P0_2 = 0
    if(P0_6 == 0){
        value = 4;
    }
    else if(P0_5 == 0){
        value = 5;
    }
    else if(P0_4 == 0){
        value = 6;
    }
    P0 = 0b011111101;
    if(P0_6 == 0){
        value = 7;
    }
    else if(P0_5 == 0){
        value = 8;
    }
    else if(P0_4 == 0){
        value = 9;
    }
    P0 = 0b011111110;
    if(P0_5 == 0)
    value = 0;
    while(1){
        P1 = c[value];
    }
}

*/
/*
#include <8051.h>

main(){
    P3 = 0;
    unsigned char value;
    P0 = 0b01110111;///P0_7 = 0, P0_3 = 0
        if(P0_6 == 0){
            value = 1;
        }
        else if(P0_5 == 0){
            value = 2;
        }
        else if(P0_4 == 0){
            value = 3;
        }

    P0 = 0b01111011; ///P0_2 = 0
        if(P0_6 == 0){
            value = 4;
        }
        else if(P0_5 == 0){
            value = 5;
        }
        else if(P0_4 == 0){
            value = 6;
        }
    P0 = 0b011111101;
        if(P0_6 == 0){
            value = 7;
        }
        else if(P0_5 == 0){
            value = 8;
        }
        else if(P0_4 == 0){
            value = 9;
        }
    P0 = 0b011111110;
        if(P0_5 == 0)
            value = 0;
    while(1){
        switch (value)
        {
        case 0:
            P1 = 0b11000000;
            break;
        case 1:
            P1 = 0b11111001;
            break;
        case 2:
            P1 = 0b10100100;
            break;

        case 3:
            P1 = 0b101100001;
            break;
        
        case 4: 
            P1 = 0b10011001;
            break;
        
        case 5:
            P1 = 0b10010010;
            break;

        case 6:
            P1 = 0b10000010;
            break;

        case 7:
            P1 = 0b11111000;
            break;

        case 8:
            P1 = 0b10000000;
            break;

        case 9:
            P1 = 0b10010000;
            break;
        default:
            break;
        }
    }
}
*/
/*
#include <8051.h>

char c[10];
char m[4][3] = {{1, 2, 3},
{4, 5, 6},
{7, 8, 9},
{'*', 0, '#'}};
int x;
int y;
void main(void) {
    c[0] = 0b11000000;
    c[1] = 0b11111001;
    c[2] = 0b10100100;
    c[3] = 0b10110000;
    c[4] = 0b10011001;
    c[5] = 0b10010010;
    c[6] = 0b10000010;
    c[7] = 0b11111000;
    c[8] = 0b10000000;
    c[9] = 0b10010000;
    
    P3 = 0;
    unsigned char value;
    P0 = 0b01110111;///P0_7 = 0, P0_3 = 0
    if(P0_6 == 0){
        value = 1;
    }
    else if(P0_5 == 0){
        value = 2;
    }
    else if(P0_4 == 0){
        value = 3;
    }
    
    P0 = 0b01111011; ///P0_2 = 0
    if(P0_6 == 0){
        value = 4;
    }
    else if(P0_5 == 0){
        value = 5;
    }
    else if(P0_4 == 0){
        value = 6;
    }
    P0 = 0b011111101;
    if(P0_6 == 0){
        value = 7;
    }
    else if(P0_5 == 0){
        value = 8;
    }
    else if(P0_4 == 0){
        value = 9;
    }
    P0 = 0b011111110;
    if(P0_5 == 0)
    value = 0;
    while(1){
        switch (value)
        {
            case 0:
            P1 = 0b11000000;
            break;
            case 1:
            P1 = 0b11111001;
            break;
            case 2:
            P1 = 0b10100100;
            break;
            
            case 3:
            P1 = 0b101100001;
            break;
            
            case 4: 
            P1 = 0b10011001;
            break;
            
            case 5:
            P1 = 0b10010010;
            break;
            
            case 6:
            P1 = 0b10000010;
            break;
            
            case 7:
            P1 = 0b11111000;
            break;
            
            case 8:
            P1 = 0b10000000;
            break;
            
            case 9:
            P1 = 0b10010000;
            break;
            default:
            break;
        }
    }
}



int x;
int temp;
int i;
int j;
void main(void) {
    //hgfedcba
    c[0] = 0b11000000;
    c[1] = 0b11111001;
    c[2] = 0b10100100;
    c[3] = 0b10110000;
    c[4] = 0b10011001;
    c[5] = 0b10010010;
    c[6] = 0b10000010;
    c[7] = 0b11111000;
    c[8] = 0b10000000;
    c[9] = 0b10010000;
    x = 1234;
    while(1) {
        temp = x;
        P0_7 = 0;
        for(i = 0; i < 4; i++) {
            switch (i)
            {
                case 0:
                P3_3 = 0;
                P3_4 = 0;
                break;
                case 1:
                P3_3 = 1;
                P3_4 = 0;
                break;
                case 2:
                P3_3 = 0;
                P3_4 = 1;
                break;
                case 3:
                P3_3 = 1;
                P3_4 = 1;
                break;
            }
            P0_7 = 0;
            P1 = c[temp % 10];
            temp = temp / 10;
            P0_7 = 1;
            for(j = 0; j < 100; j++){};
        } 
    }
}
*/
