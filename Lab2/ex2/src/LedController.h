/*
 * LedController.h
 *
 *  Created on: 7 Sep 2022
 *      Author: marin
 */

#ifndef LEDCONTROLLER_H_
#define LEDCONTROLLER_H_

#include "DigitalIoPin.h"

class LedController {
public:
	LedController();
	virtual ~LedController();
	bool get_r_state();
	bool get_g_state();
	bool get_b_state();
	void switch_led(uint8_t led);
private:
	DigitalIoPin led_r;
	DigitalIoPin led_g;
	DigitalIoPin led_b;
};

#endif /* LEDCONTROLLER_H_ */
