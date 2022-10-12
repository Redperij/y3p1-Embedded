/*
 * IntegerEdit.h
 *
 *  Created on: 2.2.2016
 *      Author: krl
 */

#ifndef INTEGEREDIT_H_
#define INTEGEREDIT_H_

#include "PropertyEdit.h"
#include "LiquidCrystal.h"
#include "LpcUart.h"
#include <string>

class IntegerEdit: public PropertyEdit {
public:
	IntegerEdit(LiquidCrystal *lcd_, LpcUart *duart_, std::string editTitle, int16_t lbound = -999, int16_t ubound = 9999, uint16_t step = 1);
	virtual ~IntegerEdit();
	void increment();
	void decrement();
	void accept();
	void cancel();
	void setFocus(bool focus);
	bool getFocus();
	void display();
	void displayDebugValue();
	int getValue();
	int getStep();
	void setValue(int value);
private:
	void save();
	LiquidCrystal *lcd;
	LpcUart *duart;
	std::string title;
	int value;
	int edit;
	bool focus;
	int16_t lbound; //Lower bound
	int16_t ubound; //Upper bound
	uint16_t step; //Determines increment / decrement step per up/down event.
};

#endif /* INTEGEREDIT_H_ */
