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

void delayMicroseconds(unsigned int us) 
{ 
    // calculate compare value
    uint64_t stop_time = Chip_Clock_GetSystemClockRate() / 1000000 * us;
    // disable RIT – compare value may only be changed when RIT is disabled
    Chip_RIT_Disable(LPC_RITIMER);
    // set compare value to RIT
    Chip_RIT_SetCompareValue(LPC_RITIMER, stop_time);
    // clear RIT counter (so that counting starts from zero)
    Chip_RIT_SetCounter(LPC_RITIMER, 0);
    // enable RIT
    Chip_RIT_Enable(LPC_RITIMER);
    // wait until RIT Int flag is set
    while(!Chip_RIT_GetIntStatus(LPC_RITIMER));
    // disable RIT
    Chip_RIT_Disable(LPC_RITIMER);
    // clear RIT Int flag
    Chip_RIT_ClearIntStatus(LPC_RITIMER);
} 

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialize all required blocks and
    // functions related to the board hardware
    Board_Init();
#endif
#endif
    // Initialise RIT.
    Chip_RIT_Init(LPC_RITIMER);
    // Test pin.
    DigitalIoPin a0(0, 8, false, true, false);
    a0.write(false);
    
    while(1) {
        //• 55 us high
        a0.write(true);
        delayMicroseconds(55);
        //• 35 us low 
        a0.write(false);
        delayMicroseconds(35);
        //• 40 us high
        a0.write(true);
        delayMicroseconds(40);
        //• 20 us low
        a0.write(false);
        delayMicroseconds(20);
    }
    return 0 ;
}
