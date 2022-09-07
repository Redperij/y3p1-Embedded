/*
 * LedController.cpp
 *
 *  Created on: 7 Sep 2022
 *      Author: marin
 */

#include "LedController.h"

LedController::LedController() : led_r{0, 25 ,false ,true, true}, led_g{0, 3 ,false ,true, true}, led_b{1, 1 ,false ,true, true} {
    //Turn LEDs off
    led_r.write(false);
    led_g.write(false);
    led_b.write(false);
}

LedController::~LedController() {
}

bool LedController::get_r_state() {
	return this->led_r.read();
}

bool LedController::get_g_state() {
	return this->led_g.read();
}

bool LedController::get_b_state() {
	return this->led_b.read();
}

void LedController::switch_led(uint8_t led) {
	switch (led)
	{
	case 0:
		if(this->get_r_state()) this->led_r.write(false);
		else this->led_r.write(true);
		break;
	case 1:
		if(this->get_g_state()) this->led_g.write(false);
		else this->led_g.write(true);
		break;
	case 2:
		if(this->get_b_state()) this->led_b.write(false);
		else this->led_b.write(true);
		break;
	default:
		//Must never get here.
		break;
	}
}
