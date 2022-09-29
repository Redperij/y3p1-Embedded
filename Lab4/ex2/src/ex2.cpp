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
#define STANDBY_TIMEOUT 5000 //ms (Convinience)

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
    
    const uint8_t reg_control = 0x01; //Control register command.
    const uint8_t reg_temp = 0x00; //Temperature register command.
    const uint8_t standby_bit = 0x80; //Standby bit in control register.
    const uint8_t ready_bit = 0x40; //Ready bit in control register.
    uint8_t read = 0x00; //Value from i2c device register.
    unsigned int standby_timer = 0;
    unsigned long long timestamp = 0; //Timestamp for JSON.
    unsigned int sample = 0; //Sample number for JSON.
    while(1) {
        if(sw3.read()) {
            //Remove standby
            standby_timer = 0;
            if(i2c_temp.read(reg_control, &read, 1) && (read & standby_bit)) {
                uint8_t normal_bit = 0x00;

                Sleep(DELAY_BETWEEN_I2C);
                timestamp += DELAY_BETWEEN_I2C;

                if(!i2c_temp.write(reg_control, &normal_bit, 1)) uart.write("Unable to connect to temperature sensor. Standby wasn't removed.\r\n");
                else uart.write("Stanby mode off.\r\n");
            }
            //Wait for button release.
            while(sw3.read());
            //Try to read temperature.
            if(i2c_temp.read(reg_control, &read, 1)) {
                if((read & ready_bit) && !(read & standby_bit)) { // if data_ready bit is set to 1.
                    char msg[128];
#if 0
                    //Debug prints.
                    snprintf(msg, 128, "Status is: 0x%02x\r\n", read);
                    uart.write(msg);
#endif
                    //Carefully seperate i2c transfers.
                    Sleep(DELAY_BETWEEN_I2C);
                    timestamp += DELAY_BETWEEN_I2C;

                    if(i2c_temp.read(reg_temp, &read, 1)) {                        
                        //read -= 0x1e; //-30 to check negative values.
#if 0
                        //Debug prints.
                        snprintf(msg, 128, "Temperature is: 0x%02x = %dC\r\n", read, (int8_t)read);
                        uart.write(msg);
#endif
                        //JSON
                        sample++;
                        snprintf(msg, 128, "{\r\n\t\"samplenr\": %d,\r\n\t\"timestamp\": %llu,\r\n\t\"temperature\": %d\r\n}\r\n", sample, timestamp, (int8_t)read);
                        uart.write(msg);
                    }
                    else uart.write("Unable to connect to temperature sensor.\r\n");
                }
                else if(read & standby_bit) {
                    uart.write("Standby is still set. No temperature for you!\r\n");
                }
                else uart.write("Data is not ready.\r\n");
            }
            else uart.write("Unable to connect to temperature sensor.\r\n");
        }
        //Sleep between button reads.
        Sleep(DELAY_BETWEEN_I2C);
        timestamp += DELAY_BETWEEN_I2C;
        //Stanby timeout handling.
        standby_timer += DELAY_BETWEEN_I2C;
        if((standby_timer >= STANDBY_TIMEOUT) && i2c_temp.read(reg_control, &read, 1) && !(read & standby_bit)) {
            uint8_t standby = 0x80;

            Sleep(DELAY_BETWEEN_I2C);
            timestamp += DELAY_BETWEEN_I2C;

            if(!i2c_temp.write(reg_control, &standby, 1)) uart.write("Unable to connect to temperature sensor. Standby wasn't set.\r\n");
            else uart.write("Stanby mode on.\r\n");
        }
    }
    return 0 ;
}
