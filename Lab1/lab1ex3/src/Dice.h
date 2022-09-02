/*
 * Dice.h
 *
 *  Created on: 31 Aug 2022
 *      Author: Daniil Marinec
 */

#ifndef DICE_H_
#define DICE_H_

#include "DigitalIoPin.h"

class Dice {
public:
	Dice();
	virtual ~Dice();
	void Show(int number);
private:
	DigitalIoPin pd[7];// = {{1, 9, false, true, false}, {0, 29, false, true, false}, {0, 9, false, true, false}, {0, 10, false, true, false}, {0, 16, false, true, false}, {1, 3, false, true, false}, {0, 0, false, true, false}};
};

#endif /* DICE_H_ */
