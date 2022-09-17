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
#include "MorseSender.h"
#include "LpcUart.h"
#include <string.h>

void empty_buffer(char *buf, unsigned int buf_len);

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

    //DigitalIoPin sw1(0, 17, true, true, true);
    DigitalIoPin a0(0, 8, false, true, false);
    DigitalIoPin led_r(0, 25, false, true, true);
    MorseSender ms(&a0, &led_r);

#if 0
    //Test strings
    ms.send("SOS\n\r");
    ms.send("TTT\n\r");
    ms.send("AAA\n\r");
    ms.send("Save me from this nightmare.\n\r");
    ms.send("You'd better     work as ; expected!\n\r");
#endif
	LpcPinMap none = {-1, -1}; // unused pin has negative values in it
	LpcPinMap txpin = { 0, 18 }; // transmit pin that goes to debugger's UART->USB converter
	LpcPinMap rxpin = { 0, 13 }; // receive pin that goes to debugger's UART->USB converter
	LpcUartConfig cfg = { LPC_USART0, 115200, UART_CFG_DATALEN_8 | UART_CFG_PARITY_NONE | UART_CFG_STOPLEN_1, false, txpin, rxpin, none, none };
	LpcUart uart(cfg);

    char c;
    char buf[81] = {0};
    bool idle = true;
    volatile static int q = 0;
    unsigned int i = 0;
    while(1) {
        if(uart.read(c)) {
            // Echo back what we receive
            if (c == '\n') {
                uart.write('\r'); // precede linefeed with carriage return
                c = '\r';
            }
            uart.write(c);
            if (c == '\r') {
                uart.write('\n'); // send line feed after carriage return
            }
            
            buf[i] = c;
            if(i == 80) i = 0;
            else ++i;

            if (c == '\r' && strlen(buf) > 1) {
                char str[82] = {0};
                if (i == 0) {
                    strcpy(str, buf);
                    empty_buffer(buf, 81);
                    str[81] = '\n';
                    str[82] = '\0';
                }
                else if(buf[i + 1] == 0) {
                    strcpy(str, buf);
                    empty_buffer(buf, 81);
                    unsigned int len = strlen(str);
                    str[len] = '\n';
                    str[len + 1] = '\0';
                }
                else {
                    unsigned int e = 0;
                    for(unsigned int w = i + 1; w < 81; ++w) {
                        str[e] = buf[w];
                        buf[w] = 0;
                        ++e;
                    }
                    for(unsigned int w = 0; w <= i; ++w) {
                        str[e] = buf[w];
                        buf[w] = 0;
                        ++e;
                    }
                    unsigned int len = strlen(str);
                    str[len] = '\n';
                    str[len + 1] = '\0';
                }
                i = 0;
                char command[82] = {0};
                char text[82] = {0};
                //sscanf(str, "%s %s", command, text);
                sscanf(str, "%s ", command);
                strcpy(text, str + strlen(command) + 1);
                // Debug
                #if 1
                uart.write("\r\nLOOK!\r\n");
                uart.write(command);
                uart.write("\r\n---------\r\n");
                uart.write(text);
                #endif
            }
            else if (buf[0] == '\r') {
                buf[0] = '\0';
                i = 0;
            }



            //if(i < 79) {
            //    if (c == '\n' || c == '\r') {
            //        str[i] = '\n';
            //        str[i + 1] = '\0';
            //        ms.send(str);
            //        i = 0;
            //    }
            //    else {
            //        str[i] = (char) c;
            //        ++i;
            //    }
            //}
            //else {
            //    str[i] = (char) c;
            //    str[i + 1] = '\0';
            //    ms.send(str);
            //    i = 0;
            //}
        }
        q++;
        __asm volatile ("nop");
    }
    return 0;
}

void empty_buffer(char *buf, unsigned int buf_len) {
    for (unsigned i = 0; i < buf_len; ++i) {
        buf[i] = 0;
    }
}