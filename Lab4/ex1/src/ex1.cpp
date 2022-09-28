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
#include "I2CHandler.h"
#include "LpcUart.h"
#include <string>

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
    // Set the LED to the state of "On"
    Board_LED_Set(0, true);
#endif
#endif
    //Activity LED off
    Board_LED_Set(0, false);
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

    //Button
    DigitalIoPin sw3(1, 9 ,true ,true, true);
    //I2C device.
    I2CHandler i2c_temp((uint8_t)0x48);

    volatile static int i = 0 ;
    unsigned int flipper = 0;
    uint8_t transmit[2] = {1, 0};
    uint16_t transmit_s = 1;
    uint8_t read = 0;
    uint16_t read_s = 1;
    std::string str;
    char mes[127];
    while(1) {
        switch (flipper)
        {
        case 0: //Get the value of control reg, with standby.
            transmit[0] = 0x01;
            transmit_s = 1;
            read = 0;
            read_s = 1;
            flipper = 1;
            str = "Control reg, standby?";
            break;
        case 1: //Get the value of control reg, remove standby.
            transmit[0] = 0x01;
            transmit[1] = 0x00;
            transmit_s = 2;
            read = 0;
            read_s = 1;
            flipper = 2;
            str = "Control reg, remove standby";
            break;
        case 2: //Get the value of control reg, no standby.
            transmit[0] = 0x01;
            transmit_s = 1;
            read = 0;
            read_s = 1;
            flipper = 3;
            str = "Control reg, no standby";
            break;
        case 3: //Get the value of temp reg, no standby.
            transmit[0] = 0x00;
            transmit_s = 1;
            read = 0;
            read_s = 1;
            flipper = 4;
            str = "Temp reg, no standby";
            break;
        case 4: //Get the value of control reg put standby.
            transmit[0] = 0x01;
            transmit[1] = 0x80;
            transmit_s = 2;
            read = 0;
            read_s = 1;
            flipper = 5;
            str = "Control reg, set standby";
            break;
        case 5: //Get the value of temp reg with standby.
            transmit[0] = 0x00;
            transmit_s = 1;
            read = 0;
            read_s = 1;
            flipper = 6;
            str = "Temp reg, standby";
            break;
        case 6: //Get the value of control reg put standby.
            transmit[0] = 0x01;
            transmit[1] = 0x80;
            transmit_s = 2;
            read = 0;
            read_s = 1;
            flipper = 0;
            str = "Control reg, set standby";
            break;
        default:
            break;
        }
        while(!sw3.read());
        while(sw3.read());
        i2c_temp.SetupXferRecAndExecute(transmit, transmit_s, &read, read_s);
        snprintf(mes, 127, "%s: 0x%02x\r\n", str.c_str(), read);
        uart.write(mes);
        i++;
        __asm volatile ("nop");

    }
    return 0 ;
}
