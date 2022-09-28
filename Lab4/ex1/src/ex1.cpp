/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#define DELAY_BETWEEN_I2C 5 //ms (note: Again, used for convinience. 5ms just to make sure that nothing goes wrong.)

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

    //Button
    DigitalIoPin sw3(1, 9 ,true ,true, true);
    //I2C device.
    I2CHandler i2c_temp((uint8_t)0x48);
    //Value from i2c device register.
    uint8_t read = 0x00;
    while(1) {
        if(sw3.read()) {
            while(sw3.read());
            if(i2c_temp.read(0x01, &read, 1)) {
                if(read & 0x40) { // if data_ready bit is set to 1.
                    char msg[32];
                    snprintf(msg, 32, "Status is: 0x%02x\r\n", read);
                    uart.write(msg);
                    Sleep(DELAY_BETWEEN_I2C);
                    if(i2c_temp.read(0x00, &read, 1)) {
                        //read -= 0x1e; //-30 to check negative values.
                        snprintf(msg, 32, "Temperature is: 0x%02x = %dC\r\n", read, (int8_t)read);
                        uart.write(msg);
                    }
                    else uart.write("Unable to connect to temperature sensor.\r\n");
                }
                else uart.write("Data is not ready.\r\n");
            }
            else uart.write("Unable to connect to temperature sensor.\r\n");
        }
        Sleep(DELAY_BETWEEN_I2C);
    }
    return 0 ;
}