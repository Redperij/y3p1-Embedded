/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    Board_UARTPutSTR("\r\nHello, World\r\n");
    Board_UARTPutChar('!');
    Board_UARTPutChar('\r');
    Board_UARTPutChar('\n');
    int c; //EOF can't be put to char.
    while(1) { // echo back what we receive
        c = Board_UARTGetChar();
        if(c != EOF) {
            if(c == '\n') Board_UARTPutChar('\r'); // precede linefeed with carriage return
            Board_UARTPutChar(c);
            if(c == '\r') Board_UARTPutChar('\n'); // send line feed after carriage return
        }
    }
    return 0;
}
