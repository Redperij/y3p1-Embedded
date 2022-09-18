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
void strtolower(char *str);

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
    //Morse init.
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

    char c; //Character read by UART.
    char buf[81] = {0}; //Buffer for input.
    volatile static int q = 0; //Cycle counter.
    unsigned int i = 0; //Buffer tracking.
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

            //Buffer is not filled yet -> write character to buffer.
            if (i < 80 && c != '\0') {
                buf[i] = c;
                ++i;
            }

            //On line feed after receiving some input -> parse input.
            if (c == '\r' && strlen(buf) > 1) {
                char str[81] = {0};
                char command[81] = {0};
                strcpy(str, buf);
                
                // Restore empty buffer state.
                empty_buffer(buf, 81);
                i = 0;
                
                //Regard received string as "[command] [value]".
                sscanf(str, "%s ", command); //Getting command.
                strcpy(str, str + strlen(command) + 1); //Getting value.
                
                #if 0 // Debug
                uart.write("\r\n->Received:\r\n->");
                uart.write(command);
                uart.write("\r\n->---------\r\n->");
                uart.write(str);
                #endif

                //Command recognition.
                strtolower(command);
                if (!strcmp(command, "wpm")) {
                    //Regard str as possible number. It must be in range between 3 and 200.
                    int wpm = 0;
                    sscanf(str, "%d", &wpm);
                    if (wpm > 2 && wpm < 201) ms.set_wpm(wpm);
                    else uart.write("\r\n->Given value is invalid. Valid range is from 3 to 200.\r\n");
                }
                else if (!strcmp(command, "send")) {
                    if (strlen(str) > 0) ms.send(str);
                    else uart.write("\r\n->No text to send.\r\n");
                }
                else if (!strcmp(command, "set")) {
                    char temp[6] = {0};
                    uart.write("\r\n->Current wpm is: ");
                    snprintf(temp, 6, "%d", ms.get_wpm());
                    uart.write(temp);
                    uart.write("\r\n->Current dot length is: ");
                    snprintf(temp, 6, "%d", ms.get_dot_length());
                    uart.write(temp);
                    uart.write("\r\n");
                }
                else {
                    uart.write("\r\n->Command was not recognised.\r\n->Possible commands:\r\n");
                    uart.write("->wpm [number], send [text], set\r\n");
                }
            }
            //Lone line feed -> ignore it.
            else if (buf[0] == '\r') {
                // Restore empty buffer state.
                buf[0] = '\0';
                i = 0;
            }
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

void strtolower(char *str) {
    for (unsigned int i = 0; i < strlen(str); ++i) {
        str[i] = tolower(str[i]);
    }
}