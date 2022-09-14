/*
 * MorseSender.h
 *
 *  Created on: 14 Sep 2022
 *      Author: marin
 */

#ifndef MORSESENDER_H_
#define MORSESENDER_H_

#include "DigitalIoPin.h"
#include <string>
#include <ctype.h>

struct MorseCode {
	char symbol;
	unsigned char code[7];
};

class MorseSender {
public:
	MorseSender(DigitalIoPin *opin, DigitalIoPin *led);
	void send(const char *str) const;
	void send(std::string str) const;
	virtual ~MorseSender();
private:
	void send_char(unsigned int ch) const;
	DigitalIoPin *opin;
	DigitalIoPin *led;
	const unsigned char DOT = 1;
	const unsigned char DASH = 3;
	const unsigned int dot_time = 20;
	const struct MorseCode ITU_morse[37] = {
	{ 'A', { DOT, DASH } }, // A
	{ 'B', { DASH, DOT, DOT, DOT } }, // B
	{ 'C', { DASH, DOT, DASH, DOT } }, // C
	{ 'D', { DASH, DOT, DOT } }, // D
	{ 'E', { DOT } }, // E
	{ 'F', { DOT, DOT, DASH, DOT } }, // F
	{ 'G', { DASH, DASH, DOT } }, // G
	{ 'H', { DOT, DOT, DOT, DOT } }, // H
	{ 'I', { DOT, DOT } }, // I
	{ 'J', { DOT, DASH, DASH, DASH } }, // J
	{ 'K', { DASH, DOT, DASH } }, // K
	{ 'L', { DOT, DASH, DOT, DOT } }, // L
	{ 'M', { DASH, DASH } }, // M
	{ 'N', { DASH, DOT } }, // N
	{ 'O', { DASH, DASH, DASH } }, // O
	{ 'P', { DOT, DASH, DASH, DOT } }, // P
	{ 'Q', { DASH, DASH, DOT, DASH } }, // Q
	{ 'R', { DOT, DASH, DOT } }, // R
	{ 'S', { DOT, DOT, DOT } }, // S
	{ 'T', { DASH } }, // T
	{ 'U', { DOT, DOT, DASH } }, // U
	{ 'V', { DOT, DOT, DOT, DASH } }, // V
	{ 'W', { DOT, DASH, DASH } }, // W
	{ 'X', { DASH, DOT, DOT, DASH } }, // X
	{ 'Y', { DASH, DOT, DASH, DASH } }, // Y
	{ 'Z', { DASH, DASH, DOT, DOT } }, // Z
	{ '1', { DOT, DASH, DASH, DASH, DASH } }, // 1
	{ '2', { DOT, DOT, DASH, DASH, DASH } }, // 2
	{ '3', { DOT, DOT, DOT, DASH, DASH } }, // 3
	{ '4', { DOT, DOT, DOT, DOT, DASH } }, // 4
	{ '5', { DOT, DOT, DOT, DOT, DOT } }, // 5
	{ '6', { DASH, DOT, DOT, DOT, DOT } }, // 6
	{ '7', { DASH, DASH, DOT, DOT, DOT } }, // 7
	{ '8', { DASH, DASH, DASH, DOT, DOT } }, // 8
	{ '9', { DASH, DASH, DASH, DASH, DOT } }, // 9
	{ '0', { DASH, DASH, DASH, DASH, DASH } }, // 0
	{ 0, { 0 } } // terminating entry - Do not remove!
	};

};

#endif /* MORSESENDER_H_ */
