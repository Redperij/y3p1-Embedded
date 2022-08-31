/*
===============================================================================
 Name        : lab1ex3.cpp
 Author      : Daniil Marinec
 Version     : 0.0.1
 Copyright   : Intellectual property is a disease.
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
#include "Dice.h"

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

    Board_LED_Set(0, false);

    // Init buttons
    DigitalIoPin pc0(0, 8, true, true, true);
    DigitalIoPin pc1(1, 6, true, true, true);
    // Init dice
    Dice d;

    // Force the counter to be placed into memory
    volatile static int i = 0 ;
    uint8_t counter = 0;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        
        if(pc0.read()) {
            d.Show(7);
            while(pc0.read());
            d.Show(0);
        }
        if(pc1.read()) {
            d.Show(0);
            while(pc1.read()) {
                if(counter + 1 > 6) counter = 1;
                else ++counter;
            }
        }
        if(counter) {
            d.Show(counter);
        }

        i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        __asm volatile ("nop");
    }
    return 0 ;
}
/* Some port stuff leftover
    //Main//
	//Buttons
	this->pc[0](0, 8, true, true, true);
	this->pc[1](1, 6, true, true, true);

	//Additional//
	//Buttons
	this->pc[2](1, 8, true, true, true); //No prototype
	this->pc[3](0, 5, true, true, true); //No old, prototype
	this->pb[1](1, 0, true, true, true); //No prototype
	//Some input
	this->pd[0](1, 10, true, true, true);
	//IO (button on old gambling shield)
	this->pb[0](0, 24, true, true, true); //No prototype
	//Leds (are buttons on the old gambling shield)
	this->pb[2](0, 27, false); //No prototype
	this->pb[3](0, 28, false); //No prototype
	//Leds
	this->pb[4](0, 12, false); //No old, prototype
	this->pb[5](0, 14, false); //No old, prototype
*/