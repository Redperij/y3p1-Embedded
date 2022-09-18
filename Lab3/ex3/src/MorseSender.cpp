/*
 * MorseSender.cpp
 *
 *  Created on: 14 Sep 2022
 *      Author: marin
 */

#include "MorseSender.h"
//It's ugly, I know.
#include "chip.h"
#include "board.h"
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

MorseSender::MorseSender(DigitalIoPin *opin, DigitalIoPin *led) : opin(opin), led(led) {
	this->led->write(false);
	this->opin->write(false);
	//SysTick
    uint32_t sysTickRate;
	Chip_Clock_SetSysTickClockDiv(1);
	sysTickRate = Chip_Clock_GetSysTickClockRate();
	SysTick_Config(sysTickRate / 1000);
}

MorseSender::~MorseSender() {
}

/**
 * @brief Sends string via opin as a morse code.
 * 
 * @param str Null terminated string to send as a morse code.
 */
void MorseSender::send(const char *str) const {
	bool prev_wspace = false;
	Sleep(this->dot_time * 7 * this->DOT); // Wait before sending anything.
	for (int i = 0; str[i] != '\0'; ++i) {
		//If encountered whitespace -> sleep for 4 dots, since 3 dots were already skipped due to the last letter of the word printing
		//(or it's the start, doesn't matter so much).
		if (isspace(str[i]) && !prev_wspace) {
			Sleep(this->dot_time * 4 * this->DOT);
			prev_wspace = true;
		} 
		//Skip all the following whitespaces and continue with character.
		else if(!isspace(str[i])) {
			prev_wspace = false;
			//If valid character -> pass it as an uppercase to printing.
			if (isalnum(str[i])) send_char((unsigned int) toupper(str[i]));
			//Invalid charactes turn to 'X'.
			else send_char((unsigned int)'X');
		}
	}
	Sleep(this->dot_time * 7 * this->DOT); // Wait after sending everything.
}
/**
 * @brief Sends string via opin as a morse code.
 * 
 * @param str Null terminated string to send as a morse code.
 */
void MorseSender::send(std::string str) const {
	send(str.c_str());
}
/**
 * @brief Sets dot length according to the provided words per minute value.
 * 
 * @param wpm Words per minute.
 */
void MorseSender::set_wpm(const int wpm) {
	//minute = 60 sec = 60000 millisec
	//one word = 60 dot lengths
	//'n' wpm = 'n' words in 60000 millisec meaning 60 * n dot lengths in 60000 millisec.
	if (wpm > 2 && wpm < 201) this->dot_time = 60000 / (60 * wpm);
}
/**
 * @brief Returns words per minute value.
 * 
 * @return unsigned int words per minute.
 */
unsigned int MorseSender::get_wpm() const {
	return 60000 / (this->dot_time * 60);
}
/**
 * @brief Returns dot length value.
 * 
 * @return unsigned int dot length.
 */
unsigned int MorseSender::get_dot_length() const {
	return this->dot_time;
}

void MorseSender::send_char(unsigned int ch) const {
	unsigned int q = -1; //it is done in order to check the first element. (Fully aware about the overflow)
	do {
		++q;
		if(ITU_morse[q].symbol == ch) {
			for (int i = 0; ITU_morse[q].code[i] != 0; ++i) {
				this->led->write(true);
				this->opin->write(true);
				Sleep(this->dot_time * ITU_morse[q].code[i]); //Morse unit.
				this->opin->write(false);
				this->led->write(false);
				Sleep(this->dot_time * this->DOT); //Inter-unit gap.
			}
		}
	} while (ITU_morse[q].symbol != 0 && ITU_morse[q].symbol != ch);
	
	//Intercharacter gap, 1 dot was skipped in the inter-unit gap. 
	Sleep(this->dot_time * 2 * this->DOT);
}