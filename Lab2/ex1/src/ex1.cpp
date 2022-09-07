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
#include <atomic>

static volatile std::atomic_int counter;
#ifdef __cplusplus
extern "C" {
#endif
/**
* @brief Handle interrupt from SysTick timer
* @return Nothing
*/
void SysTick_Handler(void)
{
    if(counter > 0) counter--;
}
#ifdef __cplusplus
}
#endif

void Sleep(int ms)
{
    counter = ms;
    while(counter > 0) {
        __WFI();
    }
}

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
    //SysTick
    uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);

    //Init ITM wrapper
    PrintITM console;
    //Init buttons
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin sw2(1, 11 ,true ,true, true);
    DigitalIoPin sw3(1, 9 ,true ,true, true);
    //Integers to track button presses.
    uint32_t sw1t = 0;
    uint32_t sw2t = 0;
    uint32_t sw3t = 0;

    //volatile static int i = 0 ;
    //Main loop.
    while(1) {
        Sleep(10);
    	if(sw1.read()) {
            sw1t += 10;
        }
        else if(sw1t != 0){
            console.print("SW1 pressed for %d ms\n", sw1t);
            sw1t = 0;
        }
        if(sw2.read()) {
            sw2t += 10;
        }
        else if(sw2t != 0){
            console.print("SW2 pressed for %d ms\n", sw2t);
            sw2t = 0;
        }
        if(sw3.read()) {
            sw3t += 10;
        }
        else if(sw3t != 0){
            console.print("SW3 pressed for %d ms\n", sw3t);
            sw3t = 0;
        }       

        //i++ ;
        //__asm volatile ("nop");
    }
    return 0 ;
}
