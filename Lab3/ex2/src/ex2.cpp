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
#include <ctype.h>
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

    //Board_UARTPutSTR("\r\nHello, World\r\n");
    
    //SysTick
    uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);

    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin led_r(0, 25 ,false ,true, true);
    led_r.write(true);

    int c; //EOF can't be put to char.
    bool upper_case = true;
    bool press_flag = false;
    while(1) {
        Sleep(1);
        // Check button and flip Upper/Lower case on release.
        if(sw1.read()) {
            press_flag = true;
        }
        else if (press_flag) {
            upper_case = upper_case ? false : true;
            led_r.read() ? led_r.write(false) : led_r.write(true);
            press_flag = false;
        }
        // Echo back what we receive
        c = Board_UARTGetChar();
        if(c != EOF) {
            c = upper_case ? toupper(c) : tolower(c);
            if(c == '\n') Board_UARTPutChar('\r'); // precede linefeed with carriage return
            Board_UARTPutChar(c);
            if(c == '\r') Board_UARTPutChar('\n'); // send line feed after carriage return
        }
    }
    return 0;
}
