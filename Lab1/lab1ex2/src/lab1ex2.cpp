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

    //SysTick
    uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);

    // Init buttons
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin sw2(1, 11 ,true ,true, true);
    DigitalIoPin sw3(1, 9 ,true ,true, true);

    // Force the counter to be placed into memory
    //volatile static int i = 0 ;
    // Variables to remember time.
    int r = 0, g = 0, b = 0;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        //Button handling
        //Red

        if(sw1.read()) {
            r = 0;
            Board_LED_Set(0, true);
        }
        else if(r == 1000) {
            Board_LED_Set(0, false);
        }
        //Green
        if(sw2.read()) {
            g = 0;
            Board_LED_Set(1, true);
        }
        else if(g == 1000) {
            Board_LED_Set(1, false);
        }
        //Blue
        if(sw3.read()) {
            b = 0;
            Board_LED_Set(2, true);
        }
        else if(b == 1000) {
            Board_LED_Set(2, false);
        }


        //Loop conclusion.
        Sleep(10);
        r += 10;
        g += 10;
        b += 10;

        //i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        //__asm volatile ("nop");
    }
    return 0 ;
}
