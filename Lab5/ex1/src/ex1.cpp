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
#include <string>
#include "LiquidCrystal.h"
#include "DigitalIoPin.h"

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
    // LCD pins init.
    DigitalIoPin rs(0, 8, false, true, false);
    DigitalIoPin en(1, 6, false, true, false);
    DigitalIoPin d4(1, 8, false, true, false);
    DigitalIoPin d5(0, 5, false, true, false);
    DigitalIoPin d6(0, 6, false, true, false);
    DigitalIoPin d7(0, 7, false, true, false);
    rs.write(false);
    en.write(false);
    d4.write(false);
    d5.write(false);
    d6.write(false);
    d7.write(false);
    
    //Buttons init.
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin sw2(1, 11 ,true ,true, true);
    DigitalIoPin sw3(1, 9 ,true ,true, true);

    // LCD init.
    LiquidCrystal lcd(&rs, &en, &d4, &d5, &d6, &d7);
    // Configure display geometry.
    lcd.begin(16, 2);

    volatile static int i = 0 ;
    std::string ssw1 = "UP  ";
    std::string ssw2 = "UP  ";
    std::string ssw3 = "UP  ";
    char buf[33];
    while(1) {
        if (sw1.read()) ssw1 = "DOWN";
        else ssw1 = "UP  ";
        if (sw2.read()) ssw2 = "DOWN";
        else ssw2 = "UP  ";
        if (sw3.read()) ssw3 = "DOWN";
        else ssw3 = "UP  ";
        snprintf(buf, 33, "B1    B2    B3  %s  %s  %s", ssw1.c_str(), ssw2.c_str(), ssw3.c_str());
        lcd.print(buf);
        i++ ;
        __asm volatile ("nop");
    }
    return 0 ;
}
