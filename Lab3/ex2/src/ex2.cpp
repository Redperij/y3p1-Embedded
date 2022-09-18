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
#include "DigitalIoPin.h"
#include "MorseSender.h"

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

    //Morse init.
    DigitalIoPin a0(0, 8, false, true, false);
    DigitalIoPin led_r(0, 25, false, true, true);
    MorseSender ms(&a0, &led_r);

#if 0
    //Test strings
    ms.send("SOS\n\r");
    ms.send("TTT\n\r");
    ms.send("AAA\n\r");
    ms.send("Save me from this nightmare.\n\r");
    ms.send("You'd better     work as ; expected!\n\r");
#endif


    int c; //EOF can't be put to char.
    char str[81];
    volatile static int q = 0;
    unsigned int i = 0;
    while(1) {
        // Echo back what we receive
        c = Board_UARTGetChar();
        if(c != EOF) {
            if(c == '\n') Board_UARTPutChar('\r'); // precede linefeed with carriage return
            Board_UARTPutChar(c);
            if(c == '\r') Board_UARTPutChar('\n'); // send line feed after carriage return
            if(i < 79) {
                if (c == '\n' || c == '\r') {
                    str[i] = '\n';
                    str[i + 1] = '\0';
                    ms.send(str);
                    i = 0;
                }
                else {
                    str[i] = (char) c;
                    ++i;
                }
            }
            else {
                str[i] = (char) c;
                str[i + 1] = '\0';
                ms.send(str);
                i = 0;
            }
        }
        q++;
        __asm volatile ("nop");
    }
    return 0;
}
