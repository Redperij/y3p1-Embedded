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

#define SAMPLEC 50 //Sample count is defined for convenience in order to make tests faster.

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
    //Button init
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    //Array
    uint32_t samples[SAMPLEC] = {0};
    //Counter for received samples
    uint8_t scount = 0;
    bool finished = false;
    
    //Init counter.
    CoreDebug->DEMCR |= 1 << 24;
    DWT->CTRL |= 1;

    volatile static int i = 0 ;
    while(1) {
        while (scount < SAMPLEC)
        {
            if(sw1.read()) {
                uint32_t value = DWT->CYCCNT;
                Sleep(1);
                samples[scount] = DWT->CYCCNT; 
                if (samples[scount] > value) samples[scount] -= value;
                else samples[scount] = 0xffffffff - value + 1 + samples[scount];
                ++scount;
                while(sw1.read());
                Sleep(50);
            }
        }
        if(!finished) {
            for (int i = 0; i < SAMPLEC; ++i) {
                for (int q = i + 1; q < SAMPLEC; ++q) {
                    if (samples[i] > samples[q]) {
                        uint32_t temp = samples[i];
                        samples[i] = samples[q];
                        samples[q] = temp;
                    }
                }
            }

            uint32_t aver = 0;
            for (int i = 0; i < SAMPLEC; ++i) {
                aver += samples[i];
                console.print("%d ", samples[i]);
            }
            console.print("\n");

            aver /= SAMPLEC;
            console.print("Average = %d counter cycles. Or %d microseconds.", aver, 1000);
            finished = true;
        }
        

        
        i++ ;
        __asm volatile ("nop");
    }
    return 0 ;
}
