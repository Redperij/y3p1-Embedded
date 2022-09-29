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
#include "LpcUart.h"
#include "I2C.h"
#include "I2CDevice.h"
#include "Event.h"
#include "EventQueue.h"
#include "StateMachine.h"

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
    SystemCoreClockUpdate();
#if !defined(NO_BOARD_LIB)
    Board_Init();
#endif
#endif
    //SysTick
    uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);
    //UART setup
    LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart uart(cfg);

    //Buttons
    DigitalIoPin sw1(0, 17 ,true ,true, true);
    DigitalIoPin sw2(1, 11 ,true ,true, true);
    
    //I2C device.
    I2C i2c;
    I2CDevice i2c_temp(&i2c, (uint8_t)0x48);
    
    //State machine
    StateMachine msm(&uart, &i2c_temp);
    EventQueue events;

    bool sw1_pressed = false;
    bool sw2_pressed = false;
    while(1) {
        Sleep(1);
        if(sw1.read()) {
            sw1_pressed = true;
        }
        else if(sw1_pressed){
            sw1_pressed = false;
            events.publish(Event(Event::eLunch, 0));
        }
        if(sw2.read()) {
            sw2_pressed = true;
        }
        else if(sw2_pressed){
            sw2_pressed = false;
            events.publish(Event(Event::eNight, 0));
        } 
        //Tick is given every 1 ms.
        events.publish(Event(Event::eTick, 0));
        while (events.pending()) {
            msm.HandleState(events.consume());
        }
    }
    return 0 ;
}
