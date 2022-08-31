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

/**
 * @brief Initialises SW1 button as a digital input with pullup resistor and inverted result.
 * Assumption: 1 - pressed; 0 - not pressed.
 */
void SW1_Init() {
    Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 17, (IOCON_MODE_PULLUP | IOCON_DIGMODE_EN | IOCON_INV_EN));
    Chip_GPIO_SetPinDIRInput(LPC_GPIO, 0, 17);
}

/**
 * @brief Reads state of the button. 
 * @return bool Button state. True = pressed.
 */
bool SW1_read() {
    return (Chip_GPIO_ReadValue(LPC_GPIO, 0) << 14) >> 31;
}

int main(void) {

#if defined (__USE_LPCOPEN)
    // Read clock settings and update SystemCoreClock variable
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    // Set up and initialise all required blocks and
    // functions related to the board hardware
    Board_Init();
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif

    //Initialising switch 1.
    SW1_Init();
    Board_LED_Set(0, false);

    uint32_t sysTickRate;

    /* The sysTick counter only has 24 bits of precision, so it will
	    overflow quickly with a fast core clock. You can alter the
	    sysTick divider to generate slower sysTick clock rates. */
	Chip_Clock_SetSysTickClockDiv(1);

	/* A SysTick divider is present that scales the sysTick rate down
	   from the core clock. Using the SystemCoreClock variable as a
	   rate reference for the SysTick_Config() function won't work,
	   so get the sysTick rate by calling Chip_Clock_GetSysTickClockRate() */
	sysTickRate = Chip_Clock_GetSysTickClockRate();

	/* Enable and setup SysTick Timer at a periodic rate */
	SysTick_Config(sysTickRate / 1000);
    
    // Force the counter to be placed into memory
    //volatile static int i = 0 ;
    // Enter an infinite loop, just incrementing a counter
    while(1) {
        if (SW1_read()) {
            Board_LED_Set(0, true);
            Sleep(333);
            Board_LED_Set(0, false);
            Board_LED_Set(1, true);
            Sleep(333);
            Board_LED_Set(1, false);
            Board_LED_Set(2, true);
            Sleep(333);
            Board_LED_Set(2, false);
        }
        
        //i++ ;
        // "Dummy" NOP to allow source level single
        // stepping of tight while() loop
        //__asm volatile ("nop");
    }
    return 0 ;
}
