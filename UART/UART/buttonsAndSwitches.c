#include <stdint.h>
#include <pic32mx.h>


int getsw( void ) {
    int retval = (PORTD & 0xF00)>>8; // Read from bit 11 through 8 = SW4-SW1, bit shift to least significant nibble
    return retval;         
}

int getbtns( void ) {
    int retval = (PORTD & 0xE0)>>5; // Read from bit 7 through 5 = BTN4-BTN2, bit shift to least significant 3 bits
    return retval; 
}

