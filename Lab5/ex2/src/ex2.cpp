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
#include <ctime>
#include "LiquidCrystal.h"
#include "DigitalIoPin.h"
#include "RealTimeClock.h"
#include <chrono>

static RealTimeClock *rtc;

#ifdef __cplusplus 
extern "C" { 
#endif 
/** 
 * @brief Handle interrupt from SysTick timer 
 * @return  Nothing 
 */ 
void SysTick_Handler(void) 
{ 
    if(rtc != NULL) rtc->tick(); 
} 
#ifdef __cplusplus 
} 
#endif

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

    //Set clock.
    RealTimeClock clk(1000); //1000 interrupts before 1 second passes.
    rtc = &clk;

    //SysTick
    uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);

    //Buttons init.
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin sw2(1, 11 ,true ,true, true);
    DigitalIoPin sw3(1, 9 ,true ,true, true);

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
    // LCD init.
    LiquidCrystal lcd(&rs, &en, &d4, &d5, &d6, &d7);
    // LCD configure display geometry.
    lcd.begin(16, 2);

    char buf[33];
    tm curt;
    while(1) {
        __WFI();
        clk.gettime(&curt);
        snprintf(buf, 33, "%2d:%02d:%02d", curt.tm_hour, curt.tm_min, curt.tm_sec);
        lcd.print(buf);
    }
    return 0 ;
}
