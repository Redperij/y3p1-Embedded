#define S_TO_MS(n)  (n * 1000) //Seconds to milliseconds
#define BACK_TIME_MS S_TO_MS(10) //Determines timeout for "back" button push.

#if defined (__USE_LPCOPEN)
#if defined(NO_BOARD_LIB)
#include "chip.h"
#else
#include "board.h"
#endif
#endif

#include <cr_section_macros.h>
#include "LiquidCrystal.h"
#include "DigitalIoPin.h"
#include "SimpleMenu.h"
#include "IntegerEdit.h"
#include "LpcUart.h"
#include <atomic>

static volatile std::atomic_int counter;
#ifdef __cplusplus 
extern "C" { 
#endif 
/** 
 * @brief Handle interrupt from SysTick timer 
 * @return  Nothing 
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
	LpcUart *uart = new LpcUart(cfg);

    //RIT
    Chip_RIT_Init(LPC_RITIMER);
    //NVIC_EnableIRQ(RITIMER_IRQn);
    
    //Buttons init.
    DigitalIoPin sw1(0, 17, true, true, true);
    DigitalIoPin sw2(1, 11, true, true, true);
    DigitalIoPin sw3(1, 9, true, true, true);

    //LCD pins init.
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

    //LCD
    LiquidCrystal *lcd = new LiquidCrystal(&rs, &en, &d4, &d5, &d6, &d7);
    lcd->begin(16,2);
    lcd->setCursor(0,0);

    //Menu
    SimpleMenu menu(uart);
    IntegerEdit *time = new IntegerEdit(lcd, uart, std::string("Time"), 0, 10);
    IntegerEdit *blank = new IntegerEdit(lcd, uart, std::string("Blank"), 100, 200);
    IntegerEdit *light = new IntegerEdit(lcd, uart, std::string("Light"), 0, 2);
    menu.addItem(new MenuItem(time));
    menu.addItem(new MenuItem(blank));
    menu.addItem(new MenuItem(light));
    time->setValue(5);
    blank->setValue(121);
    light->setValue(0);

    menu.event(MenuItem::show); //Display first menu item

    bool sw1_pressed = false; //"ok" button flag.
    bool sw2_pressed = false; //"down" button flag.
    bool sw3_pressed = false; //"up" button flag.
    unsigned int timeout = 0; //Timeout for "back" call.
    while(1) {
        Sleep(1);
        timeout++;
        if(sw1.read()) {
            sw1_pressed = true;
        }
        else if(sw1_pressed){
            timeout = 0;
            sw1_pressed = false;
            menu.event(MenuItem::ok);
        }
        if(sw2.read()) {
            sw2_pressed = true;
        }
        else if(sw2_pressed){
            timeout = 0;
            sw2_pressed = false;
            menu.event(MenuItem::down);
        }
        if(sw3.read()) {
            sw3_pressed = true;
        }
        else if(sw3_pressed){
            timeout = 0;
            sw3_pressed = false;
            menu.event(MenuItem::up);
        }
        //"back" is called every 10 seconds
        if(timeout >= BACK_TIME_MS) {
            timeout = 0;
            menu.event(MenuItem::back);
        }
    }
    return 0 ;
}
