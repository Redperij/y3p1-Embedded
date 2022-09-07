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
#include "PrintITM.h"
#include "DigitalIoPin.h"
#include "Menu.h"

int main(void) {
    //Hardware init.
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
    Menu main_menu;
    
    //Init buttons
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin sw2(1, 11 ,true ,true, true);
    DigitalIoPin sw3(1, 9 ,true ,true, true);
    //Flags to track button presses.
    bool sw1_state = false;
    bool sw2_state = false;
    bool sw3_state = false;

    main_menu.print();
    volatile static int i = 0 ;
    //Main loop.
    while(1) {
        if(sw1.read()) {
            sw1_state = true;
        }
        else if(sw1_state){
            main_menu.move_up();
            main_menu.print();
            sw1_state = false;
        }
        if(sw2.read()) {
            sw2_state = true;
        }
        else if(sw2_state){
            main_menu.switch_selected_led();
            main_menu.print();
            sw2_state = false;
        }
        if(sw3.read()) {
            sw3_state = true;
        }
        else if(sw3_state){
            main_menu.move_down();
            main_menu.print();
            sw3_state = false;
        }   

        i++ ;
        __asm volatile ("nop");
    }
    return 0 ;
}
