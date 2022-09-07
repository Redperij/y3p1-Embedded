/*
 * DigitalIoPin.cpp
 *
 *  Created on: 31 Aug 2022
 *      Author: Daniil Marinec
 */

#include "DigitalIoPin.h"

DigitalIoPin::DigitalIoPin(int port, int pin, bool input, bool pullup, bool invert) : port(port), pin(pin), input(input) {
	if(input) {
		uint32_t pin_modes = IOCON_DIGMODE_EN;
		if(invert) pin_modes |= IOCON_INV_EN;
		if(pullup) pin_modes |= IOCON_MODE_PULLUP;
		else pin_modes |= IOCON_MODE_PULLDOWN;

		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, pin_modes);
    	Chip_GPIO_SetPinDIRInput(LPC_GPIO, port, pin);
	}
	else {
		Chip_IOCON_PinMuxSet(LPC_IOCON, port, pin, IOCON_MODE_INACT | IOCON_DIGMODE_EN);
    	Chip_GPIO_SetPinDIROutput(LPC_GPIO, port, pin);
	}
}

DigitalIoPin::~DigitalIoPin() {
	// TODO Auto-generated destructor stub
}

bool DigitalIoPin::read() {
	return Chip_GPIO_GetPinState(LPC_GPIO, port, pin);
}

void DigitalIoPin::write(bool value) {
	if(!this->input) return Chip_GPIO_SetPinState(LPC_GPIO, port, pin, value);
}
